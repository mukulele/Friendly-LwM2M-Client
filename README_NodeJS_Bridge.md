# LwM2M Node.js Bridge with JSON Configuration

This system provides a Node.js bridge for the Friendly LwM2M Client with JSON-based configuration for objects, instances, and resources.

## Architecture Overview

```
┌─────────────────┐    ┌──────────────────┐    ┌─────────────────────┐
│   Web Dashboard │    │   Node.js Bridge │    │  C++ LwM2M Client   │
│                 │◄──►│                  │◄──►│                     │
│  - REST API     │    │  - Config Mgr    │    │  - WppClient        │
│  - WebSocket    │    │  - Value Resolver│    │  - WppTaskQueue     │
│  - Live Updates │    │  - Periodic Tasks│    │  - Object Registry  │
└─────────────────┘    └──────────────────┘    └─────────────────────┘
                                │                         │
                                ▼                         ▼
                       ┌─────────────────┐    ┌─────────────────────┐
                       │ lwm2m_config.json│    │   LwM2M Server      │
                       │ - Fixed Values   │    │   (CoAP/UDP)       │
                       │ - Dynamic Paths  │    │                     │
                       │ - Update Rules   │    │                     │
                       └─────────────────┘    └─────────────────────┘
```

## JSON Configuration Format

### Basic Structure

```json
{
  "client": {
    "endpoint_name": "FriendlyDevice001",
    "lifetime": 86400,
    "binding": "U"
  },
  "objects": {
    "objectId": {
      "name": "Object Name",
      "instances": {
        "instanceId": {
          "resources": {
            "resourceId": {
              "name": "Resource Name",
              "type": "string|integer|float|boolean|time|opaque",
              "access": "R|W|RW",
              "value": "fixed_value",
              "value_path": "dynamic_source",
              "unit": "optional_unit",
              "range": "optional_range"
            }
          }
        }
      }
    }
  },
  "update_intervals": {
    "objectId/instanceId/resourceId": interval_in_seconds
  },
  "notifications": {
    "objectId/instanceId/resourceId": {
      "condition": "value_changed|threshold|time_based",
      "threshold": "numeric_value",
      "operator": "greater_than|less_than|equal|not_equal"
    }
  }
}
```

### Resource Value Sources

#### 1. Fixed Values
```json
{
  "0": {
    "name": "Manufacturer",
    "type": "string",
    "access": "R",
    "value": "Friendly Technologies"
  }
}
```

#### 2. File Paths
```json
{
  "2": {
    "name": "Serial Number",
    "type": "string",
    "access": "R",
    "value_path": "/sys/class/dmi/id/product_serial"
  }
}
```

#### 3. System Commands
```json
{
  "10": {
    "name": "Memory Free",
    "type": "integer",
    "access": "R",
    "value_path": "command:free -b | grep Mem | awk '{print $7}'",
    "unit": "bytes"
  }
}
```

#### 4. System Values
```json
{
  "13": {
    "name": "Current Time",
    "type": "time",
    "access": "RW",
    "value_path": "system:time"
  }
}
```

## Setup Instructions

### 1. Install Node.js Dependencies

```bash
cd /home/pi/Friendly-LwM2M-Client-1
npm install
```

### 2. Configure Your Objects

Edit `lwm2m_config.json` to define your objects, instances, and resources:

```json
{
  "objects": {
    "3": {
      "name": "Device",
      "instances": {
        "0": {
          "resources": {
            "0": {
              "name": "Manufacturer",
              "type": "string",
              "access": "R",
              "value": "Your Company Name"
            },
            "9": {
              "name": "Battery Level",
              "type": "integer",
              "access": "R",
              "value_path": "/sys/class/power_supply/BAT0/capacity",
              "unit": "%"
            }
          }
        }
      }
    }
  },
  "update_intervals": {
    "3/0/9": 60
  }
}
```

### 3. Start the Node.js Bridge

```bash
npm start
```

The bridge will start on `http://localhost:3000` by default.

### 4. Access the Dashboard

Open your web browser and navigate to:
- **Dashboard**: http://localhost:3000
- **API Documentation**: http://localhost:3000/api/config

## API Endpoints

### REST API

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/config` | Get complete configuration |
| GET | `/api/objects/{objectId}` | Get object configuration |
| GET | `/api/objects/{objectId}/instances/{instanceId}` | Get all resources for instance |
| GET | `/api/objects/{objectId}/instances/{instanceId}/resources/{resourceId}` | Get resource value |
| PUT | `/api/objects/{objectId}/instances/{instanceId}/resources/{resourceId}` | Update resource value |
| POST | `/api/objects/{objectId}/instances/{instanceId}/resources/{resourceId}/update` | Force resource update |
| GET | `/api/health` | System health check |

### WebSocket Events

Connect to `ws://localhost:3000` for real-time updates:

```javascript
const ws = new WebSocket('ws://localhost:3000');

// Subscribe to resource updates
ws.send(JSON.stringify({
  type: 'subscribe_resource',
  objectId: '3',
  instanceId: '0', 
  resourceId: '9'
}));

// Get resource value
ws.send(JSON.stringify({
  type: 'get_resource',
  objectId: '3',
  instanceId: '0',
  resourceId: '9'
}));
```

## Usage Examples

### 1. Reading Dynamic System Values

```bash
# Get battery level
curl http://localhost:3000/api/objects/3/instances/0/resources/9

# Get memory usage
curl http://localhost:3000/api/objects/3/instances/0/resources/10
```

### 2. Updating Writable Resources

```bash
# Update device time
curl -X PUT http://localhost:3000/api/objects/3/instances/0/resources/13 \
  -H "Content-Type: application/json" \
  -d '{"value": 1699708800}'

# Update audio level
curl -X PUT http://localhost:3000/api/objects/3339/instances/0/resources/2 \
  -H "Content-Type: application/json" \
  -d '{"value": 0.75}'
```

### 3. Force Resource Updates

```bash
# Trigger immediate update of battery level
curl -X POST http://localhost:3000/api/objects/3/instances/0/resources/9/update
```

## Advanced Configuration

### Periodic Updates

Configure automatic resource updates:

```json
{
  "update_intervals": {
    "3/0/9": 300,    // Battery level every 5 minutes
    "3/0/10": 60,    // Memory free every minute
    "4/0/2": 30      // Signal strength every 30 seconds
  }
}
```

### Notifications

Set up conditional notifications:

```json
{
  "notifications": {
    "3/0/11": {
      "condition": "value_changed",
      "immediate": true
    },
    "4/0/2": {
      "condition": "threshold", 
      "threshold": -70,
      "operator": "less_than"
    }
  }
}
```

### Custom Value Paths

Supported path formats:

- `file:/path/to/file` - Read from file
- `command:shell_command` - Execute shell command
- `system:time|uptime|hostname` - Built-in system values
- `/absolute/path` - Direct file path (default)

## Integration with C++ Client

The Node.js bridge communicates with the C++ LwM2M client through:

1. **JSON Commands** - Sent via stdin/IPC
2. **WppTaskQueue** - For thread-safe resource updates
3. **Object Registry** - Access to LwM2M objects and resources

### Example C++ Integration

```cpp
#include "NodeJsBridge.h"

// In your main.cpp
wpp::NodeJsBridge bridge;
bridge.start();

// Process commands from Node.js
std::string line;
while (std::getline(std::cin, line)) {
    bridge.processJsonCommand(line);
}
```

## Troubleshooting

### Common Issues

1. **Permission Errors**: Ensure Node.js has permission to read system files
2. **Command Failures**: Check that shell commands work in the terminal
3. **Port Conflicts**: Change port in `lwm2m_bridge.js` if 3000 is in use
4. **WebSocket Issues**: Check firewall settings

### Debug Mode

Enable verbose logging:

```bash
DEBUG=* npm start
```

### Health Check

```bash
curl http://localhost:3000/api/health
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.