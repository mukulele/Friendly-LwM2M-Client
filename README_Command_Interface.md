# LwM2M Client Command Interface

This document describes the Node.js command interface for controlling the LwM2M client lifecycle operations: start, stop, deregister, and register.

## 🎯 Available Control Methods

### **1. REST API Endpoints**
| Method | Endpoint | Description |
|--------|----------|-------------|
| POST | `/api/client/start` | Start the LwM2M C++ client |
| POST | `/api/client/stop` | Stop the LwM2M C++ client |
| POST | `/api/client/register` | Register client with LwM2M server |
| POST | `/api/client/deregister` | Deregister client from LwM2M server |
| POST | `/api/client/restart` | Restart the LwM2M client |
| GET | `/api/client/status` | Get current client status |

### **2. Command Line Interface**
```bash
# Install dependencies first
npm install

# Available commands:
npm run cli start        # Start the client
npm run cli stop         # Stop the client  
npm run cli register     # Register with server
npm run cli deregister   # Deregister from server
npm run cli restart      # Restart client
npm run cli status       # Get status
npm run cli health       # Get bridge health
npm run cli config       # Get connection config
```

### **3. WebSocket Commands**
```javascript
// Connect to WebSocket
const ws = new WebSocket('ws://localhost:3000');

// Send control commands
ws.send(JSON.stringify({
    type: 'client_control',
    command: 'start' // start, stop, register, deregister
}));

// Get client status
ws.send(JSON.stringify({
    type: 'get_client_status'
}));
```

### **4. Web Dashboard**
Access the control dashboard at: `http://localhost:3000/control.html`

## 🚀 Usage Examples

### **Starting the Bridge and Client**

```bash
# 1. Start the Node.js bridge
npm start

# 2. In another terminal, control the client
npm run cli start
```

### **REST API Examples**

```bash
# Start client
curl -X POST http://localhost:3000/api/client/start

# Get status
curl http://localhost:3000/api/client/status

# Stop client
curl -X POST http://localhost:3000/api/client/stop

# Register with server
curl -X POST http://localhost:3000/api/client/register

# Deregister from server
curl -X POST http://localhost:3000/api/client/deregister
```

### **Node.js Programmatic Control**

```javascript
const axios = require('axios');

class LwM2MController {
    constructor(baseUrl = 'http://localhost:3000') {
        this.baseUrl = baseUrl;
    }

    async startClient() {
        const response = await axios.post(`${this.baseUrl}/api/client/start`);
        return response.data;
    }

    async stopClient() {
        const response = await axios.post(`${this.baseUrl}/api/client/stop`);
        return response.data;
    }

    async getStatus() {
        const response = await axios.get(`${this.baseUrl}/api/client/status`);
        return response.data;
    }
}

// Usage
const controller = new LwM2MController();
await controller.startClient();
```

## 📊 Client States

| State | Description | Available Actions |
|-------|-------------|-------------------|
| `stopped` | Client is not running | start, restart |
| `starting` | Client is starting up | stop |
| `running` | Client is running but not registered | stop, register, restart |
| `registered` | Client is registered with server | stop, deregister, restart |
| `stopping` | Client is shutting down | (wait) |
| `error` | Client encountered an error | start, restart |

## 🎛️ API Response Format

### **Success Response**
```json
{
    "success": true,
    "message": "LwM2M Client started successfully",
    "state": "running",
    "pid": 12345,
    "timestamp": 1699708800000
}
```

### **Error Response**
```json
{
    "success": false,
    "error": "Client is already running",
    "state": "running",
    "timestamp": 1699708800000
}
```

### **Status Response**
```json
{
    "state": "registered",
    "pid": 12345,
    "active": true,
    "uptime": 30000,
    "connection": {
        "serverHost": "leshan.eclipseprojects.io",
        "serverPort": 5683,
        "endpoint": "signalk-lwm2m-client"
    },
    "timestamp": 1699708800000
}
```

## 🔄 WebSocket Events

### **Client Status Updates**
```javascript
{
    "type": "client_status",
    "event": "started|stopped|registered|error|output",
    "message": "Status message",
    "state": "running",
    "pid": 12345,
    "timestamp": 1699708800000
}
```

### **Control Command Responses**
```javascript
{
    "type": "client_control_response", 
    "command": "start",
    "result": {
        "success": true,
        "message": "Client started successfully"
    },
    "timestamp": 1699708800000
}
```

## 🛠️ Integration Examples

### **Automated Startup Script**
```bash
#!/bin/bash

# Start bridge
npm start &
BRIDGE_PID=$!

# Wait for bridge to start
sleep 3

# Start LwM2M client
npm run cli start

# Monitor and restart if needed
while true; do
    sleep 30
    STATUS=$(npm run cli status --silent | grep "State:" | cut -d" " -f3)
    
    if [ "$STATUS" = "error" ]; then
        echo "Client in error state, restarting..."
        npm run cli restart
    fi
done
```

### **Health Monitoring**
```javascript
const { exec } = require('child_process');

async function monitorClient() {
    setInterval(async () => {
        try {
            const status = await axios.get('http://localhost:3000/api/client/status');
            
            if (status.data.state === 'error') {
                console.log('Client in error state, restarting...');
                await axios.post('http://localhost:3000/api/client/restart');
            }
            
        } catch (error) {
            console.error('Health check failed:', error.message);
        }
    }, 30000); // Check every 30 seconds
}

monitorClient();
```

### **SignalK Plugin Integration**
```javascript
module.exports = function(app) {
    const plugin = {};
    
    plugin.start = function(options) {
        // Start LwM2M client when SignalK starts
        axios.post('http://localhost:3000/api/client/start')
            .then(response => {
                console.log('LwM2M client started:', response.data.message);
            })
            .catch(error => {
                console.error('Failed to start LwM2M client:', error.message);
            });
    };
    
    plugin.stop = function() {
        // Stop LwM2M client when SignalK stops
        axios.post('http://localhost:3000/api/client/stop')
            .then(response => {
                console.log('LwM2M client stopped:', response.data.message);
            });
    };
    
    return plugin;
};
```

## 🔧 Configuration

The client control system uses your existing configuration:
- Connection parameters from `signalk_lwm2m_config.json`
- C++ executable path: `_build/examples/WppExample`
- Bridge port: 3000 (configurable)

## 📝 Troubleshooting

### **Common Issues**

1. **"C++ executable not found"**
   ```bash
   # Build the C++ client first
   cmake --build _build --target WppExample
   ```

2. **"Connection Error: Unable to connect to LwM2M Bridge"**
   ```bash
   # Make sure the bridge is running
   npm start
   ```

3. **"Client is already running"**
   ```bash
   # Stop the client first
   npm run cli stop
   ```

### **Debug Commands**

```bash
# Check bridge health
npm run cli health

# View detailed status
npm run cli status

# Check connection config
npm run cli config

# Monitor in real-time
# Open: http://localhost:3000/control.html
```

## 🎯 Use Cases

- **Remote Device Management**: Control LwM2M clients from web interfaces
- **Automated Deployments**: Script-based client lifecycle management  
- **Marine IoT Integration**: SignalK-triggered registration/deregistration
- **Development Testing**: Rapid start/stop cycles during development
- **Monitoring Systems**: Health checks and automatic recovery

This command interface provides complete programmatic control over your LwM2M client, enabling seamless integration with existing marine IoT systems and SignalK deployments.