#!/usr/bin/env node
"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.LwM2MClientCLI = void 0;
const axios_1 = __importDefault(require("axios"));
const commander_1 = require("commander");
/**
 * LwM2M Client Command Line Interface
 * Control the LwM2M client through REST API commands
 */
class LwM2MClientCLI {
    constructor(baseUrl = 'http://localhost:3000') {
        this.baseUrl = baseUrl;
    }
    async makeRequest(endpoint, method = 'GET', data) {
        try {
            const config = {
                method,
                url: `${this.baseUrl}${endpoint}`,
                headers: {
                    'Content-Type': 'application/json'
                },
                data
            };
            const response = await (0, axios_1.default)(config);
            return response.data;
        }
        catch (error) {
            if (error.response) {
                return error.response.data;
            }
            else if (error.request) {
                return {
                    success: false,
                    error: 'No response from server. Is the bridge server running?'
                };
            }
            else {
                return {
                    success: false,
                    error: error.message
                };
            }
        }
    }
    /**
     * Start the LwM2M client
     */
    async startClient() {
        console.log('Starting LwM2M client...');
        const response = await this.makeRequest('/api/client/start', 'POST');
        if (response.success) {
            console.log('✓', response.message || 'LwM2M client started successfully');
        }
        else {
            console.error('✗', response.error || 'Failed to start client');
            process.exit(1);
        }
    }
    /**
     * Stop the LwM2M client
     */
    async stopClient() {
        console.log('Stopping LwM2M client...');
        const response = await this.makeRequest('/api/client/stop', 'POST');
        if (response.success) {
            console.log('✓', response.message || 'LwM2M client stopped successfully');
        }
        else {
            console.error('✗', response.error || 'Failed to stop client');
            process.exit(1);
        }
    }
    /**
     * Deregister and stop the LwM2M client
     */
    async deregisterClient() {
        console.log('Deregistering LwM2M client...');
        const response = await this.makeRequest('/api/client/deregister', 'POST');
        if (response.success) {
            console.log('✓', response.message || 'LwM2M client deregistered successfully');
        }
        else {
            console.error('✗', response.error || 'Failed to deregister client');
            process.exit(1);
        }
    }
    /**
     * Get client status
     */
    async getStatus() {
        const response = await this.makeRequest('/api/client/status');
        if (response.success && response.data) {
            const status = response.data;
            console.log('\\n=== LwM2M Client Status ===');
            console.log(`Status: ${this.getStatusIcon(status.status)} ${status.status.toUpperCase()}`);
            console.log(`Registration: ${status.registrationStatus}`);
            console.log(`Server Connection: ${status.serverConnection ? '✓ Connected' : '✗ Disconnected'}`);
            console.log(`Uptime: ${this.formatUptime(status.uptime)}`);
            console.log(`Last Update: ${new Date(status.lastUpdate).toLocaleString()}`);
        }
        else {
            console.error('✗', response.error || 'Failed to get status');
            process.exit(1);
        }
    }
    /**
     * Register (start) the LwM2M client
     */
    async registerClient() {
        await this.startClient();
    }
    /**
     * Write a value to a specific resource
     */
    async writeValue(objectId, instanceId, resourceId, value) {
        console.log(`Writing value to resource ${objectId}/${instanceId}/${resourceId}: ${value}`);
        const endpoint = `/api/write/${objectId}/${instanceId}/${resourceId}`;
        const response = await this.makeRequest(endpoint, 'POST', { value: this.parseValue(value) });
        if (response.success) {
            console.log('✓', response.message || 'Value written successfully');
        }
        else {
            console.error('✗', response.error || 'Failed to write value');
            process.exit(1);
        }
    }
    /**
     * Read a resource value
     */
    async readValue(objectId, instanceId, resourceId) {
        console.log(`Reading resource ${objectId}/${instanceId}/${resourceId}...`);
        const endpoint = `/api/objects/${objectId}/instances/${instanceId}/resources/${resourceId}`;
        const response = await this.makeRequest(endpoint);
        if (response.success && response.data) {
            console.log(`✓ Resource value: ${JSON.stringify(response.data.value, null, 2)}`);
        }
        else {
            console.error('✗', response.error || 'Failed to read value');
            process.exit(1);
        }
    }
    /**
     * List all objects in configuration
     */
    async listObjects() {
        console.log('Fetching objects configuration...');
        const response = await this.makeRequest('/api/config');
        if (response.success && response.data) {
            const config = response.data;
            console.log('\\n=== LwM2M Objects ===');
            Object.entries(config.objects).forEach(([objectId, objConfig]) => {
                console.log(`\\n📦 Object ${objectId}: ${objConfig.description}`);
                Object.entries(objConfig.instances).forEach(([instanceId, instConfig]) => {
                    console.log(`  └─ Instance ${instanceId}: ${instConfig.description}`);
                    Object.entries(instConfig.resources).forEach(([resourceId, resConfig]) => {
                        console.log(`     └─ Resource ${resourceId}: ${resConfig.name} (${resConfig.type}, ${resConfig.access})`);
                    });
                });
            });
        }
        else {
            console.error('✗', response.error || 'Failed to fetch configuration');
            process.exit(1);
        }
    }
    /**
     * Get SignalK integration status
     */
    async getSignalKStatus() {
        console.log('Checking SignalK integration status...');
        const response = await this.makeRequest('/api/signalk/status');
        if (response.success && response.data) {
            const status = response.data;
            console.log('\\n=== SignalK Integration ===');
            console.log(`Connection: ${status.connected ? '✓ Connected' : '✗ Disconnected'}`);
            console.log(`Data Points: ${status.dataPoints}`);
            // Also get some data if connected
            if (status.connected) {
                const dataResponse = await this.makeRequest('/api/signalk/data');
                if (dataResponse.success && dataResponse.data) {
                    const dataCount = Object.keys(dataResponse.data).length;
                    console.log(`Available Data Streams: ${dataCount}`);
                    // Show first few data points as examples
                    const entries = Object.entries(dataResponse.data).slice(0, 3);
                    if (entries.length > 0) {
                        console.log('\\nRecent Data (sample):');
                        entries.forEach(([path, value]) => {
                            console.log(`  ${path}: ${JSON.stringify(value)}`);
                        });
                    }
                }
            }
        }
        else {
            console.error('✗', response.error || 'Failed to get SignalK status');
            process.exit(1);
        }
    }
    /**
     * Parse value from string to appropriate type
     */
    parseValue(value) {
        // Try to parse as number
        if (/^-?\\d+(\\.\\d+)?$/.test(value)) {
            return parseFloat(value);
        }
        // Try to parse as boolean
        if (value.toLowerCase() === 'true')
            return true;
        if (value.toLowerCase() === 'false')
            return false;
        // Try to parse as JSON
        try {
            return JSON.parse(value);
        }
        catch {
            // Return as string
            return value;
        }
    }
    /**
     * Get status icon for client state
     */
    getStatusIcon(status) {
        switch (status) {
            case 'running': return '🟢';
            case 'stopped': return '🔴';
            case 'starting': return '🟡';
            case 'stopping': return '🟡';
            case 'error': return '💥';
            default: return '❓';
        }
    }
    /**
     * Format uptime in human readable format
     */
    formatUptime(uptimeMs) {
        const seconds = Math.floor(uptimeMs / 1000);
        const minutes = Math.floor(seconds / 60);
        const hours = Math.floor(minutes / 60);
        const days = Math.floor(hours / 24);
        if (days > 0) {
            return `${days}d ${hours % 24}h ${minutes % 60}m`;
        }
        else if (hours > 0) {
            return `${hours}h ${minutes % 60}m`;
        }
        else if (minutes > 0) {
            return `${minutes}m ${seconds % 60}s`;
        }
        else {
            return `${seconds}s`;
        }
    }
}
exports.LwM2MClientCLI = LwM2MClientCLI;
// Set up CLI commands
const program = new commander_1.Command();
program
    .name('lwm2m-cli')
    .description('LwM2M Client Command Line Interface')
    .version('1.0.0')
    .option('-u, --url <url>', 'Bridge server URL', 'http://localhost:3000');
program
    .command('start')
    .description('Start the LwM2M client')
    .action(async (options) => {
    const cli = new LwM2MClientCLI(program.opts().url);
    await cli.startClient();
});
program
    .command('stop')
    .description('Stop the LwM2M client')
    .action(async (options) => {
    const cli = new LwM2MClientCLI(program.opts().url);
    await cli.stopClient();
});
program
    .command('register')
    .description('Register (start) the LwM2M client')
    .action(async (options) => {
    const cli = new LwM2MClientCLI(program.opts().url);
    await cli.registerClient();
});
program
    .command('deregister')
    .description('Deregister and stop the LwM2M client')
    .action(async (options) => {
    const cli = new LwM2MClientCLI(program.opts().url);
    await cli.deregisterClient();
});
program
    .command('status')
    .description('Get LwM2M client status')
    .action(async (options) => {
    const cli = new LwM2MClientCLI(program.opts().url);
    await cli.getStatus();
});
program
    .command('write')
    .description('Write a value to a resource')
    .argument('<objectId>', 'Object ID')
    .argument('<instanceId>', 'Instance ID')
    .argument('<resourceId>', 'Resource ID')
    .argument('<value>', 'Value to write')
    .action(async (objectId, instanceId, resourceId, value, options) => {
    const cli = new LwM2MClientCLI(program.opts().url);
    await cli.writeValue(objectId, instanceId, resourceId, value);
});
program
    .command('read')
    .description('Read a resource value')
    .argument('<objectId>', 'Object ID')
    .argument('<instanceId>', 'Instance ID')
    .argument('<resourceId>', 'Resource ID')
    .action(async (objectId, instanceId, resourceId, options) => {
    const cli = new LwM2MClientCLI(program.opts().url);
    await cli.readValue(objectId, instanceId, resourceId);
});
program
    .command('list')
    .description('List all objects and resources')
    .action(async (options) => {
    const cli = new LwM2MClientCLI(program.opts().url);
    await cli.listObjects();
});
program
    .command('signalk')
    .description('Check SignalK integration status')
    .action(async (options) => {
    const cli = new LwM2MClientCLI(program.opts().url);
    await cli.getSignalKStatus();
});
// Parse command line arguments
program.parse();
exports.default = LwM2MClientCLI;
