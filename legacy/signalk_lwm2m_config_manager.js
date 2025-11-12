const fs = require('fs');
const { exec } = require('child_process');
const WebSocket = require('ws');

/**
 * Enhanced LwM2M Configuration Manager with SignalK Integration
 * Handles JSON configuration parsing, dynamic value resolution, and SignalK data sources
 */
class SignalKLwM2MConfigManager {
    constructor(configPath) {
        this.configPath = configPath;
        this.config = null;
        this.valueCache = new Map();
        this.signalkWs = null;
        this.signalkData = new Map();
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
        } catch (error) {
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
        console.log(`Connecting to SignalK: ${url}`);
        
        try {
            this.signalkWs = new WebSocket(url);
            
            this.signalkWs.on('open', () => {
                console.log('Connected to SignalK');
                this.subscribeToSignalKPaths();
            });
            
            this.signalkWs.on('message', (data) => {
                this.handleSignalKMessage(data);
            });
            
            this.signalkWs.on('close', () => {
                console.log('SignalK connection closed, attempting reconnect in 5s');
                setTimeout(() => this.connectToSignalK(url), 5000);
            });
            
            this.signalkWs.on('error', (error) => {
                console.error('SignalK WebSocket error:', error.message);
            });
        } catch (error) {
            console.error('Failed to connect to SignalK:', error.message);
            setTimeout(() => this.connectToSignalK(url), 5000);
        }
    }

    /**
     * Subscribe to SignalK data paths
     */
    subscribeToSignalKPaths() {
        if (!this.config.signalk.subscription_paths) return;

        const subscription = {
            context: 'vessels.self',
            subscribe: this.config.signalk.subscription_paths.map(path => ({
                path: path,
                period: 1000,
                minPeriod: 200
            }))
        };

        this.signalkWs.send(JSON.stringify(subscription));
        console.log('Subscribed to SignalK paths:', this.config.signalk.subscription_paths);
    }

    /**
     * Handle SignalK WebSocket messages
     */
    handleSignalKMessage(data) {
        try {
            const message = JSON.parse(data.toString());
            
            if (message.updates) {
                message.updates.forEach(update => {
                    if (update.values) {
                        update.values.forEach(value => {
                            this.signalkData.set(value.path, {
                                value: value.value,
                                timestamp: update.timestamp || new Date().toISOString()
                            });
                        });
                    }
                });
            }
        } catch (error) {
            console.error('Error parsing SignalK message:', error.message);
        }
    }

    /**
     * Get connection configuration
     */
    getConnectionConfig() {
        return this.config.connection || {};
    }

    /**
     * Get client configuration
     */
    getClientConfig() {
        return this.config.client || {};
    }

    /**
     * Get all objects configuration
     */
    getObjects() {
        return this.config.objects || {};
    }

    /**
     * Get specific resource configuration
     */
    getResource(objectId, instanceId, resourceId) {
        const objConfig = this.config.objects[objectId];
        if (!objConfig) return null;
        
        const instConfig = objConfig.instances[instanceId];
        if (!instConfig) return null;
        
        return instConfig.resources[resourceId] || null;
    }

    /**
     * Resolve resource value (fixed value or dynamic path)
     */
    async resolveResourceValue(objectId, instanceId, resourceId) {
        const resource = this.getResource(objectId, instanceId, resourceId);
        if (!resource) {
            throw new Error(`Resource ${objectId}/${instanceId}/${resourceId} not found`);
        }

        // If fixed value is defined, return it
        if (resource.hasOwnProperty('value')) {
            return resource.value;
        }

        // If value_path is defined, resolve it dynamically
        if (resource.value_path) {
            return await this.resolveValuePath(resource.value_path, resource.type, resource.parser);
        }

        throw new Error(`No value or value_path defined for resource ${objectId}/${instanceId}/${resourceId}`);
    }

    /**
     * Resolve dynamic value from various path types
     */
    async resolveValuePath(valuePath, dataType, parser = null) {
        const cacheKey = valuePath;
        
        // Check cache (with TTL if needed)
        if (this.valueCache.has(cacheKey)) {
            const cached = this.valueCache.get(cacheKey);
            if (Date.now() - cached.timestamp < 10000) { // 10 second cache
                return cached.value;
            }
        }

        let resolvedValue;

        if (valuePath.startsWith('config:')) {
            resolvedValue = await this.getConfigValue(valuePath.substring(7));
        } else if (valuePath.startsWith('signalk:')) {
            resolvedValue = await this.getSignalKValue(valuePath.substring(8));
        } else if (valuePath.startsWith('file:')) {
            resolvedValue = await this.readFileValue(valuePath.substring(5));
        } else if (valuePath.startsWith('command:')) {
            resolvedValue = await this.executeCommandValue(valuePath.substring(8));
        } else if (valuePath.startsWith('system:')) {
            resolvedValue = await this.getSystemValue(valuePath.substring(7));
        } else {
            // Assume it's a file path
            resolvedValue = await this.readFileValue(valuePath);
        }

        // Apply parser if specified
        if (parser && resolvedValue) {
            resolvedValue = await this.applyParser(resolvedValue, parser);
        }

        // Convert to appropriate data type
        const typedValue = this.convertToType(resolvedValue, dataType);
        
        // Cache the result
        this.valueCache.set(cacheKey, {
            value: typedValue,
            timestamp: Date.now()
        });

        return typedValue;
    }

    /**
     * Get configuration value by path
     */
    async getConfigValue(configPath) {
        const parts = configPath.split(':');
        let value = this.config;
        
        for (const part of parts) {
            if (value && typeof value === 'object' && value.hasOwnProperty(part)) {
                value = value[part];
            } else {
                return '';
            }
        }

        // Handle special case for server URI construction
        if (parts.length === 2 && parts[0] === 'connection.serverHost' && parts[1] === 'connection.serverPort') {
            const host = this.config.connection.serverHost;
            const port = this.config.connection.serverPort;
            const protocol = this.config.connection.dtlsConfig?.enableDTLS ? 'coaps' : 'coap';
            return `${protocol}://${host}:${port}`;
        }

        return value || '';
    }

    /**
     * Get SignalK value by path
     */
    async getSignalKValue(signalkPath) {
        const data = this.signalkData.get(signalkPath);
        if (data) {
            return data.value;
        }
        return '';
    }

    /**
     * Apply parser to extracted value
     */
    async executeCommandValue(command) {
        return new Promise((resolve, reject) => {
            exec(command, { timeout: 5000 }, (error, stdout, stderr) => {
                if (error) {
                    console.error(`Command error: ${command}`, error.message);
                    resolve('');
                } else {
                    resolve(stdout.trim());
                }
            });
        });
    }

    /**
     * Apply parser command to raw value
     */
    async applyParser(rawValue, parser) {
        const command = `echo '${rawValue}' | ${parser}`;
        return await this.executeCommandValue(command);
    }

    /**
     * Read value from file
     */
    async readFileValue(filePath) {
        return new Promise((resolve, reject) => {
            fs.readFile(filePath, 'utf8', (err, data) => {
                if (err) {
                    console.error(`Error reading file ${filePath}:`, err.message);
                    resolve('');
                } else {
                    resolve(data.trim());
                }
            });
        });
    }

    /**
     * Get system values
     */
    async getSystemValue(systemType) {
        switch (systemType) {
            case 'time':
                return Math.floor(Date.now() / 1000);
            case 'uptime':
                const uptime = await this.executeCommandValue('cat /proc/uptime | cut -d" " -f1');
                return parseFloat(uptime);
            case 'hostname':
                return await this.executeCommandValue('hostname');
            default:
                return '';
        }
    }

    /**
     * Convert string value to appropriate LwM2M data type
     */
    convertToType(value, dataType) {
        if (value === '' || value === null || value === undefined) {
            return this.getDefaultValue(dataType);
        }

        switch (dataType) {
            case 'integer':
                const intValue = parseInt(value);
                return isNaN(intValue) ? 0 : intValue;
            case 'float':
                const floatValue = parseFloat(value);
                return isNaN(floatValue) ? 0.0 : floatValue;
            case 'boolean':
                return value.toLowerCase() === 'true' || value === '1' || value === true;
            case 'time':
                if (typeof value === 'string' && value.includes('T')) {
                    // ISO timestamp
                    return Math.floor(new Date(value).getTime() / 1000);
                }
                return typeof value === 'number' ? value : Math.floor(Date.now() / 1000);
            case 'string':
            case 'opaque':
            default:
                return String(value);
        }
    }

    /**
     * Get default value for data type
     */
    getDefaultValue(dataType) {
        switch (dataType) {
            case 'integer':
            case 'time':
                return 0;
            case 'float':
                return 0.0;
            case 'boolean':
                return false;
            case 'string':
            case 'opaque':
            default:
                return '';
        }
    }

    /**
     * Get resources that need periodic updates
     */
    getPeriodicResources() {
        const intervals = this.config.update_intervals || {};
        return Object.entries(intervals).map(([resource, interval]) => {
            const [objectId, instanceId, resourceId] = resource.split('/');
            return {
                objectId,
                instanceId,
                resourceId,
                interval: interval * 1000
            };
        });
    }

    /**
     * Get notification configurations
     */
    getNotificationConfig(objectId, instanceId, resourceId) {
        const resourceKey = `${objectId}/${instanceId}/${resourceId}`;
        return this.config.notifications[resourceKey] || null;
    }

    /**
     * Generate C++ configuration code for connection
     */
    generateCppConnectionConfig() {
        const conn = this.getConnectionConfig();
        const client = this.getClientConfig();
        
        return `
// Auto-generated LwM2M connection configuration
void configureConnection(Connection& connection, WppClient& client) {
    // Server configuration
    const char* serverHost = "${conn.serverHost}";
    const uint16_t serverPort = ${conn.serverPort};
    const char* endpointName = "${conn.endpoint}";
    
    // Client configuration  
    const uint32_t lifetime = ${client.lifetime};
    const bool useIPv4 = ${conn.useIPv4 ? 'true' : 'false'};
    const bool requestBootstrap = ${conn.request_bootstrap ? 'true' : 'false'};
    
    // DTLS configuration
    const bool enableDTLS = ${conn.dtlsConfig.enableDTLS ? 'true' : 'false'};
    const char* pskIdentity = "${conn.dtlsConfig.PSK_Identity}";
    const char* psk = "${conn.dtlsConfig.PSK}";
    
    printf("Connecting to: %s:%d\\n", serverHost, serverPort);
    printf("Endpoint: %s, Lifetime: %d\\n", endpointName, lifetime);
}
`;
    }

    /**
     * Get SignalK status
     */
    getSignalKStatus() {
        return {
            enabled: this.config.signalk?.enabled || false,
            connected: this.signalkWs && this.signalkWs.readyState === WebSocket.OPEN,
            dataPoints: this.signalkData.size,
            lastUpdate: this.signalkData.size > 0 ? 
                Math.max(...Array.from(this.signalkData.values()).map(d => new Date(d.timestamp).getTime())) : null
        };
    }
}

module.exports = SignalKLwM2MConfigManager;

// Example usage
if (require.main === module) {
    const manager = new SignalKLwM2MConfigManager('./signalk_lwm2m_config.json');
    
    // Test configuration resolution
    async function testConfiguration() {
        try {
            console.log('=== Connection Configuration ===');
            console.log(JSON.stringify(manager.getConnectionConfig(), null, 2));
            
            console.log('=== Client Configuration ===');
            console.log(JSON.stringify(manager.getClientConfig(), null, 2));
            
            console.log('=== Testing Resource Value Resolution ===');
            
            // Test connection config resolution
            const serverUri = await manager.resolveResourceValue('0', '0', '0');
            console.log('LwM2M Server URI:', serverUri);
            
            // Test SignalK integration after connection established
            setTimeout(async () => {
                try {
                    const latitude = await manager.resolveResourceValue('6', '0', '0');
                    console.log('Latitude from SignalK:', latitude);
                    
                    const signalkStatus = manager.getSignalKStatus();
                    console.log('SignalK Status:', signalkStatus);
                } catch (error) {
                    console.log('SignalK test (expected if no SignalK server):', error.message);
                }
            }, 2000);
            
        } catch (error) {
            console.error('Error:', error.message);
        }
    }
    
    testConfiguration();
}