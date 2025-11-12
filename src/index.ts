/**
 * Main entry point for the LwM2M Bridge TypeScript application
 */

export { LwM2MBridge } from './lwm2m-bridge';
export { SignalKLwM2MConfigManager } from './signalk-lwm2m-config-manager';
export { LwM2MClientCLI } from './lwm2m-cli';
export * from './types/lwm2m';

// If running directly (not imported)
if (require.main === module) {
  import('./lwm2m-bridge').then(({ LwM2MBridge }) => {
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