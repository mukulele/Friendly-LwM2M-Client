# Bridge Directory - C++ ↔ Node.js Integration

This directory contains C++ components that bridge between the LwM2M client implementation and external interfaces (Node.js, SignalK, etc.).

## 🏗️ Directory Structure

```
bridge/
├── CMakeLists.txt           # Build configuration
├── README.md               # This file
├── include/                # Header files (.h)
│   ├── NodeJsBridge.h     # Node.js bridge interface
│   └── SignalKLwM2MConfig.h # SignalK configuration handler
└── src/                   # Source files (.cpp)
    └── signalk_main.cpp   # SignalK LwM2M client executable
```

## 🎯 Purpose

The bridge directory serves as:

- **🌉 Integration Layer**: Connects C++ LwM2M client with external systems
- **🔧 Interface Abstraction**: Provides clean APIs for different integrations
- **📦 Modular Design**: Separate from core LwM2M implementation (`wpp/`)
- **🚀 Extensible Architecture**: Easy to add new bridge implementations

## 🔧 Components

### Headers (`include/`)

#### `NodeJsBridge.h`
- **Purpose**: Interface for Node.js ↔ C++ communication
- **Features**: Thread-safe message passing, resource management
- **Usage**: Enables TypeScript bridge to control C++ client

#### `SignalKLwM2MConfig.h`
- **Purpose**: SignalK protocol integration and configuration
- **Features**: Marine data protocol handling, dynamic configuration
- **Usage**: Connects LwM2M resources to SignalK data streams

### Source Files (`src/`)

#### `signalk_main.cpp`
- **Purpose**: Executable SignalK-enabled LwM2M client
- **Features**: Complete LwM2M client with SignalK integration
- **Usage**: Can be launched from TypeScript bridge or standalone

## 🔨 Build Targets

The CMakeLists.txt creates:

1. **`lwm2m_bridge`** (Static Library)
   - Reusable bridge components
   - Linkable by other C++ projects
   - Clean separation of concerns

2. **`signalk_lwm2m_client`** (Executable)
   - Ready-to-run SignalK LwM2M client
   - Used by TypeScript bridge server
   - Standalone operation support

## 🚀 Development Guidelines

### Adding New Bridge Components

1. **Create Header File**
   ```cpp
   // include/NewBridge.h
   #ifndef NEW_BRIDGE_H
   #define NEW_BRIDGE_H
   
   #include "WppClient.h"
   
   namespace bridge {
       class NewBridge {
           // Interface definition
       };
   }
   #endif
   ```

2. **Create Source File**
   ```cpp
   // src/NewBridge.cpp
   #include "NewBridge.h"
   
   namespace bridge {
       // Implementation
   }
   ```

3. **Update CMakeLists.txt**
   ```cmake
   # Add to library sources
   add_library(lwm2m_bridge STATIC
       src/signalk_main.cpp
       src/NewBridge.cpp  # Add new source
   )
   ```

### File Organization Rules

- **Headers** → `include/` directory
- **Source Files** → `src/` directory  
- **Executables** → One main file per executable in `src/`
- **Libraries** → Reusable components in both `include/` and `src/`

### Naming Conventions

- **Files**: `PascalCase.h` and `PascalCase.cpp`
- **Classes**: `PascalCase`
- **Namespaces**: `lowercase` or `bridge::`
- **Executables**: `snake_case_executable`

## 🔗 Integration with TypeScript

The bridge components are designed to work seamlessly with the TypeScript Node.js bridge:

```typescript
// TypeScript calls C++ executable
const cppClient = spawn('./build/bridge/signalk_lwm2m_client', args);

// C++ client communicates back via stdout/stdin
cppClient.stdout.on('data', (data) => {
    // Handle LwM2M events in TypeScript
});
```

## 📊 Build Integration

### From Root Directory
```bash
# Build everything including bridge
cmake --build build

# Bridge targets will be in:
# - build/bridge/libwm2m_bridge.a
# - build/bridge/signalk_lwm2m_client
```

### From Bridge Directory
```bash
# Build just bridge components
cd bridge
mkdir build && cd build
cmake ..
make
```

## 🎯 Why This Organization?

| Aspect | Benefit |
|--------|---------|
| **Separation** | Bridge code separate from core LwM2M (`wpp/`) |
| **Modularity** | Each integration has its own components |
| **Reusability** | Static library can be used by multiple projects |
| **Clarity** | Clear purpose and scope for bridge functionality |
| **Extensibility** | Easy to add new bridge types (MQTT, HTTP, etc.) |
| **Build System** | Proper CMake integration with dependencies |

## 🔄 Future Expansion

This structure easily supports additional bridge types:

- **MQTT Bridge** → `MqttBridge.h/cpp`
- **HTTP Bridge** → `HttpBridge.h/cpp`  
- **Database Bridge** → `DatabaseBridge.h/cpp`
- **Custom Protocol** → `CustomBridge.h/cpp`

Each new bridge follows the same pattern and integrates cleanly with the existing build system and TypeScript interface.