const express = require('express');
const { spawn } = require('child_process');
const WebSocket = require('ws');
const SignalKLwM2MConfigManager = require('./signalk_lwm2m_config_manager');

/**
 * LwM2M Node.js Bridge Server
 * Provides REST API and WebSocket interface for LwM2M resource management
 */
class LwM2MBridge {
    constructor(configPath = './signalk_lwm2m_config.json', port = 3000) {
        this.configManager = new SignalKLwM2MConfigManager(configPath);
        this.app = express();
        this.port = port;
        this.cppClient = null;
        this.periodicTimers = new Map();
        this.clientState = 'stopped'; // stopped, starting, running, stopping, error
        
        this.setupMiddleware();
        this.setupRoutes();
        this.setupWebSocket();
        this.startPeriodicUpdates();
    }

    setupMiddleware() {
        this.app.use(express.json());
        this.app.use(express.static('public'));
        
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
                return res.status(404).json({ error: 'Object not found' });
            }
            
            res.json(objConfig);
        });

        // Get resource value
        this.app.get('/api/objects/:objectId/instances/:instanceId/resources/:resourceId', async (req, res) => {
            try {
                const { objectId, instanceId, resourceId } = req.params;
                const value = await this.configManager.resolveResourceValue(objectId, instanceId, resourceId);
                const resource = this.configManager.getResource(objectId, instanceId, resourceId);
                
                res.json({
                    objectId,
                    instanceId,
                    resourceId,
                    value,
                    type: resource?.type || 'unknown',
                    name: resource?.name || 'Unknown Resource',
                    timestamp: Date.now()
                });
            } catch (error) {
                res.status(404).json({ error: error.message });
            }
        });

        // Update resource value
        this.app.put('/api/objects/:objectId/instances/:instanceId/resources/:resourceId', async (req, res) => {
            try {
                const { objectId, instanceId, resourceId } = req.params;
                const { value } = req.body;
                
                const resource = this.configManager.getResource(objectId, instanceId, resourceId);
                if (!resource) {
                    return res.status(404).json({ error: 'Resource not found' });
                }

                // Check if resource is writable
                if (!resource.access.includes('W')) {
                    return res.status(403).json({ error: 'Resource is not writable' });
                }

                // Convert value to appropriate type
                const typedValue = this.configManager.convertToType(value, resource.type);
                
                // Send update command to C++ client
                const command = {
                    action: 'write_resource',
                    objectId,
                    instanceId,
                    resourceId,
                    value: typedValue,
                    timestamp: Date.now()
                };

                this.sendCommandToCppClient(command);

                res.json({
                    success: true,
                    objectId,
                    instanceId,
                    resourceId,
                    value: typedValue,
                    timestamp: Date.now()
                });

            } catch (error) {
                res.status(400).json({ error: error.message });
            }
        });

        // Get all resource values for an instance
        this.app.get('/api/objects/:objectId/instances/:instanceId', async (req, res) => {
            try {
                const { objectId, instanceId } = req.params;
                const objConfig = this.configManager.config.objects[objectId];
                
                if (!objConfig?.instances[instanceId]) {
                    return res.status(404).json({ error: 'Object instance not found' });
                }

                const instance = objConfig.instances[instanceId];
                const resources = {};

                for (const [resourceId, resourceConfig] of Object.entries(instance.resources)) {
                    try {
                        const value = await this.configManager.resolveResourceValue(objectId, instanceId, resourceId);
                        resources[resourceId] = {
                            name: resourceConfig.name,
                            type: resourceConfig.type,
                            access: resourceConfig.access,
                            value: value,
                            unit: resourceConfig.unit || null
                        };
                    } catch (error) {
                        resources[resourceId] = {
                            name: resourceConfig.name,
                            type: resourceConfig.type,
                            access: resourceConfig.access,
                            error: error.message
                        };
                    }
                }

                res.json({
                    objectId,
                    instanceId,
                    name: objConfig.name,
                    resources,
                    timestamp: Date.now()
                });

            } catch (error) {
                res.status(500).json({ error: error.message });
            }
        });

        // Trigger resource update
        this.app.post('/api/objects/:objectId/instances/:instanceId/resources/:resourceId/update', async (req, res) => {
            try {
                const { objectId, instanceId, resourceId } = req.params;
                
                // Force update of dynamic resource
                const value = await this.configManager.resolveResourceValue(objectId, instanceId, resourceId);
                
                // Send notify command to C++ client
                const command = {
                    action: 'notify_resource',
                    objectId,
                    instanceId,
                    resourceId,
                    value,
                    timestamp: Date.now()
                };

                this.sendCommandToCppClient(command);

                res.json({
                    success: true,
                    objectId,
                    instanceId,
                    resourceId,
                    value,
                    timestamp: Date.now()
                });

            } catch (error) {
                res.status(400).json({ error: error.message });
            }
        });

        // Get connection configuration
        this.app.get('/api/connection', (req, res) => {
            res.json({
                connection: this.configManager.getConnectionConfig(),
                client: this.configManager.getClientConfig()
            });
        });

        // Get SignalK status
        this.app.get('/api/signalk/status', (req, res) => {
            res.json(this.configManager.getSignalKStatus());
        });

        // Generate C++ connection config
        this.app.get('/api/cpp/connection', (req, res) => {
            res.set('Content-Type', 'text/plain');
            res.send(this.configManager.generateCppConnectionConfig());
        });

        // LwM2M Client Control Commands
        
        // Start LwM2M Client
        this.app.post('/api/client/start', async (req, res) => {
            try {
                const result = await this.startLwM2MClient();
                res.json(result);
            } catch (error) {
                res.status(500).json({ 
                    success: false, 
                    error: error.message,
                    timestamp: Date.now()
                });
            }
        });

        // Stop LwM2M Client
        this.app.post('/api/client/stop', async (req, res) => {
            try {
                const result = await this.stopLwM2MClient();
                res.json(result);
            } catch (error) {
                res.status(500).json({ 
                    success: false, 
                    error: error.message,
                    timestamp: Date.now()
                });
            }
        });

        // Deregister LwM2M Client from server
        this.app.post('/api/client/deregister', async (req, res) => {
            try {
                const result = await this.deregisterLwM2MClient();
                res.json(result);
            } catch (error) {
                res.status(500).json({ 
                    success: false, 
                    error: error.message,
                    timestamp: Date.now()
                });
            }
        });

        // Register LwM2M Client to server
        this.app.post('/api/client/register', async (req, res) => {
            try {
                const result = await this.registerLwM2MClient();
                res.json(result);
            } catch (error) {
                res.status(500).json({ 
                    success: false, 
                    error: error.message,
                    timestamp: Date.now()
                });
            }
        });

        // Restart LwM2M Client
        this.app.post('/api/client/restart', async (req, res) => {
            try {
                const stopResult = await this.stopLwM2MClient();
                if (stopResult.success) {
                    // Wait a moment before restarting
                    setTimeout(async () => {
                        try {
                            await this.startLwM2MClient();
                        } catch (error) {
                            console.error('Error restarting client:', error.message);
                        }
                    }, 2000);
                }
                
                res.json({
                    success: true,
                    message: 'Client restart initiated',
                    state: this.clientState,
                    timestamp: Date.now()
                });
            } catch (error) {
                res.status(500).json({ 
                    success: false, 
                    error: error.message,
                    timestamp: Date.now()
                });
            }
        });

        // Get LwM2M Client status
        this.app.get('/api/client/status', (req, res) => {
            res.json({
                state: this.clientState,
                pid: this.cppClient ? this.cppClient.pid : null,
                active: this.cppClient !== null && !this.cppClient.killed,
                uptime: this.cppClient ? Date.now() - this.cppClient.startTime : 0,
                connection: this.configManager.getConnectionConfig(),
                timestamp: Date.now()
            });
        });

        // Health check
        this.app.get('/api/health', (req, res) => {
            const signalkStatus = this.configManager.getSignalKStatus();
            res.json({
                status: 'healthy',
                client: {
                    state: this.clientState,
                    active: this.cppClient !== null && !this.cppClient.killed,
                    pid: this.cppClient ? this.cppClient.pid : null
                },
                signalk: signalkStatus,
                timestamp: Date.now()
            });
        });
    }

    setupWebSocket() {
        this.server = require('http').createServer(this.app);
        this.wss = new WebSocket.Server({ server: this.server });

        this.wss.on('connection', (ws) => {
            console.log('WebSocket client connected');

            ws.on('message', async (message) => {
                try {
                    const data = JSON.parse(message);
                    
                    switch (data.type) {
                        case 'subscribe_resource':
                            // Subscribe to resource updates
                            ws.resourceSubscription = `${data.objectId}/${data.instanceId}/${data.resourceId}`;
                            ws.send(JSON.stringify({
                                type: 'subscription_confirmed',
                                resource: ws.resourceSubscription
                            }));
                            break;

                        case 'get_resource':
                            const value = await this.configManager.resolveResourceValue(
                                data.objectId, data.instanceId, data.resourceId
                            );
                            ws.send(JSON.stringify({
                                type: 'resource_value',
                                objectId: data.objectId,
                                instanceId: data.instanceId,
                                resourceId: data.resourceId,
                                value,
                                timestamp: Date.now()
                            }));
                            break;

                        case 'client_control':
                            let result;
                            switch (data.command) {
                                case 'start':
                                    result = await this.startLwM2MClient();
                                    break;
                                case 'stop':
                                    result = await this.stopLwM2MClient();
                                    break;
                                case 'deregister':
                                    result = await this.deregisterLwM2MClient();
                                    break;
                                case 'register':
                                    result = await this.registerLwM2MClient();
                                    break;
                                default:
                                    result = {
                                        success: false,
                                        error: `Unknown command: ${data.command}`
                                    };
                            }
                            
                            ws.send(JSON.stringify({
                                type: 'client_control_response',
                                command: data.command,
                                result: result,
                                timestamp: Date.now()
                            }));
                            break;

                        case 'get_client_status':
                            ws.send(JSON.stringify({
                                type: 'client_status',
                                event: 'status_response',
                                state: this.clientState,
                                pid: this.cppClient ? this.cppClient.pid : null,
                                active: this.cppClient !== null && !this.cppClient.killed,
                                timestamp: Date.now()
                            }));
                            break;
                    }
                } catch (error) {
                    ws.send(JSON.stringify({
                        type: 'error',
                        message: error.message
                    }));
                }
            });

            ws.on('close', () => {
                console.log('WebSocket client disconnected');
            });
        });
    }

    sendCommandToCppClient(command) {
        if (this.cppClient && !this.cppClient.killed) {
            try {
                const jsonCommand = JSON.stringify(command) + '\n';
                this.cppClient.stdin.write(jsonCommand);
                console.log('Sent command to C++ client:', command.action);
            } catch (error) {
                console.error('Error sending command to C++ client:', error.message);
            }
        } else {
            console.warn('C++ client not available for command:', command.action);
        }
    }

    broadcastResourceUpdate(objectId, instanceId, resourceId, value) {
        const resourcePath = `${objectId}/${instanceId}/${resourceId}`;
        
        this.wss.clients.forEach((client) => {
            if (client.readyState === WebSocket.OPEN && 
                client.resourceSubscription === resourcePath) {
                client.send(JSON.stringify({
                    type: 'resource_update',
                    objectId,
                    instanceId,
                    resourceId,
                    value,
                    timestamp: Date.now()
                }));
            }
        });
    }

    startPeriodicUpdates() {
        const periodicResources = this.configManager.getPeriodicResources();
        
        periodicResources.forEach(resource => {
            const timer = setInterval(async () => {
                try {
                    const value = await this.configManager.resolveResourceValue(
                        resource.objectId, resource.instanceId, resource.resourceId
                    );

                    // Send update command to C++ client
                    const command = {
                        action: 'update_resource',
                        objectId: resource.objectId,
                        instanceId: resource.instanceId,
                        resourceId: resource.resourceId,
                        value,
                        timestamp: Date.now()
                    };

                    this.sendCommandToCppClient(command);
                    
                    // Broadcast to WebSocket clients
                    this.broadcastResourceUpdate(
                        resource.objectId, resource.instanceId, resource.resourceId, value
                    );

                } catch (error) {
                    console.error(`Error updating periodic resource ${resource.objectId}/${resource.instanceId}/${resource.resourceId}:`, error.message);
                }
            }, resource.interval);

            this.periodicTimers.set(`${resource.objectId}/${resource.instanceId}/${resource.resourceId}`, timer);
        });

        console.log(`Started ${periodicResources.length} periodic update timers`);
    }

    start() {
        this.server.listen(this.port, () => {
            console.log(`LwM2M Bridge Server running on port ${this.port}`);
            console.log(`REST API: http://localhost:${this.port}/api/`);
            console.log(`WebSocket: ws://localhost:${this.port}/`);
        });
    }

    /**
     * Start the LwM2M C++ Client
     */
    async startLwM2MClient() {
        if (this.cppClient && !this.cppClient.killed) {
            return {
                success: false,
                message: 'Client is already running',
                state: this.clientState,
                pid: this.cppClient.pid
            };
        }

        this.clientState = 'starting';
        
        try {
            const cppExecutable = '_build/examples/WppExample';
            const fs = require('fs');
            
            // Check if executable exists
            if (!fs.existsSync(cppExecutable)) {
                throw new Error(`C++ executable not found: ${cppExecutable}`);
            }

            console.log('Starting LwM2M C++ Client...');
            
            this.cppClient = spawn('./' + cppExecutable, [], {
                cwd: process.cwd(),
                stdio: ['pipe', 'pipe', 'pipe']
            });

            this.cppClient.startTime = Date.now();
            
            // Handle client stdout
            this.cppClient.stdout.on('data', (data) => {
                const output = data.toString().trim();
                console.log(`[LwM2M Client]: ${output}`);
                
                // Broadcast client output to WebSocket clients
                this.broadcastClientStatus('output', output);
                
                // Check for registration success
                if (output.includes('REGISTERED') || output.includes('Registration successful')) {
                    this.clientState = 'registered';
                    this.broadcastClientStatus('registered', 'Client registered successfully');
                }
            });

            // Handle client stderr
            this.cppClient.stderr.on('data', (data) => {
                const error = data.toString().trim();
                console.error(`[LwM2M Client Error]: ${error}`);
                this.broadcastClientStatus('error', error);
            });

            // Handle client exit
            this.cppClient.on('exit', (code, signal) => {
                console.log(`LwM2M Client exited with code ${code}, signal ${signal}`);
                this.clientState = code === 0 ? 'stopped' : 'error';
                this.broadcastClientStatus('exit', `Client exited with code ${code}`);
                this.cppClient = null;
            });

            // Handle client error
            this.cppClient.on('error', (error) => {
                console.error('LwM2M Client process error:', error.message);
                this.clientState = 'error';
                this.broadcastClientStatus('error', error.message);
                this.cppClient = null;
            });

            // Wait a moment to ensure process started
            await new Promise(resolve => setTimeout(resolve, 1000));
            
            if (this.cppClient && !this.cppClient.killed) {
                this.clientState = 'running';
                return {
                    success: true,
                    message: 'LwM2M Client started successfully',
                    state: this.clientState,
                    pid: this.cppClient.pid,
                    timestamp: Date.now()
                };
            } else {
                throw new Error('Client process failed to start');
            }

        } catch (error) {
            this.clientState = 'error';
            console.error('Error starting LwM2M Client:', error.message);
            throw error;
        }
    }

    /**
     * Stop the LwM2M C++ Client
     */
    async stopLwM2MClient() {
        if (!this.cppClient || this.cppClient.killed) {
            return {
                success: true,
                message: 'Client is already stopped',
                state: 'stopped',
                timestamp: Date.now()
            };
        }

        this.clientState = 'stopping';
        
        try {
            console.log('Stopping LwM2M C++ Client...');
            
            // Send graceful shutdown signal
            this.cppClient.kill('SIGTERM');
            
            // Wait for graceful shutdown or force kill after timeout
            const shutdownPromise = new Promise((resolve) => {
                const timeout = setTimeout(() => {
                    if (this.cppClient && !this.cppClient.killed) {
                        console.log('Force killing LwM2M Client...');
                        this.cppClient.kill('SIGKILL');
                    }
                    resolve();
                }, 5000); // 5 second timeout

                this.cppClient.on('exit', () => {
                    clearTimeout(timeout);
                    resolve();
                });
            });

            await shutdownPromise;
            
            this.clientState = 'stopped';
            this.cppClient = null;
            
            console.log('LwM2M Client stopped successfully');
            this.broadcastClientStatus('stopped', 'Client stopped successfully');
            
            return {
                success: true,
                message: 'LwM2M Client stopped successfully', 
                state: this.clientState,
                timestamp: Date.now()
            };

        } catch (error) {
            this.clientState = 'error';
            console.error('Error stopping LwM2M Client:', error.message);
            throw error;
        }
    }

    /**
     * Deregister LwM2M Client from server
     */
    async deregisterLwM2MClient() {
        if (!this.cppClient || this.cppClient.killed) {
            return {
                success: false,
                message: 'Client is not running',
                state: this.clientState,
                timestamp: Date.now()
            };
        }

        try {
            console.log('Deregistering LwM2M Client from server...');
            
            // Send deregister command to C++ client
            const command = {
                action: 'deregister',
                timestamp: Date.now()
            };

            this.sendCommandToCppClient(command);
            this.broadcastClientStatus('deregistering', 'Deregistration initiated');
            
            return {
                success: true,
                message: 'Deregistration command sent to client',
                state: this.clientState,
                timestamp: Date.now()
            };

        } catch (error) {
            console.error('Error deregistering LwM2M Client:', error.message);
            throw error;
        }
    }

    /**
     * Register/Re-register LwM2M Client to server
     */
    async registerLwM2MClient() {
        if (!this.cppClient || this.cppClient.killed) {
            return {
                success: false,
                message: 'Client is not running',
                state: this.clientState,
                timestamp: Date.now()
            };
        }

        try {
            console.log('Registering LwM2M Client to server...');
            
            // Send register command to C++ client
            const command = {
                action: 'register',
                timestamp: Date.now()
            };

            this.sendCommandToCppClient(command);
            this.broadcastClientStatus('registering', 'Registration initiated');
            
            return {
                success: true,
                message: 'Registration command sent to client',
                state: this.clientState,
                timestamp: Date.now()
            };

        } catch (error) {
            console.error('Error registering LwM2M Client:', error.message);
            throw error;
        }
    }

    /**
     * Broadcast client status to WebSocket clients
     */
    broadcastClientStatus(event, message) {
        const statusMessage = {
            type: 'client_status',
            event: event,
            message: message,
            state: this.clientState,
            pid: this.cppClient ? this.cppClient.pid : null,
            timestamp: Date.now()
        };

        this.wss.clients.forEach((client) => {
            if (client.readyState === WebSocket.OPEN) {
                client.send(JSON.stringify(statusMessage));
            }
        });
    }

    stop() {
        // Stop LwM2M client first
        if (this.cppClient && !this.cppClient.killed) {
            this.stopLwM2MClient();
        }

        // Clear periodic timers
        this.periodicTimers.forEach(timer => clearInterval(timer));
        this.periodicTimers.clear();

        // Close WebSocket server
        if (this.wss) {
            this.wss.close();
        }

        // Close HTTP server
        if (this.server) {
            this.server.close();
        }
    }
}

module.exports = LwM2MBridge;

// Start server if run directly
if (require.main === module) {
    const bridge = new LwM2MBridge();
    bridge.start();

    // Graceful shutdown
    process.on('SIGINT', () => {
        console.log('\nShutting down LwM2M Bridge...');
        bridge.stop();
        process.exit(0);
    });
}