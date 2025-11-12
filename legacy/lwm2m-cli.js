#!/usr/bin/env node

const axios = require('axios');
const { program } = require('commander');

/**
 * LwM2M Client Command Line Interface
 * Control the LwM2M client through REST API commands
 */
class LwM2MClientCLI {
    constructor(baseUrl = 'http://localhost:3000') {
        this.baseUrl = baseUrl;
    }

    async makeRequest(endpoint, method = 'GET', data = null) {
        try {
            const config = {
                method,
                url: `${this.baseUrl}${endpoint}`,
                headers: {
                    'Content-Type': 'application/json'
                }
            };

            if (data) {
                config.data = data;
            }

            const response = await axios(config);
            return response.data;
        } catch (error) {
            if (error.response) {
                throw new Error(`API Error: ${error.response.status} - ${error.response.data.error || error.response.statusText}`);
            } else if (error.request) {
                throw new Error('Connection Error: Unable to connect to LwM2M Bridge');
            } else {
                throw new Error(`Request Error: ${error.message}`);
            }
        }
    }

    async startClient() {
        console.log('🚀 Starting LwM2M Client...');
        const result = await this.makeRequest('/api/client/start', 'POST');
        
        if (result.success) {
            console.log('✅ Success:', result.message);
            console.log(`   State: ${result.state}`);
            console.log(`   PID: ${result.pid}`);
        } else {
            console.log('❌ Failed:', result.message || result.error);
        }
        
        return result;
    }

    async stopClient() {
        console.log('⏹️  Stopping LwM2M Client...');
        const result = await this.makeRequest('/api/client/stop', 'POST');
        
        if (result.success) {
            console.log('✅ Success:', result.message);
            console.log(`   State: ${result.state}`);
        } else {
            console.log('❌ Failed:', result.message || result.error);
        }
        
        return result;
    }

    async deregisterClient() {
        console.log('📤 Deregistering LwM2M Client...');
        const result = await this.makeRequest('/api/client/deregister', 'POST');
        
        if (result.success) {
            console.log('✅ Success:', result.message);
            console.log(`   State: ${result.state}`);
        } else {
            console.log('❌ Failed:', result.message || result.error);
        }
        
        return result;
    }

    async registerClient() {
        console.log('📥 Registering LwM2M Client...');
        const result = await this.makeRequest('/api/client/register', 'POST');
        
        if (result.success) {
            console.log('✅ Success:', result.message);
            console.log(`   State: ${result.state}`);
        } else {
            console.log('❌ Failed:', result.message || result.error);
        }
        
        return result;
    }

    async restartClient() {
        console.log('🔄 Restarting LwM2M Client...');
        const result = await this.makeRequest('/api/client/restart', 'POST');
        
        if (result.success) {
            console.log('✅ Success:', result.message);
            console.log(`   State: ${result.state}`);
        } else {
            console.log('❌ Failed:', result.message || result.error);
        }
        
        return result;
    }

    async getStatus() {
        const result = await this.makeRequest('/api/client/status');
        
        console.log('📊 LwM2M Client Status:');
        console.log(`   State: ${this.getStateIcon(result.state)} ${result.state}`);
        console.log(`   Active: ${result.active ? '✅ Yes' : '❌ No'}`);
        console.log(`   PID: ${result.pid || 'N/A'}`);
        console.log(`   Uptime: ${this.formatUptime(result.uptime)}`);
        console.log(`   Server: ${result.connection.serverHost}:${result.connection.serverPort}`);
        console.log(`   Endpoint: ${result.connection.endpoint}`);
        
        return result;
    }

    async getHealth() {
        const result = await this.makeRequest('/api/health');
        
        console.log('🏥 Bridge Health Status:');
        console.log(`   Bridge: ${result.status === 'healthy' ? '✅' : '❌'} ${result.status}`);
        console.log(`   Client State: ${this.getStateIcon(result.client.state)} ${result.client.state}`);
        console.log(`   Client Active: ${result.client.active ? '✅ Yes' : '❌ No'}`);
        console.log(`   SignalK Connected: ${result.signalk.connected ? '✅ Yes' : '❌ No'}`);
        console.log(`   SignalK Data Points: ${result.signalk.dataPoints}`);
        
        return result;
    }

    async getConnection() {
        const result = await this.makeRequest('/api/connection');
        
        console.log('🔗 Connection Configuration:');
        console.log(`   Server: ${result.connection.serverHost}:${result.connection.serverPort}`);
        console.log(`   Listen Port: ${result.connection.listenPort}`);
        console.log(`   Endpoint: ${result.connection.endpoint}`);
        console.log(`   IPv4: ${result.connection.useIPv4 ? '✅' : '❌'}`);
        console.log(`   Bootstrap: ${result.connection.request_bootstrap ? '✅' : '❌'}`);
        console.log(`   DTLS: ${result.connection.dtlsConfig.enableDTLS ? '✅' : '❌'}`);
        console.log(`   Lifetime: ${result.client.lifetime}s`);
        console.log(`   Binding: ${result.client.binding}`);
        
        return result;
    }

    getStateIcon(state) {
        const icons = {
            'stopped': '⏹️',
            'starting': '🚀',
            'running': '▶️',
            'registered': '✅',
            'stopping': '⏸️',
            'error': '❌',
            'registering': '📥',
            'deregistering': '📤'
        };
        return icons[state] || '❓';
    }

    formatUptime(uptime) {
        if (!uptime || uptime === 0) return 'N/A';
        
        const seconds = Math.floor(uptime / 1000);
        const minutes = Math.floor(seconds / 60);
        const hours = Math.floor(minutes / 60);
        
        if (hours > 0) {
            return `${hours}h ${minutes % 60}m ${seconds % 60}s`;
        } else if (minutes > 0) {
            return `${minutes}m ${seconds % 60}s`;
        } else {
            return `${seconds}s`;
        }
    }
}

// CLI Setup
const cli = new LwM2MClientCLI();

program
    .name('lwm2m-cli')
    .description('LwM2M Client Command Line Interface')
    .version('1.0.0');

program
    .command('start')
    .description('Start the LwM2M client')
    .action(async () => {
        try {
            await cli.startClient();
        } catch (error) {
            console.error('Error:', error.message);
            process.exit(1);
        }
    });

program
    .command('stop')
    .description('Stop the LwM2M client')
    .action(async () => {
        try {
            await cli.stopClient();
        } catch (error) {
            console.error('Error:', error.message);
            process.exit(1);
        }
    });

program
    .command('deregister')
    .description('Deregister the LwM2M client from server')
    .action(async () => {
        try {
            await cli.deregisterClient();
        } catch (error) {
            console.error('Error:', error.message);
            process.exit(1);
        }
    });

program
    .command('register')
    .description('Register the LwM2M client to server')
    .action(async () => {
        try {
            await cli.registerClient();
        } catch (error) {
            console.error('Error:', error.message);
            process.exit(1);
        }
    });

program
    .command('restart')
    .description('Restart the LwM2M client')
    .action(async () => {
        try {
            await cli.restartClient();
        } catch (error) {
            console.error('Error:', error.message);
            process.exit(1);
        }
    });

program
    .command('status')
    .description('Get LwM2M client status')
    .action(async () => {
        try {
            await cli.getStatus();
        } catch (error) {
            console.error('Error:', error.message);
            process.exit(1);
        }
    });

program
    .command('health')
    .description('Get bridge health status')
    .action(async () => {
        try {
            await cli.getHealth();
        } catch (error) {
            console.error('Error:', error.message);
            process.exit(1);
        }
    });

program
    .command('config')
    .description('Get connection configuration')
    .action(async () => {
        try {
            await cli.getConnection();
        } catch (error) {
            console.error('Error:', error.message);
            process.exit(1);
        }
    });

// Parse command line arguments
program.parse();

module.exports = LwM2MClientCLI;