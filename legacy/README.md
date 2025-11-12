# Legacy JavaScript Files

This directory contains the original JavaScript implementation of the LwM2M Bridge system. These files are preserved for backward compatibility and reference.

## 📁 Files

- **`lwm2m_bridge.js`** - Original main bridge server
- **`lwm2m_config_manager.js`** - Basic configuration manager
- **`lwm2m-cli.js`** - Command line interface
- **`signalk_lwm2m_config_manager.js`** - SignalK integration manager

## 🚀 Usage

### Running Legacy JavaScript Version

```bash
# Start the JavaScript bridge server
npm run start:js

# Run JavaScript CLI
npm run cli:js status

# Development with JavaScript
npm run dev:js
```

### Direct Node.js Execution

```bash
# Run the server directly
node legacy/lwm2m_bridge.js

# Run CLI directly
node legacy/lwm2m-cli.js --help
```

## ⚠️ Deprecation Notice

**The TypeScript version in `/src/` is now the primary implementation.**

The legacy JavaScript files are:
- ✅ **Maintained for compatibility**
- ✅ **Available through npm scripts**
- ⚠️ **Not receiving new features**
- ⚠️ **May be removed in future major versions**

## 🔄 Migration Path

If you're still using the JavaScript version:

1. **Current**: Use npm scripts (`npm run start:js`, `npm run cli:js`)
2. **Recommended**: Migrate to TypeScript version (`npm start`, `npm run cli`)
3. **Future**: TypeScript-only (legacy files may be removed)

## 📚 Documentation

- **Current/TypeScript**: See `README_TypeScript.md`
- **Legacy/JavaScript**: See `README_NodeJS_Bridge.md`

## 🤝 Support

- **TypeScript version**: Full support and active development
- **JavaScript version**: Bug fixes only, no new features