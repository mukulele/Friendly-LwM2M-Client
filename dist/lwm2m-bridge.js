"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.LwM2MBridge = void 0;
const express_1 = __importDefault(require("express"));
const child_process_1 = require("child_process");
const ws_1 = __importDefault(require("ws"));
const signalk_lwm2m_config_manager_1 = require("./signalk-lwm2m-config-manager");
/**
 * LwM2M Node.js Bridge Server
 * Provides REST API and WebSocket interface for LwM2M resource management
 */
class LwM2MBridge {
    constructor(configPath = './signalk_lwm2m_config.json', port = 3000) {
        this.server = null;
        this.wss = null;
        this.cppClient = null;
        this.periodicTimers = new Map();
        this.clientState = 'stopped';
        this.clients = new Set();
        this.startTime = null;
        this.configManager = new signalk_lwm2m_config_manager_1.SignalKLwM2MConfigManager(configPath);
        this.app = (0, express_1.default)();
        this.port = port;
        this.setupMiddleware();
        this.setupRoutes();
        this.setupWebSocket();
        this.startPeriodicUpdates();
    }
    setupMiddleware() {
        this.app.use(express_1.default.json());
        this.app.use(express_1.default.static('public'));
        // CORS middleware
        this.app.use((req, res, next) => {
            res.header('Access-Control-Allow-Origin', '*');
            res.header('Access-Control-Allow-Methods', 'GET, POST, PUT, DELETE');
            res.header('Access-Control-Allow-Headers', 'Content-Type');
            next();
        });
    }
    setupRoutes() {
        // Get all objects configuration
        this.app.get('/api/config', (req, res) => {
            res.json(this.configManager.config);
        });
        // Get specific object
        this.app.get('/api/objects/:objectId', (req, res) => {
            const { objectId } = req.params;
            const objConfig = this.configManager.config.objects[objectId];
            if (!objConfig) {
                res.status(404).json({ error: 'Object not found' });
                return;
            }
            res.json({ success: true, data: objConfig });
        });
        // Get specific instance
        this.app.get('/api/objects/:objectId/instances/:instanceId', (req, res) => {
            const { objectId, instanceId } = req.params;
            const objConfig = this.configManager.config.objects[objectId];
            if (!objConfig?.instances[instanceId]) {
                res.status(404).json({ error: 'Instance not found' });
                return;
            }
            res.json({ success: true, data: objConfig.instances[instanceId] });
        });
        // Get instance values (resolved)
        this.app.get('/api/objects/:objectId/instances/:instanceId/values', async (req, res) => {
            try {
                const { objectId, instanceId } = req.params;
                const values = await this.configManager.getInstanceValues(objectId, instanceId);
                res.json({ success: true, data: values });
            }
            catch (error) {
                res.status(404).json({
                    success: false,
                    error: error.message
                });
            }
        });
        // Get specific resource value
        this.app.get('/api/objects/:objectId/instances/:instanceId/resources/:resourceId', async (req, res) => {
            try {
                const { objectId, instanceId, resourceId } = req.params;
                const value = await this.configManager.getResourceValue(objectId, instanceId, resourceId);
                res.json({ success: true, data: { value } });
            }
            catch (error) {
                res.status(404).json({
                    success: false,
                    error: error.message
                });
            }
        });
        // Write resource value
        this.app.post('/api/write/:objectId/:instanceId/:resourceId', (req, res) => {
            const { objectId, instanceId, resourceId } = req.params;
            const { value } = req.body;
            if (value === undefined) {
                res.status(400).json({
                    success: false,
                    error: 'Value is required'
                });
                return;
            }
            try {
                this.writeResourceValue(parseInt(objectId), parseInt(instanceId), parseInt(resourceId), value);
                const resourceValue = {
                    objectId: parseInt(objectId),
                    instanceId: parseInt(instanceId),
                    resourceId: parseInt(resourceId),
                    value,
                    timestamp: new Date().toISOString(),
                    type: 'String' // This should be determined from config
                };
                this.broadcastToWebSocketClients({
                    type: 'resource_update',
                    data: resourceValue,
                    timestamp: new Date().toISOString()
                });
                res.json({
                    success: true,
                    message: `Resource ${objectId}/${instanceId}/${resourceId} updated successfully`
                });
            }
            catch (error) {
                res.status(500).json({
                    success: false,
                    error: error.message
                });
            }
        });
        // Client control endpoints
        this.app.post('/api/client/start', async (req, res) => {
            try {
                await this.startLwM2MClient();
                res.json({
                    success: true,
                    message: 'LwM2M client started successfully'
                });
            }
            catch (error) {
                res.status(500).json({
                    success: false,
                    error: error.message
                });
            }
        });
        this.app.post('/api/client/stop', async (req, res) => {
            try {
                await this.stopLwM2MClient();
                res.json({
                    success: true,
                    message: 'LwM2M client stopped successfully'
                });
            }
            catch (error) {
                res.status(500).json({
                    success: false,
                    error: error.message
                });
            }
        });
        this.app.post('/api/client/deregister', async (req, res) => {
            try {
                await this.deregisterLwM2MClient();
                res.json({
                    success: true,
                    message: 'LwM2M client deregistered successfully'
                });
            }
            catch (error) {
                res.status(500).json({
                    success: false,
                    error: error.message
                });
            }
        });
        this.app.get('/api/client/status', (req, res) => {
            const clientInfo = {
                status: this.clientState,
                uptime: this.startTime ? Date.now() - this.startTime.getTime() : 0,
                registrationStatus: this.clientState === 'running' ? 'registered' : 'unregistered',
                serverConnection: this.clientState === 'running',
                lastUpdate: new Date().toISOString()
            };
            res.json({ success: true, data: clientInfo });
        });
        // SignalK status endpoint
        this.app.get('/api/signalk/status', (req, res) => {
            const status = this.configManager.getSignalKStatus();
            res.json({ success: true, data: status });
        });
        this.app.get('/api/signalk/data', (req, res) => {
            const data = this.configManager.getSignalKData();
            res.json({ success: true, data });
        });
    }
    setupWebSocket() {
        // WebSocket server will be created when HTTP server starts
    }
    async startLwM2MClient() {
        if (this.clientState === 'running' || this.clientState === 'starting') {
            throw new Error('LwM2M client is already running or starting');
        }
        this.clientState = 'starting';
        this.broadcastStatusUpdate();
        try {
            // Build the client executable
            const buildResult = await this.buildClient();
            if (!buildResult.success) {
                throw new Error(`Build failed: ${buildResult.error}`);
            }
            // Start the client process
            const config = this.configManager.config.connection;
            const args = [
                '-s', config.serverHost,
                '-p', config.serverPort.toString(),
                '-n', config.clientName,
                '-e', config.endpointName
            ];
            if (config.lifetime) {
                args.push('-t', config.lifetime.toString());
            }
            this.cppClient = (0, child_process_1.spawn)('./build/examples/signalk_lwm2m_client', args, {
                cwd: process.cwd(),
                stdio: ['pipe', 'pipe', 'pipe']
            });
            this.cppClient.stdout?.on('data', (data) => {
                const output = data.toString();
                console.log('LwM2M Client:', output);
                const logEntry = {
                    level: 'info',
                    message: output.trim(),
                    timestamp: new Date().toISOString(),
                    source: 'lwm2m_client'
                };
                this.broadcastToWebSocketClients({
                    type: 'log',
                    data: logEntry,
                    timestamp: new Date().toISOString()
                });
            });
            this.cppClient.stderr?.on('data', (data) => {
                const output = data.toString();
                console.error('LwM2M Client Error:', output);
                const logEntry = {
                    level: 'error',
                    message: output.trim(),
                    timestamp: new Date().toISOString(),
                    source: 'lwm2m_client'
                };
                this.broadcastToWebSocketClients({
                    type: 'log',
                    data: logEntry,
                    timestamp: new Date().toISOString()
                });
            });
            this.cppClient.on('close', (code) => {
                console.log(`LwM2M client exited with code ${code}`);
                this.clientState = 'stopped';
                this.cppClient = null;
                this.startTime = null;
                this.broadcastStatusUpdate();
            });
            this.cppClient.on('error', (error) => {
                console.error('LwM2M client process error:', error);
                this.clientState = 'error';
                this.broadcastStatusUpdate();
            });
            // Wait a moment for the process to start
            await new Promise(resolve => setTimeout(resolve, 2000));
            this.clientState = 'running';
            this.startTime = new Date();
            this.broadcastStatusUpdate();
        }
        catch (error) {
            this.clientState = 'error';
            this.broadcastStatusUpdate();
            throw error;
        }
    }
    async stopLwM2MClient() {
        if (this.clientState === 'stopped' || this.clientState === 'stopping') {
            return;
        }
        this.clientState = 'stopping';
        this.broadcastStatusUpdate();
        if (this.cppClient) {
            this.cppClient.kill('SIGTERM');
            // Wait for graceful shutdown
            await new Promise((resolve) => {
                const timeout = setTimeout(() => {
                    if (this.cppClient) {
                        this.cppClient.kill('SIGKILL');
                    }
                    resolve();
                }, 5000);
                if (this.cppClient) {
                    this.cppClient.on('close', () => {
                        clearTimeout(timeout);
                        resolve();
                    });
                }
                else {
                    resolve();
                }
            });
        }
        this.clientState = 'stopped';
        this.startTime = null;
        this.broadcastStatusUpdate();
    }
    async deregisterLwM2MClient() {
        if (this.clientState !== 'running') {
            throw new Error('Client is not running');
        }
        // Send deregister command to client
        if (this.cppClient && this.cppClient.stdin) {
            this.cppClient.stdin.write('d\n');
        }
        await this.stopLwM2MClient();
    }
    async buildClient() {
        return new Promise((resolve) => {
            const { spawn } = require('child_process');
            const buildProcess = spawn('cmake', ['--build', 'build'], {
                cwd: process.cwd(),
                stdio: 'pipe'
            });
            let errorOutput = '';
            buildProcess.stderr?.on('data', (data) => {
                errorOutput += data.toString();
            });
            buildProcess.on('close', (code) => {
                if (code === 0) {
                    resolve({ success: true });
                }
                else {
                    resolve({ success: false, error: errorOutput });
                }
            });
            buildProcess.on('error', (error) => {
                resolve({ success: false, error: error.message });
            });
        });
    }
    writeResourceValue(objectId, instanceId, resourceId, value) {
        console.log(`Writing value to ${objectId}/${instanceId}/${resourceId}: ${value}`);
        // Update the configuration
        this.configManager.updateResourceValue(objectId.toString(), instanceId.toString(), resourceId.toString(), value);
        // If client is running, send write command
        if (this.cppClient && this.cppClient.stdin && this.clientState === 'running') {
            const writeCommand = `w ${objectId} ${instanceId} ${resourceId} ${value}\n`;
            this.cppClient.stdin.write(writeCommand);
            console.log(`Sent write command: ${writeCommand.trim()}`);
        }
    }
    startPeriodicUpdates() {
        // Clear existing timers
        this.periodicTimers.forEach(timer => clearInterval(timer));
        this.periodicTimers.clear();
        Object.entries(this.configManager.config.objects).forEach(([objectId, objConfig]) => {
            if (objConfig.updateInterval && objConfig.updateInterval > 0) {
                const timer = setInterval(async () => {
                    try {
                        await this.updateObjectInstances(objectId);
                    }
                    catch (error) {
                        console.error(`Error updating object ${objectId}:`, error.message);
                    }
                }, objConfig.updateInterval);
                this.periodicTimers.set(objectId, timer);
            }
        });
    }
    async updateObjectInstances(objectId) {
        const objConfig = this.configManager.config.objects[objectId];
        if (!objConfig)
            return;
        for (const instanceId of Object.keys(objConfig.instances)) {
            try {
                const values = await this.configManager.getInstanceValues(objectId, instanceId);
                // Broadcast updates to WebSocket clients
                Object.entries(values).forEach(([resourceId, value]) => {
                    const resourceValue = {
                        objectId: parseInt(objectId),
                        instanceId: parseInt(instanceId),
                        resourceId: parseInt(resourceId),
                        value,
                        timestamp: new Date().toISOString(),
                        type: 'String' // Should be determined from config
                    };
                    this.broadcastToWebSocketClients({
                        type: 'resource_update',
                        data: resourceValue,
                        timestamp: new Date().toISOString()
                    });
                });
            }
            catch (error) {
                console.error(`Error updating instance ${objectId}/${instanceId}:`, error.message);
            }
        }
    }
    broadcastStatusUpdate() {
        const clientInfo = {
            status: this.clientState,
            uptime: this.startTime ? Date.now() - this.startTime.getTime() : 0,
            registrationStatus: this.clientState === 'running' ? 'registered' : 'unregistered',
            serverConnection: this.clientState === 'running',
            lastUpdate: new Date().toISOString()
        };
        this.broadcastToWebSocketClients({
            type: 'status',
            data: clientInfo,
            timestamp: new Date().toISOString()
        });
    }
    broadcastToWebSocketClients(message) {
        const messageStr = JSON.stringify(message);
        this.clients.forEach(ws => {
            if (ws.readyState === ws_1.default.OPEN) {
                ws.send(messageStr);
            }
        });
    }
    /**
     * Start the HTTP and WebSocket servers
     */
    start() {
        this.server = this.app.listen(this.port, () => {
            console.log(`LwM2M Bridge Server running on port ${this.port}`);
            console.log(`API endpoints: http://localhost:${this.port}/api/`);
            console.log(`Web interface: http://localhost:${this.port}/`);
            console.log(`WebSocket: ws://localhost:${this.port}`);
        });
        // Create WebSocket server
        this.wss = new ws_1.default.Server({ server: this.server });
        this.wss.on('connection', (ws) => {
            console.log('WebSocket client connected');
            this.clients.add(ws);
            // Send current status to new client
            this.broadcastStatusUpdate();
            ws.on('message', (message) => {
                try {
                    const data = JSON.parse(message.toString());
                    this.handleWebSocketMessage(ws, data);
                }
                catch (error) {
                    console.error('Error parsing WebSocket message:', error.message);
                }
            });
            ws.on('close', () => {
                console.log('WebSocket client disconnected');
                this.clients.delete(ws);
            });
            ws.on('error', (error) => {
                console.error('WebSocket error:', error.message);
                this.clients.delete(ws);
            });
        });
    }
    async handleWebSocketMessage(ws, data) {
        try {
            switch (data.action) {
                case 'start_client':
                    await this.startLwM2MClient();
                    break;
                case 'stop_client':
                    await this.stopLwM2MClient();
                    break;
                case 'deregister_client':
                    await this.deregisterLwM2MClient();
                    break;
                case 'get_status':
                    this.broadcastStatusUpdate();
                    break;
                default:
                    console.log('Unknown WebSocket action:', data.action);
            }
        }
        catch (error) {
            const errorMessage = {
                type: 'error',
                data: { message: error.message },
                timestamp: new Date().toISOString()
            };
            if (ws.readyState === ws_1.default.OPEN) {
                ws.send(JSON.stringify(errorMessage));
            }
        }
    }
    /**
     * Stop the server and cleanup resources
     */
    async stop() {
        console.log('Stopping LwM2M Bridge Server...');
        // Stop LwM2M client
        await this.stopLwM2MClient();
        // Clear periodic timers
        this.periodicTimers.forEach(timer => clearInterval(timer));
        this.periodicTimers.clear();
        // Close WebSocket connections
        this.clients.forEach(ws => ws.close());
        this.clients.clear();
        // Close WebSocket server
        if (this.wss) {
            this.wss.close();
        }
        // Close HTTP server
        if (this.server) {
            await new Promise((resolve) => {
                this.server.close(() => resolve());
            });
        }
        // Cleanup configuration manager
        this.configManager.cleanup();
        console.log('LwM2M Bridge Server stopped');
    }
}
exports.LwM2MBridge = LwM2MBridge;
// Export for CLI usage
exports.default = LwM2MBridge;
//# sourceMappingURL=lwm2m-bridge.js.map