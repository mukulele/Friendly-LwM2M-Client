"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || (function () {
    var ownKeys = function(o) {
        ownKeys = Object.getOwnPropertyNames || function (o) {
            var ar = [];
            for (var k in o) if (Object.prototype.hasOwnProperty.call(o, k)) ar[ar.length] = k;
            return ar;
        };
        return ownKeys(o);
    };
    return function (mod) {
        if (mod && mod.__esModule) return mod;
        var result = {};
        if (mod != null) for (var k = ownKeys(mod), i = 0; i < k.length; i++) if (k[i] !== "default") __createBinding(result, mod, k[i]);
        __setModuleDefault(result, mod);
        return result;
    };
})();
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.SignalKLwM2MConfigManager = void 0;
const fs = __importStar(require("fs"));
const child_process_1 = require("child_process");
const ws_1 = __importDefault(require("ws"));
/**
 * Enhanced LwM2M Configuration Manager with SignalK Integration
 * Handles JSON configuration parsing, dynamic value resolution, and SignalK data sources
 */
class SignalKLwM2MConfigManager {
    constructor(configPath) {
        this.configPath = configPath;
        this.config = {};
        this.valueCache = new Map();
        this.signalkWs = null;
        this.signalkData = new Map();
        this.reconnectTimer = null;
        this.loadConfig();
        this.initializeSignalK();
    }
    /**
     * Load and parse JSON configuration
     */
    loadConfig() {
        try {
            const configData = fs.readFileSync(this.configPath, 'utf8');
            this.config = JSON.parse(configData);
            console.log('Configuration loaded successfully');
        }
        catch (error) {
            console.error('Error loading config:', error.message);
            throw error;
        }
    }
    /**
     * Initialize SignalK WebSocket connection
     */
    initializeSignalK() {
        if (!this.config.signalk?.enabled) {
            console.log('SignalK integration disabled');
            return;
        }
        const { host, port, protocol } = this.config.signalk;
        const signalkUrl = `${protocol}://${host}:${port}/signalk/v1/stream`;
        this.connectToSignalK(signalkUrl);
    }
    /**
     * Connect to SignalK WebSocket stream
     */
    connectToSignalK(url) {
        try {
            console.log(`Connecting to SignalK at ${url}`);
            this.signalkWs = new ws_1.default(url);
            this.signalkWs.on('open', () => {
                console.log('Connected to SignalK server');
                // Subscribe to all navigation data
                const subscription = {
                    context: 'vessels.self',
                    subscribe: [
                        {
                            path: '*',
                            period: 1000,
                            format: 'delta',
                            policy: 'ideal',
                            minPeriod: 200
                        }
                    ]
                };
                if (this.signalkWs && this.signalkWs.readyState === ws_1.default.OPEN) {
                    this.signalkWs.send(JSON.stringify(subscription));
                    console.log('Subscribed to SignalK data streams');
                }
            });
            this.signalkWs.on('message', (data) => {
                this.handleSignalKMessage(data);
            });
            this.signalkWs.on('close', () => {
                console.log('SignalK connection closed, attempting reconnect...');
                this.scheduleReconnect(url);
            });
            this.signalkWs.on('error', (error) => {
                console.error('SignalK WebSocket error:', error.message);
                this.scheduleReconnect(url);
            });
        }
        catch (error) {
            console.error('Failed to connect to SignalK:', error.message);
            this.scheduleReconnect(url);
        }
    }
    /**
     * Handle incoming SignalK messages
     */
    handleSignalKMessage(data) {
        try {
            const message = JSON.parse(data.toString());
            if (message.updates) {
                message.updates.forEach(update => {
                    update.values.forEach(valueUpdate => {
                        const fullPath = `vessels.self.${valueUpdate.path}`;
                        this.signalkData.set(fullPath, valueUpdate.value);
                        // Cache with timestamp for debugging
                        this.valueCache.set(`signalk:${valueUpdate.path}`, {
                            value: valueUpdate.value,
                            timestamp: update.timestamp,
                            source: update.source.label
                        });
                    });
                });
            }
        }
        catch (error) {
            console.error('Error parsing SignalK message:', error.message);
        }
    }
    /**
     * Schedule reconnection to SignalK
     */
    scheduleReconnect(url) {
        if (this.reconnectTimer) {
            clearTimeout(this.reconnectTimer);
        }
        const reconnectInterval = this.config.signalk?.reconnectInterval || 5000;
        this.reconnectTimer = setTimeout(() => {
            console.log('Attempting to reconnect to SignalK...');
            this.connectToSignalK(url);
        }, reconnectInterval);
    }
    /**
     * Resolve dynamic value from various sources
     */
    async resolveValue(value) {
        if (typeof value !== 'string') {
            return value;
        }
        // Handle different value source prefixes
        if (value.startsWith('config:')) {
            return this.resolveConfigValue(value);
        }
        else if (value.startsWith('signalk:')) {
            return this.resolveSignalKValue(value);
        }
        else if (value.startsWith('command:')) {
            return await this.resolveCommandValue(value);
        }
        else if (value.startsWith('system:')) {
            return this.resolveSystemValue(value);
        }
        else if (value.startsWith('file:')) {
            return this.resolveFileValue(value);
        }
        return value;
    }
    /**
     * Resolve configuration value references
     */
    resolveConfigValue(value) {
        const path = value.replace('config:', '');
        const pathParts = path.split('.');
        let current = this.config;
        for (const part of pathParts) {
            if (current && typeof current === 'object' && part in current) {
                current = current[part];
            }
            else {
                console.warn(`Config path not found: ${path}`);
                return null;
            }
        }
        return current;
    }
    /**
     * Resolve SignalK data path
     */
    resolveSignalKValue(value) {
        const path = value.replace('signalk:', '');
        const cached = this.valueCache.get(value);
        if (cached) {
            return cached.value;
        }
        // Try direct path lookup
        const directValue = this.signalkData.get(`vessels.self.${path}`);
        if (directValue !== undefined) {
            return directValue;
        }
        console.warn(`SignalK path not found: ${path}`);
        return null;
    }
    /**
     * Execute command and return result
     */
    async resolveCommandValue(value) {
        const command = value.replace('command:', '');
        return new Promise((resolve, reject) => {
            (0, child_process_1.exec)(command, { timeout: 5000 }, (error, stdout, stderr) => {
                if (error) {
                    console.error(`Command error: ${error.message}`);
                    resolve(null);
                    return;
                }
                const output = stdout.trim();
                // Try to parse as number or boolean
                if (/^\d+(\.\d+)?$/.test(output)) {
                    resolve(parseFloat(output));
                }
                else if (output === 'true' || output === 'false') {
                    resolve(output === 'true');
                }
                else {
                    resolve(output);
                }
            });
        });
    }
    /**
     * Resolve system information
     */
    resolveSystemValue(value) {
        const systemPath = value.replace('system:', '');
        switch (systemPath) {
            case 'timestamp':
                return new Date().toISOString();
            case 'hostname':
                return require('os').hostname();
            case 'platform':
                return process.platform;
            case 'uptime':
                return process.uptime();
            case 'memory.used':
                const memUsage = process.memoryUsage();
                return Math.round(memUsage.heapUsed / 1024 / 1024); // MB
            case 'memory.total':
                const memTotal = process.memoryUsage();
                return Math.round(memTotal.heapTotal / 1024 / 1024); // MB
            default:
                console.warn(`Unknown system path: ${systemPath}`);
                return null;
        }
    }
    /**
     * Read value from file
     */
    resolveFileValue(value) {
        const filePath = value.replace('file:', '');
        try {
            const content = fs.readFileSync(filePath, 'utf8').trim();
            // Try to parse as JSON first
            try {
                return JSON.parse(content);
            }
            catch {
                // Try to parse as number
                if (/^\d+(\.\d+)?$/.test(content)) {
                    return parseFloat(content);
                }
                // Return as string
                return content;
            }
        }
        catch (error) {
            console.error(`Error reading file ${filePath}:`, error.message);
            return null;
        }
    }
    /**
     * Get all resource values for a specific object/instance
     */
    async getInstanceValues(objectId, instanceId) {
        const objConfig = this.config.objects[objectId];
        if (!objConfig) {
            throw new Error(`Object ${objectId} not found in configuration`);
        }
        const instance = objConfig.instances[instanceId];
        if (!instance) {
            throw new Error(`Instance ${instanceId} not found in object ${objectId}`);
        }
        const values = {};
        for (const [resourceId, resourceConfig] of Object.entries(instance.resources)) {
            try {
                values[resourceId] = await this.resolveValue(resourceConfig.value);
            }
            catch (error) {
                console.error(`Error resolving value for ${objectId}/${instanceId}/${resourceId}:`, error.message);
                values[resourceId] = null;
            }
        }
        return values;
    }
    /**
     * Get resolved value for specific resource
     */
    async getResourceValue(objectId, instanceId, resourceId) {
        const objConfig = this.config.objects[objectId];
        if (!objConfig?.instances[instanceId]?.resources[resourceId]) {
            throw new Error(`Resource ${objectId}/${instanceId}/${resourceId} not found`);
        }
        const resourceConfig = objConfig.instances[instanceId].resources[resourceId];
        return await this.resolveValue(resourceConfig.value);
    }
    /**
     * Update resource value in configuration
     */
    updateResourceValue(objectId, instanceId, resourceId, newValue) {
        const objConfig = this.config.objects[objectId];
        if (!objConfig?.instances[instanceId]?.resources[resourceId]) {
            throw new Error(`Resource ${objectId}/${instanceId}/${resourceId} not found`);
        }
        objConfig.instances[instanceId].resources[resourceId].value = newValue;
        console.log(`Updated resource ${objectId}/${instanceId}/${resourceId} = ${newValue}`);
    }
    /**
     * Save current configuration to file
     */
    saveConfig() {
        try {
            fs.writeFileSync(this.configPath, JSON.stringify(this.config, null, 2));
            console.log('Configuration saved successfully');
        }
        catch (error) {
            console.error('Error saving configuration:', error.message);
            throw error;
        }
    }
    /**
     * Get SignalK connection status
     */
    getSignalKStatus() {
        return {
            connected: this.signalkWs?.readyState === ws_1.default.OPEN,
            dataPoints: this.signalkData.size
        };
    }
    /**
     * Get cached SignalK data for debugging
     */
    getSignalKData() {
        const result = {};
        this.signalkData.forEach((value, key) => {
            result[key] = value;
        });
        return result;
    }
    /**
     * Cleanup resources
     */
    cleanup() {
        if (this.signalkWs) {
            this.signalkWs.close();
        }
        if (this.reconnectTimer) {
            clearTimeout(this.reconnectTimer);
        }
        this.valueCache.clear();
        this.signalkData.clear();
    }
}
exports.SignalKLwM2MConfigManager = SignalKLwM2MConfigManager;
//# sourceMappingURL=signalk-lwm2m-config-manager.js.map