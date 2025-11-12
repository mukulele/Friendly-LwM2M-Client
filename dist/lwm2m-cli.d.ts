#!/usr/bin/env node
/**
 * LwM2M Client Command Line Interface
 * Control the LwM2M client through REST API commands
 */
declare class LwM2MClientCLI {
    private baseUrl;
    constructor(baseUrl?: string);
    private makeRequest;
    /**
     * Start the LwM2M client
     */
    startClient(): Promise<void>;
    /**
     * Stop the LwM2M client
     */
    stopClient(): Promise<void>;
    /**
     * Deregister and stop the LwM2M client
     */
    deregisterClient(): Promise<void>;
    /**
     * Get client status
     */
    getStatus(): Promise<void>;
    /**
     * Register (start) the LwM2M client
     */
    registerClient(): Promise<void>;
    /**
     * Write a value to a specific resource
     */
    writeValue(objectId: string, instanceId: string, resourceId: string, value: string): Promise<void>;
    /**
     * Read a resource value
     */
    readValue(objectId: string, instanceId: string, resourceId: string): Promise<void>;
    /**
     * List all objects in configuration
     */
    listObjects(): Promise<void>;
    /**
     * Get SignalK integration status
     */
    getSignalKStatus(): Promise<void>;
    /**
     * Parse value from string to appropriate type
     */
    private parseValue;
    /**
     * Get status icon for client state
     */
    private getStatusIcon;
    /**
     * Format uptime in human readable format
     */
    private formatUptime;
}
export { LwM2MClientCLI };
export default LwM2MClientCLI;
//# sourceMappingURL=lwm2m-cli.d.ts.map