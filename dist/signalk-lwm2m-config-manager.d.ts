import { LwM2MConfig } from './types/lwm2m';
/**
 * Enhanced LwM2M Configuration Manager with SignalK Integration
 * Handles JSON configuration parsing, dynamic value resolution, and SignalK data sources
 */
export declare class SignalKLwM2MConfigManager {
    private configPath;
    config: LwM2MConfig;
    private valueCache;
    private signalkWs;
    private signalkData;
    private reconnectTimer;
    constructor(configPath: string);
    /**
     * Load and parse JSON configuration
     */
    private loadConfig;
    /**
     * Initialize SignalK WebSocket connection
     */
    private initializeSignalK;
    /**
     * Connect to SignalK WebSocket stream
     */
    private connectToSignalK;
    /**
     * Handle incoming SignalK messages
     */
    private handleSignalKMessage;
    /**
     * Schedule reconnection to SignalK
     */
    private scheduleReconnect;
    /**
     * Resolve dynamic value from various sources
     */
    resolveValue(value: any): Promise<any>;
    /**
     * Resolve configuration value references
     */
    private resolveConfigValue;
    /**
     * Resolve SignalK data path
     */
    private resolveSignalKValue;
    /**
     * Execute command and return result
     */
    private resolveCommandValue;
    /**
     * Resolve system information
     */
    private resolveSystemValue;
    /**
     * Read value from file
     */
    private resolveFileValue;
    /**
     * Get all resource values for a specific object/instance
     */
    getInstanceValues(objectId: string, instanceId: string): Promise<Record<string, any>>;
    /**
     * Get resolved value for specific resource
     */
    getResourceValue(objectId: string, instanceId: string, resourceId: string): Promise<any>;
    /**
     * Update resource value in configuration
     */
    updateResourceValue(objectId: string, instanceId: string, resourceId: string, newValue: any): void;
    /**
     * Save current configuration to file
     */
    saveConfig(): void;
    /**
     * Get SignalK connection status
     */
    getSignalKStatus(): {
        connected: boolean;
        dataPoints: number;
    };
    /**
     * Get cached SignalK data for debugging
     */
    getSignalKData(): Record<string, any>;
    /**
     * Cleanup resources
     */
    cleanup(): void;
}
//# sourceMappingURL=signalk-lwm2m-config-manager.d.ts.map