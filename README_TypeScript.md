# LwM2M Bridge TypeScript Development

This document describes the TypeScript implementation of the LwM2M Bridge system, which provides a Node.js interface for the Friendly LwM2M Client with enhanced type safety and developer experience.

## 🏗️ Project Structure

```
src/
├── types/
│   └── lwm2m.ts                     # Type definitions
├── index.ts                         # Main entry point
├── lwm2m-bridge.ts                  # Main bridge server
├── signalk-lwm2m-config-manager.ts  # Configuration manager
└── lwm2m-cli.ts                     # Command line interface

dist/                                # Compiled JavaScript output
├── types/
├── index.js
├── lwm2m-bridge.js
├── signalk-lwm2m-config-manager.js
└── lwm2m-cli.js
```

## 📦 TypeScript Setup

### Dependencies

- **Runtime**: `express`, `ws`, `axios`, `commander`
- **TypeScript**: `typescript`, `@types/node`, `@types/express`, `@types/ws`

### Configuration

- **tsconfig.json**: TypeScript compiler configuration
- **Target**: ES2020 with CommonJS modules
- **Output**: `dist/` directory with source maps and declarations

## 🚀 Usage

### Development Scripts

```bash
# Build TypeScript to JavaScript
npm run build

# Start the server (TypeScript version)
npm start

# Development with auto-rebuild and restart
npm run dev

# Run CLI (TypeScript version)
npm run cli <command>

# Type checking without compilation
npm run lint

# Clean compiled output
npm run clean
```

### Legacy JavaScript Support

The original JavaScript files are still available:

```bash
# Start JavaScript version
npm run start:js

# Run JavaScript CLI
npm run cli:js

# Development with JavaScript version
npm run dev:js
```

## 🎯 Type Definitions

### Core Types

- **`LwM2MConfig`**: Complete configuration structure
- **`ConnectionConfig`**: LwM2M server connection parameters
- **`SignalKConfig`**: SignalK integration settings
- **`ObjectConfig`**: LwM2M object definitions
- **`ClientStatus`**: Client state management
- **`APIResponse<T>`**: Standardized API responses
- **`WebSocketMessage`**: Real-time communication

### Key Interfaces

```typescript
// Configuration structure
interface LwM2MConfig {
  connection: ConnectionConfig;
  signalk?: SignalKConfig;
  objects: Record<string, ObjectConfig>;
}

// API responses
interface APIResponse<T = any> {
  success: boolean;
  data?: T;
  error?: string;
  message?: string;
}

// Client status
interface ClientInfo {
  status: ClientStatus;
  uptime: number;
  registrationStatus: 'registered' | 'unregistered' | 'bootstrap' | 'error';
  serverConnection: boolean;
  lastUpdate: string;
}
```

## 🔧 Classes

### LwM2MBridge

Main server class providing REST API and WebSocket interface:

```typescript
const bridge = new LwM2MBridge('./signalk_lwm2m_config.json', 3000);
bridge.start();
```

**Key Methods:**
- `start()`: Start HTTP and WebSocket servers
- `stop()`: Graceful shutdown with cleanup
- `startLwM2MClient()`: Launch C++ client process
- `stopLwM2MClient()`: Stop client process
- `writeResourceValue()`: Update LwM2M resources

### SignalKLwM2MConfigManager

Configuration and SignalK integration:

```typescript
const config = new SignalKLwM2MConfigManager('./config.json');
await config.resolveValue('signalk:navigation.position.latitude');
```

**Key Methods:**
- `resolveValue()`: Dynamic value resolution
- `getInstanceValues()`: Get all resource values
- `updateResourceValue()`: Update configuration
- `getSignalKStatus()`: Connection status

### LwM2MClientCLI

Command line interface:

```typescript
const cli = new LwM2MClientCLI('http://localhost:3000');
await cli.startClient();
await cli.writeValue('3', '0', '1', 'New Value');
```

## 🔄 Migration from JavaScript

The TypeScript version maintains full compatibility with the JavaScript API:

### Import Changes

```javascript
// JavaScript
const LwM2MBridge = require('./lwm2m_bridge');

// TypeScript
import { LwM2MBridge } from './lwm2m-bridge';
// or
const { LwM2MBridge } = require('./dist/lwm2m-bridge');
```

### Type Safety Benefits

- **Compile-time error detection**
- **IntelliSense and autocomplete**
- **Refactoring safety**
- **API contract enforcement**
- **Documentation through types**

## 🏃 Getting Started

1. **Install dependencies:**
   ```bash
   npm install
   ```

2. **Build TypeScript:**
   ```bash
   npm run build
   ```

3. **Start the server:**
   ```bash
   npm start
   ```

4. **Use the CLI:**
   ```bash
   npm run cli status
   npm run cli start
   npm run cli write 3 0 1 "Hello World"
   ```

## 🔍 Development

### Type Checking

```bash
# Check types without compilation
npm run lint

# Watch mode for development
npm run dev
```

### Adding New Types

Add type definitions to `src/types/lwm2m.ts`:

```typescript
export interface NewFeature {
  property: string;
  optional?: number;
}
```

### Debugging

- **Source Maps**: Available for debugging compiled JavaScript
- **Type Errors**: Shown during compilation
- **Runtime Errors**: Same as JavaScript with better stack traces

## 📚 API Documentation

The TypeScript version provides the same REST API and WebSocket interface as the JavaScript version:

- **REST API**: `http://localhost:3000/api/`
- **WebSocket**: `ws://localhost:3000`
- **Web Interface**: `http://localhost:3000/`

See `README_NodeJS_Bridge.md` for complete API documentation.

## 🤝 Contributing

1. Use TypeScript for new features
2. Maintain type definitions in `src/types/`
3. Run `npm run lint` before committing
4. Ensure `npm run build` succeeds
5. Update type definitions for API changes

## 🔄 Compatibility

- **Node.js**: >=14.0.0
- **JavaScript**: Fully backward compatible
- **Configuration**: Same JSON format
- **CLI**: Same command interface
- **API**: Same REST endpoints and WebSocket messages