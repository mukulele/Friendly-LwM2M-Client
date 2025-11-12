import * as fs from 'fs';
import { exec } from 'child_process';
import WebSocket from 'ws';
import { 
  LwM2MConfig, 
  SignalKConfig, 
  SignalKDelta, 
  ResourceValue, 
  CommandResult 
} from './types/lwm2m';

/**
 * Enhanced LwM2M Configuration Manager with SignalK Integration
 * Handles JSON configuration parsing, dynamic value resolution, and SignalK data sources
 */
export class SignalKLwM2MConfigManager {
  private configPath: string;
  public config: LwM2MConfig;
  private valueCache: Map<string, any>;
  private signalkWs: WebSocket | null;
  private signalkData: Map<string, any>;
  private reconnectTimer: NodeJS.Timeout | null;

  constructor(configPath: string) {
    this.configPath = configPath;
    this.config = {} as LwM2MConfig;
    this.valueCache = new Map();
    this.signalkWs = null;
    this.signalkData = new Map();
    this.reconnectTimer = null;
    
    this.loadConfig();
    this.initializeSignalK();
  }

  /**
   * Load and parse JSON configuration
   */
  private loadConfig(): void {
    try {
      const configData = fs.readFileSync(this.configPath, 'utf8');
      this.config = JSON.parse(configData) as LwM2MConfig;
      console.log('Configuration loaded successfully');
    } catch (error) {
      console.error('Error loading config:', (error as Error).message);
      throw error;
    }
  }

  /**
   * Initialize SignalK WebSocket connection
   */
  private initializeSignalK(): void {
    if (!this.config.signalk?.enabled) {
      console.log('SignalK integration disabled');
      return;
    }

    const { host, port, protocol } = this.config.signalk;
    const signalkUrl = `${protocol}://${host}:${port}/signalk/v1/stream`;
    
    this.connectToSignalK(signalkUrl);
  }

  /**
   * Connect to SignalK WebSocket stream
   */
  private connectToSignalK(url: string): void {
    try {
      console.log(`Connecting to SignalK at ${url}`);
      this.signalkWs = new WebSocket(url);

      this.signalkWs.on('open', () => {
        console.log('Connected to SignalK server');
        
        // Subscribe to all navigation data
        const subscription = {
          context: 'vessels.self',
          subscribe: [
            {
              path: '*',
              period: 1000,
              format: 'delta',
              policy: 'ideal',
              minPeriod: 200
            }
          ]
        };

        if (this.signalkWs && this.signalkWs.readyState === WebSocket.OPEN) {
          this.signalkWs.send(JSON.stringify(subscription));
          console.log('Subscribed to SignalK data streams');
        }
      });

      this.signalkWs.on('message', (data: WebSocket.Data) => {
        this.handleSignalKMessage(data);
      });

      this.signalkWs.on('close', () => {
        console.log('SignalK connection closed, attempting reconnect...');
        this.scheduleReconnect(url);
      });

      this.signalkWs.on('error', (error: Error) => {
        console.error('SignalK WebSocket error:', error.message);
        this.scheduleReconnect(url);
      });

    } catch (error) {
      console.error('Failed to connect to SignalK:', (error as Error).message);
      this.scheduleReconnect(url);
    }
  }

  /**
   * Handle incoming SignalK messages
   */
  private handleSignalKMessage(data: WebSocket.Data): void {
    try {
      const message = JSON.parse(data.toString()) as SignalKDelta;
      
      if (message.updates) {
        message.updates.forEach(update => {
          update.values.forEach(valueUpdate => {
            const fullPath = `vessels.self.${valueUpdate.path}`;
            this.signalkData.set(fullPath, valueUpdate.value);
            
            // Cache with timestamp for debugging
            this.valueCache.set(`signalk:${valueUpdate.path}`, {
              value: valueUpdate.value,
              timestamp: update.timestamp,
              source: update.source.label
            });
          });
        });
      }
    } catch (error) {
      console.error('Error parsing SignalK message:', (error as Error).message);
    }
  }

  /**
   * Schedule reconnection to SignalK
   */
  private scheduleReconnect(url: string): void {
    if (this.reconnectTimer) {
      clearTimeout(this.reconnectTimer);
    }

    const reconnectInterval = this.config.signalk?.reconnectInterval || 5000;
    
    this.reconnectTimer = setTimeout(() => {
      console.log('Attempting to reconnect to SignalK...');
      this.connectToSignalK(url);
    }, reconnectInterval);
  }

  /**
   * Resolve dynamic value from various sources
   */
  public async resolveValue(value: any): Promise<any> {
    if (typeof value !== 'string') {
      return value;
    }

    // Handle different value source prefixes
    if (value.startsWith('config:')) {
      return this.resolveConfigValue(value);
    } else if (value.startsWith('signalk:')) {
      return this.resolveSignalKValue(value);
    } else if (value.startsWith('command:')) {
      return await this.resolveCommandValue(value);
    } else if (value.startsWith('system:')) {
      return this.resolveSystemValue(value);
    } else if (value.startsWith('file:')) {
      return this.resolveFileValue(value);
    }

    return value;
  }

  /**
   * Resolve configuration value references
   */
  private resolveConfigValue(value: string): any {
    const path = value.replace('config:', '');
    const pathParts = path.split('.');
    
    let current: any = this.config;
    for (const part of pathParts) {
      if (current && typeof current === 'object' && part in current) {
        current = current[part];
      } else {
        console.warn(`Config path not found: ${path}`);
        return null;
      }
    }
    
    return current;
  }

  /**
   * Resolve SignalK data path
   */
  private resolveSignalKValue(value: string): any {
    const path = value.replace('signalk:', '');
    const cached = this.valueCache.get(value);
    
    if (cached) {
      return cached.value;
    }

    // Try direct path lookup
    const directValue = this.signalkData.get(`vessels.self.${path}`);
    if (directValue !== undefined) {
      return directValue;
    }

    console.warn(`SignalK path not found: ${path}`);
    return null;
  }

  /**
   * Execute command and return result
   */
  private async resolveCommandValue(value: string): Promise<any> {
    const command = value.replace('command:', '');
    
    return new Promise<any>((resolve, reject) => {
      exec(command, { timeout: 5000 }, (error, stdout, stderr) => {
        if (error) {
          console.error(`Command error: ${error.message}`);
          resolve(null);
          return;
        }
        
        const output = stdout.trim();
        
        // Try to parse as number or boolean
        if (/^\d+(\.\d+)?$/.test(output)) {
          resolve(parseFloat(output));
        } else if (output === 'true' || output === 'false') {
          resolve(output === 'true');
        } else {
          resolve(output);
        }
      });
    });
  }

  /**
   * Resolve system information
   */
  private resolveSystemValue(value: string): any {
    const systemPath = value.replace('system:', '');
    
    switch (systemPath) {
      case 'timestamp':
        return new Date().toISOString();
      case 'hostname':
        return require('os').hostname();
      case 'platform':
        return process.platform;
      case 'uptime':
        return process.uptime();
      case 'memory.used':
        const memUsage = process.memoryUsage();
        return Math.round(memUsage.heapUsed / 1024 / 1024); // MB
      case 'memory.total':
        const memTotal = process.memoryUsage();
        return Math.round(memTotal.heapTotal / 1024 / 1024); // MB
      default:
        console.warn(`Unknown system path: ${systemPath}`);
        return null;
    }
  }

  /**
   * Read value from file
   */
  private resolveFileValue(value: string): any {
    const filePath = value.replace('file:', '');
    
    try {
      const content = fs.readFileSync(filePath, 'utf8').trim();
      
      // Try to parse as JSON first
      try {
        return JSON.parse(content);
      } catch {
        // Try to parse as number
        if (/^\d+(\.\d+)?$/.test(content)) {
          return parseFloat(content);
        }
        // Return as string
        return content;
      }
    } catch (error) {
      console.error(`Error reading file ${filePath}:`, (error as Error).message);
      return null;
    }
  }

  /**
   * Get all resource values for a specific object/instance
   */
  public async getInstanceValues(objectId: string, instanceId: string): Promise<Record<string, any>> {
    const objConfig = this.config.objects[objectId];
    if (!objConfig) {
      throw new Error(`Object ${objectId} not found in configuration`);
    }

    const instance = objConfig.instances[instanceId];
    if (!instance) {
      throw new Error(`Instance ${instanceId} not found in object ${objectId}`);
    }

    const values: Record<string, any> = {};
    
    for (const [resourceId, resourceConfig] of Object.entries(instance.resources)) {
      try {
        values[resourceId] = await this.resolveValue(resourceConfig.value);
      } catch (error) {
        console.error(`Error resolving value for ${objectId}/${instanceId}/${resourceId}:`, (error as Error).message);
        values[resourceId] = null;
      }
    }

    return values;
  }

  /**
   * Get resolved value for specific resource
   */
  public async getResourceValue(objectId: string, instanceId: string, resourceId: string): Promise<any> {
    const objConfig = this.config.objects[objectId];
    if (!objConfig?.instances[instanceId]?.resources[resourceId]) {
      throw new Error(`Resource ${objectId}/${instanceId}/${resourceId} not found`);
    }

    const resourceConfig = objConfig.instances[instanceId].resources[resourceId];
    return await this.resolveValue(resourceConfig.value);
  }

  /**
   * Update resource value in configuration
   */
  public updateResourceValue(objectId: string, instanceId: string, resourceId: string, newValue: any): void {
    const objConfig = this.config.objects[objectId];
    if (!objConfig?.instances[instanceId]?.resources[resourceId]) {
      throw new Error(`Resource ${objectId}/${instanceId}/${resourceId} not found`);
    }

    objConfig.instances[instanceId].resources[resourceId].value = newValue;
    console.log(`Updated resource ${objectId}/${instanceId}/${resourceId} = ${newValue}`);
  }

  /**
   * Save current configuration to file
   */
  public saveConfig(): void {
    try {
      fs.writeFileSync(this.configPath, JSON.stringify(this.config, null, 2));
      console.log('Configuration saved successfully');
    } catch (error) {
      console.error('Error saving configuration:', (error as Error).message);
      throw error;
    }
  }

  /**
   * Get SignalK connection status
   */
  public getSignalKStatus(): { connected: boolean; dataPoints: number } {
    return {
      connected: this.signalkWs?.readyState === WebSocket.OPEN,
      dataPoints: this.signalkData.size
    };
  }

  /**
   * Get cached SignalK data for debugging
   */
  public getSignalKData(): Record<string, any> {
    const result: Record<string, any> = {};
    this.signalkData.forEach((value, key) => {
      result[key] = value;
    });
    return result;
  }

  /**
   * Cleanup resources
   */
  public cleanup(): void {
    if (this.signalkWs) {
      this.signalkWs.close();
    }
    
    if (this.reconnectTimer) {
      clearTimeout(this.reconnectTimer);
    }
    
    this.valueCache.clear();
    this.signalkData.clear();
  }
}