"use strict";
/**
 * Main entry point for the LwM2M Bridge TypeScript application
 */
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
var __exportStar = (this && this.__exportStar) || function(m, exports) {
    for (var p in m) if (p !== "default" && !Object.prototype.hasOwnProperty.call(exports, p)) __createBinding(exports, m, p);
};
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
Object.defineProperty(exports, "__esModule", { value: true });
exports.LwM2MClientCLI = exports.SignalKLwM2MConfigManager = exports.LwM2MBridge = void 0;
var lwm2m_bridge_1 = require("./lwm2m-bridge");
Object.defineProperty(exports, "LwM2MBridge", { enumerable: true, get: function () { return lwm2m_bridge_1.LwM2MBridge; } });
var signalk_lwm2m_config_manager_1 = require("./signalk-lwm2m-config-manager");
Object.defineProperty(exports, "SignalKLwM2MConfigManager", { enumerable: true, get: function () { return signalk_lwm2m_config_manager_1.SignalKLwM2MConfigManager; } });
var lwm2m_cli_1 = require("./lwm2m-cli");
Object.defineProperty(exports, "LwM2MClientCLI", { enumerable: true, get: function () { return lwm2m_cli_1.LwM2MClientCLI; } });
__exportStar(require("./types/lwm2m"), exports);
// If running directly (not imported)
if (require.main === module) {
    Promise.resolve().then(() => __importStar(require('./lwm2m-bridge'))).then(({ LwM2MBridge }) => {
        // Get configuration file path from command line or use default
        const configPath = process.argv[2] || './signalk_lwm2m_config.json';
        const port = parseInt(process.argv[3]) || 3000;
        console.log(`Starting LwM2M Bridge with config: ${configPath} on port: ${port}`);
        // Create and start the bridge server
        const bridge = new LwM2MBridge(configPath, port);
        bridge.start();
        // Handle graceful shutdown
        process.on('SIGINT', async () => {
            console.log('\\nReceived SIGINT, shutting down gracefully...');
            await bridge.stop();
            process.exit(0);
        });
        process.on('SIGTERM', async () => {
            console.log('\\nReceived SIGTERM, shutting down gracefully...');
            await bridge.stop();
            process.exit(0);
        });
        // Handle uncaught exceptions
        process.on('uncaughtException', async (error) => {
            console.error('Uncaught Exception:', error);
            await bridge.stop();
            process.exit(1);
        });
        process.on('unhandledRejection', async (reason, promise) => {
            console.error('Unhandled Rejection at:', promise, 'reason:', reason);
            await bridge.stop();
            process.exit(1);
        });
    }).catch((error) => {
        console.error('Failed to start LwM2M Bridge:', error);
        process.exit(1);
    });
}
