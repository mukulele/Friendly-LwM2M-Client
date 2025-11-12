const fs = require('fs');
const { exec } = require('child_process');
const path = require('path');

/**
 * LwM2M Configuration Manager
 * Handles JSON configuration parsing and dynamic value resolution
 */
class LwM2MConfigManager {
    constructor(configPath) {
        this.configPath = configPath;
        this.config = null;
        this.valueCache = new Map();
        this.loadConfig();
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
            return await this.resolveValuePath(resource.value_path, resource.type);
        }

        throw new Error(`No value or value_path defined for resource ${objectId}/${instanceId}/${resourceId}`);
    }

    /**
     * Resolve dynamic value from various path types
     */
    async resolveValuePath(valuePath, dataType) {
        const cacheKey = valuePath;
        
        // Check cache (with TTL if needed)
        if (this.valueCache.has(cacheKey)) {
            const cached = this.valueCache.get(cacheKey);
            if (Date.now() - cached.timestamp < 10000) { // 10 second cache
                return cached.value;
            }
        }

        let resolvedValue;

        if (valuePath.startsWith('file:')) {
            resolvedValue = await this.readFileValue(valuePath.substring(5));
        } else if (valuePath.startsWith('command:')) {
            resolvedValue = await this.executeCommandValue(valuePath.substring(8));
        } else if (valuePath.startsWith('system:')) {
            resolvedValue = await this.getSystemValue(valuePath.substring(7));
        } else {
            // Assume it's a file path
            resolvedValue = await this.readFileValue(valuePath);
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
     * Read value from file
     */
    async readFileValue(filePath) {
        return new Promise((resolve, reject) => {
            fs.readFile(filePath, 'utf8', (err, data) => {
                if (err) {
                    console.error(`Error reading file ${filePath}:`, err.message);
                    resolve(''); // Return empty string on error
                } else {
                    resolve(data.trim());
                }
            });
        });
    }

    /**
     * Execute command and get output
     */
    async executeCommandValue(command) {
        return new Promise((resolve, reject) => {
            exec(command, { timeout: 5000 }, (error, stdout, stderr) => {
                if (error) {
                    console.error(`Command error: ${command}`, error.message);
                    resolve(''); // Return empty string on error
                } else {
                    resolve(stdout.trim());
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
                return Math.floor(Date.now() / 1000); // Unix timestamp
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
                return value.toLowerCase() === 'true' || value === '1';
            case 'time':
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
                interval: interval * 1000 // Convert to milliseconds
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
     * Generate C++ configuration code
     */
    generateCppConfig() {
        const objects = this.getObjects();
        let cppCode = `// Auto-generated LwM2M configuration\n`;
        cppCode += `#include "objects.h"\n\n`;

        for (const [objId, objConfig] of Object.entries(objects)) {
            cppCode += `// Object ${objId}: ${objConfig.name}\n`;
            
            for (const [instId, instConfig] of Object.entries(objConfig.instances)) {
                cppCode += `void init_object_${objId}_instance_${instId}(WppClient &client) {\n`;
                
                for (const [resId, resConfig] of Object.entries(instConfig.resources)) {
                    if (resConfig.hasOwnProperty('value')) {
                        cppCode += `    // Resource ${resId}: ${resConfig.name}\n`;
                        cppCode += `    // Fixed value: ${JSON.stringify(resConfig.value)}\n`;
                    } else if (resConfig.value_path) {
                        cppCode += `    // Resource ${resId}: ${resConfig.name}\n`;
                        cppCode += `    // Dynamic path: ${resConfig.value_path}\n`;
                    }
                }
                
                cppCode += `}\n\n`;
            }
        }

        return cppCode;
    }
}

module.exports = LwM2MConfigManager;

// Example usage
if (require.main === module) {
    const manager = new LwM2MConfigManager('./lwm2m_config.json');
    
    // Test resource value resolution
    async function testResolution() {
        try {
            console.log('=== Testing Resource Value Resolution ===');
            
            // Test fixed value
            const manufacturer = await manager.resolveResourceValue('3', '0', '0');
            console.log('Manufacturer:', manufacturer);
            
            // Test system time
            const currentTime = await manager.resolveResourceValue('3', '0', '13');
            console.log('Current Time:', new Date(currentTime * 1000));
            
            // Test command execution
            const memoryFree = await manager.resolveResourceValue('3', '0', '10');
            console.log('Memory Free:', memoryFree, 'bytes');
            
            // Get periodic update resources
            const periodicResources = manager.getPeriodicResources();
            console.log('Periodic Resources:', periodicResources);
            
        } catch (error) {
            console.error('Error:', error.message);
        }
    }
    
    testResolution();
}