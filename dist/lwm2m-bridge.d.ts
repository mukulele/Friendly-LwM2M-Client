/**
 * LwM2M Node.js Bridge Server
 * Provides REST API and WebSocket interface for LwM2M resource management
 */
export declare class LwM2MBridge {
    private configManager;
    private app;
    private port;
    private server;
    private wss;
    private cppClient;
    private periodicTimers;
    private clientState;
    private clients;
    private startTime;
    constructor(configPath?: string, port?: number);
    private setupMiddleware;
    private setupRoutes;
    private setupWebSocket;
    private startLwM2MClient;
    private stopLwM2MClient;
    private deregisterLwM2MClient;
    private buildClient;
    private writeResourceValue;
    private startPeriodicUpdates;
    private updateObjectInstances;
    private broadcastStatusUpdate;
    private broadcastToWebSocketClients;
    /**
     * Start the HTTP and WebSocket servers
     */
    start(): void;
    private handleWebSocketMessage;
    /**
     * Stop the server and cleanup resources
     */
    stop(): Promise<void>;
}
export default LwM2MBridge;
//# sourceMappingURL=lwm2m-bridge.d.ts.map