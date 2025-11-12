import express from 'express';
import { spawn, ChildProcess } from 'child_process';
import WebSocket from 'ws';
import * as http from 'http';
import { SignalKLwM2MConfigManager } from './signalk-lwm2m-config-manager';
import { 
  LwM2MConfig, 
  ClientStatus, 
  ClientInfo, 
  APIResponse, 
  ResourceValue, 
  WebSocketMessage, 
  LogEntry 
} from './types/lwm2m';

/**
 * LwM2M Node.js Bridge Server
 * Provides REST API and WebSocket interface for LwM2M resource management
 */
export class LwM2MBridge {
  private configManager: SignalKLwM2MConfigManager;
  private app: express.Application;
  private port: number;
  private server: http.Server | null = null;
  private wss: WebSocket.Server | null = null;
  private cppClient: ChildProcess | null = null;
  private periodicTimers: Map<string, NodeJS.Timeout> = new Map();
  private clientState: ClientStatus = 'stopped';
  private clients: Set<WebSocket> = new Set();
  private startTime: Date | null = null;

  constructor(configPath: string = './signalk_lwm2m_config.json', port: number = 3000) {
    this.configManager = new SignalKLwM2MConfigManager(configPath);
    this.app = express();
    this.port = port;
    
    this.setupMiddleware();
    this.setupRoutes();
    this.setupWebSocket();
    this.startPeriodicUpdates();
  }

  private setupMiddleware(): void {
    this.app.use(express.json());
    this.app.use(express.static('public'));
    
    // CORS middleware
    this.app.use((req: express.Request, res: express.Response, next: express.NextFunction) => {
      res.header('Access-Control-Allow-Origin', '*');
      res.header('Access-Control-Allow-Methods', 'GET, POST, PUT, DELETE');
      res.header('Access-Control-Allow-Headers', 'Content-Type');
      next();
    });
  }

  private setupRoutes(): void {
    // Get all objects configuration
    this.app.get('/api/config', (req: express.Request, res: express.Response) => {
      res.json(this.configManager.config);
    });

    // Get specific object
    this.app.get('/api/objects/:objectId', (req: express.Request, res: express.Response): void => {
      const { objectId } = req.params;
      const objConfig = this.configManager.config.objects[objectId];
      
      if (!objConfig) {
        res.status(404).json({ error: 'Object not found' } as APIResponse);
        return;
      }

      res.json({ success: true, data: objConfig } as APIResponse);
    });

    // Get specific instance
    this.app.get('/api/objects/:objectId/instances/:instanceId', (req: express.Request, res: express.Response): void => {
      const { objectId, instanceId } = req.params;
      const objConfig = this.configManager.config.objects[objectId];
      
      if (!objConfig?.instances[instanceId]) {
        res.status(404).json({ error: 'Instance not found' } as APIResponse);
        return;
      }

      res.json({ success: true, data: objConfig.instances[instanceId] } as APIResponse);
    });

    // Get instance values (resolved)
    this.app.get('/api/objects/:objectId/instances/:instanceId/values', async (req: express.Request, res: express.Response) => {
      try {
        const { objectId, instanceId } = req.params;
        const values = await this.configManager.getInstanceValues(objectId, instanceId);
        res.json({ success: true, data: values } as APIResponse);
      } catch (error) {
        res.status(404).json({ 
          success: false, 
          error: (error as Error).message 
        } as APIResponse);
      }
    });

    // Get specific resource value
    this.app.get('/api/objects/:objectId/instances/:instanceId/resources/:resourceId', async (req: express.Request, res: express.Response) => {
      try {
        const { objectId, instanceId, resourceId } = req.params;
        const value = await this.configManager.getResourceValue(objectId, instanceId, resourceId);
        res.json({ success: true, data: { value } } as APIResponse);
      } catch (error) {
        res.status(404).json({ 
          success: false, 
          error: (error as Error).message 
        } as APIResponse);
      }
    });

    // Write resource value
    this.app.post('/api/write/:objectId/:instanceId/:resourceId', (req: express.Request, res: express.Response): void => {
      const { objectId, instanceId, resourceId } = req.params;
      const { value } = req.body;

      if (value === undefined) {
        res.status(400).json({ 
          success: false, 
          error: 'Value is required' 
        } as APIResponse);
        return;
      }

      try {
        this.writeResourceValue(parseInt(objectId), parseInt(instanceId), parseInt(resourceId), value);
        
        const resourceValue: ResourceValue = {
          objectId: parseInt(objectId),
          instanceId: parseInt(instanceId),
          resourceId: parseInt(resourceId),
          value,
          timestamp: new Date().toISOString(),
          type: 'String' // This should be determined from config
        };

        this.broadcastToWebSocketClients({
          type: 'resource_update',
          data: resourceValue,
          timestamp: new Date().toISOString()
        });

        res.json({ 
          success: true, 
          message: `Resource ${objectId}/${instanceId}/${resourceId} updated successfully` 
        } as APIResponse);
      } catch (error) {
        res.status(500).json({ 
          success: false, 
          error: (error as Error).message 
        } as APIResponse);
      }
    });

    // Client control endpoints
    this.app.post('/api/client/start', async (req: express.Request, res: express.Response) => {
      try {
        await this.startLwM2MClient();
        res.json({ 
          success: true, 
          message: 'LwM2M client started successfully' 
        } as APIResponse);
      } catch (error) {
        res.status(500).json({ 
          success: false, 
          error: (error as Error).message 
        } as APIResponse);
      }
    });

    this.app.post('/api/client/stop', async (req: express.Request, res: express.Response) => {
      try {
        await this.stopLwM2MClient();
        res.json({ 
          success: true, 
          message: 'LwM2M client stopped successfully' 
        } as APIResponse);
      } catch (error) {
        res.status(500).json({ 
          success: false, 
          error: (error as Error).message 
        } as APIResponse);
      }
    });

    this.app.post('/api/client/deregister', async (req: express.Request, res: express.Response) => {
      try {
        await this.deregisterLwM2MClient();
        res.json({ 
          success: true, 
          message: 'LwM2M client deregistered successfully' 
        } as APIResponse);
      } catch (error) {
        res.status(500).json({ 
          success: false, 
          error: (error as Error).message 
        } as APIResponse);
      }
    });

    this.app.get('/api/client/status', (req: express.Request, res: express.Response) => {
      const clientInfo: ClientInfo = {
        status: this.clientState,
        uptime: this.startTime ? Date.now() - this.startTime.getTime() : 0,
        registrationStatus: this.clientState === 'running' ? 'registered' : 'unregistered',
        serverConnection: this.clientState === 'running',
        lastUpdate: new Date().toISOString()
      };

      res.json({ success: true, data: clientInfo } as APIResponse);
    });

    // SignalK status endpoint
    this.app.get('/api/signalk/status', (req: express.Request, res: express.Response) => {
      const status = this.configManager.getSignalKStatus();
      res.json({ success: true, data: status } as APIResponse);
    });

    this.app.get('/api/signalk/data', (req: express.Request, res: express.Response) => {
      const data = this.configManager.getSignalKData();
      res.json({ success: true, data } as APIResponse);
    });
  }

  private setupWebSocket(): void {
    // WebSocket server will be created when HTTP server starts
  }

  private async startLwM2MClient(): Promise<void> {
    if (this.clientState === 'running' || this.clientState === 'starting') {
      throw new Error('LwM2M client is already running or starting');
    }

    this.clientState = 'starting';
    this.broadcastStatusUpdate();

    try {
      // Build the client executable
      const buildResult = await this.buildClient();
      if (!buildResult.success) {
        throw new Error(`Build failed: ${buildResult.error}`);
      }

      // Start the client process
      const config = this.configManager.config.connection;
      const args = [
        '-s', config.serverHost,
        '-p', config.serverPort.toString(),
        '-n', config.clientName,
        '-e', config.endpointName
      ];

      if (config.lifetime) {
        args.push('-t', config.lifetime.toString());
      }

      this.cppClient = spawn('./build/examples/signalk_lwm2m_client', args, {
        cwd: process.cwd(),
        stdio: ['pipe', 'pipe', 'pipe']
      });

      this.cppClient.stdout?.on('data', (data: Buffer) => {
        const output = data.toString();
        console.log('LwM2M Client:', output);
        
        const logEntry: LogEntry = {
          level: 'info',
          message: output.trim(),
          timestamp: new Date().toISOString(),
          source: 'lwm2m_client'
        };

        this.broadcastToWebSocketClients({
          type: 'log',
          data: logEntry,
          timestamp: new Date().toISOString()
        });
      });

      this.cppClient.stderr?.on('data', (data: Buffer) => {
        const output = data.toString();
        console.error('LwM2M Client Error:', output);
        
        const logEntry: LogEntry = {
          level: 'error',
          message: output.trim(),
          timestamp: new Date().toISOString(),
          source: 'lwm2m_client'
        };

        this.broadcastToWebSocketClients({
          type: 'log',
          data: logEntry,
          timestamp: new Date().toISOString()
        });
      });

      this.cppClient.on('close', (code: number | null) => {
        console.log(`LwM2M client exited with code ${code}`);
        this.clientState = 'stopped';
        this.cppClient = null;
        this.startTime = null;
        this.broadcastStatusUpdate();
      });

      this.cppClient.on('error', (error: Error) => {
        console.error('LwM2M client process error:', error);
        this.clientState = 'error';
        this.broadcastStatusUpdate();
      });

      // Wait a moment for the process to start
      await new Promise(resolve => setTimeout(resolve, 2000));
      
      this.clientState = 'running';
      this.startTime = new Date();
      this.broadcastStatusUpdate();

    } catch (error) {
      this.clientState = 'error';
      this.broadcastStatusUpdate();
      throw error;
    }
  }

  private async stopLwM2MClient(): Promise<void> {
    if (this.clientState === 'stopped' || this.clientState === 'stopping') {
      return;
    }

    this.clientState = 'stopping';
    this.broadcastStatusUpdate();

    if (this.cppClient) {
      this.cppClient.kill('SIGTERM');
      
      // Wait for graceful shutdown
      await new Promise<void>((resolve) => {
        const timeout = setTimeout(() => {
          if (this.cppClient) {
            this.cppClient.kill('SIGKILL');
          }
          resolve();
        }, 5000);

        if (this.cppClient) {
          this.cppClient.on('close', () => {
            clearTimeout(timeout);
            resolve();
          });
        } else {
          resolve();
        }
      });
    }

    this.clientState = 'stopped';
    this.startTime = null;
    this.broadcastStatusUpdate();
  }

  private async deregisterLwM2MClient(): Promise<void> {
    if (this.clientState !== 'running') {
      throw new Error('Client is not running');
    }

    // Send deregister command to client
    if (this.cppClient && this.cppClient.stdin) {
      this.cppClient.stdin.write('d\n');
    }

    await this.stopLwM2MClient();
  }

  private async buildClient(): Promise<{ success: boolean; error?: string }> {
    return new Promise((resolve) => {
      const { spawn } = require('child_process');
      const buildProcess = spawn('cmake', ['--build', 'build'], {
        cwd: process.cwd(),
        stdio: 'pipe'
      });

      let errorOutput = '';

      buildProcess.stderr?.on('data', (data: Buffer) => {
        errorOutput += data.toString();
      });

      buildProcess.on('close', (code: number | null) => {
        if (code === 0) {
          resolve({ success: true });
        } else {
          resolve({ success: false, error: errorOutput });
        }
      });

      buildProcess.on('error', (error: Error) => {
        resolve({ success: false, error: error.message });
      });
    });
  }

  private writeResourceValue(objectId: number, instanceId: number, resourceId: number, value: any): void {
    console.log(`Writing value to ${objectId}/${instanceId}/${resourceId}: ${value}`);
    
    // Update the configuration
    this.configManager.updateResourceValue(objectId.toString(), instanceId.toString(), resourceId.toString(), value);
    
    // If client is running, send write command
    if (this.cppClient && this.cppClient.stdin && this.clientState === 'running') {
      const writeCommand = `w ${objectId} ${instanceId} ${resourceId} ${value}\n`;
      this.cppClient.stdin.write(writeCommand);
      console.log(`Sent write command: ${writeCommand.trim()}`);
    }
  }

  private startPeriodicUpdates(): void {
    // Clear existing timers
    this.periodicTimers.forEach(timer => clearInterval(timer));
    this.periodicTimers.clear();

    Object.entries(this.configManager.config.objects).forEach(([objectId, objConfig]) => {
      if (objConfig.updateInterval && objConfig.updateInterval > 0) {
        const timer = setInterval(async () => {
          try {
            await this.updateObjectInstances(objectId);
          } catch (error) {
            console.error(`Error updating object ${objectId}:`, (error as Error).message);
          }
        }, objConfig.updateInterval);
        
        this.periodicTimers.set(objectId, timer);
      }
    });
  }

  private async updateObjectInstances(objectId: string): Promise<void> {
    const objConfig = this.configManager.config.objects[objectId];
    if (!objConfig) return;

    for (const instanceId of Object.keys(objConfig.instances)) {
      try {
        const values = await this.configManager.getInstanceValues(objectId, instanceId);
        
        // Broadcast updates to WebSocket clients
        Object.entries(values).forEach(([resourceId, value]) => {
          const resourceValue: ResourceValue = {
            objectId: parseInt(objectId),
            instanceId: parseInt(instanceId),
            resourceId: parseInt(resourceId),
            value,
            timestamp: new Date().toISOString(),
            type: 'String' // Should be determined from config
          };

          this.broadcastToWebSocketClients({
            type: 'resource_update',
            data: resourceValue,
            timestamp: new Date().toISOString()
          });
        });
      } catch (error) {
        console.error(`Error updating instance ${objectId}/${instanceId}:`, (error as Error).message);
      }
    }
  }

  private broadcastStatusUpdate(): void {
    const clientInfo: ClientInfo = {
      status: this.clientState,
      uptime: this.startTime ? Date.now() - this.startTime.getTime() : 0,
      registrationStatus: this.clientState === 'running' ? 'registered' : 'unregistered',
      serverConnection: this.clientState === 'running',
      lastUpdate: new Date().toISOString()
    };

    this.broadcastToWebSocketClients({
      type: 'status',
      data: clientInfo,
      timestamp: new Date().toISOString()
    });
  }

  private broadcastToWebSocketClients(message: WebSocketMessage): void {
    const messageStr = JSON.stringify(message);
    
    this.clients.forEach(ws => {
      if (ws.readyState === WebSocket.OPEN) {
        ws.send(messageStr);
      }
    });
  }

  /**
   * Start the HTTP and WebSocket servers
   */
  public start(): void {
    this.server = this.app.listen(this.port, () => {
      console.log(`LwM2M Bridge Server running on port ${this.port}`);
      console.log(`API endpoints: http://localhost:${this.port}/api/`);
      console.log(`Web interface: http://localhost:${this.port}/`);
      console.log(`WebSocket: ws://localhost:${this.port}`);
    });

    // Create WebSocket server
    this.wss = new WebSocket.Server({ server: this.server });
    
    this.wss.on('connection', (ws: WebSocket) => {
      console.log('WebSocket client connected');
      this.clients.add(ws);

      // Send current status to new client
      this.broadcastStatusUpdate();

      ws.on('message', (message: WebSocket.Data) => {
        try {
          const data = JSON.parse(message.toString()) as { action: string; data?: any };
          this.handleWebSocketMessage(ws, data);
        } catch (error) {
          console.error('Error parsing WebSocket message:', (error as Error).message);
        }
      });

      ws.on('close', () => {
        console.log('WebSocket client disconnected');
        this.clients.delete(ws);
      });

      ws.on('error', (error: Error) => {
        console.error('WebSocket error:', error.message);
        this.clients.delete(ws);
      });
    });
  }

  private async handleWebSocketMessage(ws: WebSocket, data: { action: string; data?: any }): Promise<void> {
    try {
      switch (data.action) {
        case 'start_client':
          await this.startLwM2MClient();
          break;
        case 'stop_client':
          await this.stopLwM2MClient();
          break;
        case 'deregister_client':
          await this.deregisterLwM2MClient();
          break;
        case 'get_status':
          this.broadcastStatusUpdate();
          break;
        default:
          console.log('Unknown WebSocket action:', data.action);
      }
    } catch (error) {
      const errorMessage: WebSocketMessage = {
        type: 'error',
        data: { message: (error as Error).message },
        timestamp: new Date().toISOString()
      };
      
      if (ws.readyState === WebSocket.OPEN) {
        ws.send(JSON.stringify(errorMessage));
      }
    }
  }

  /**
   * Stop the server and cleanup resources
   */
  public async stop(): Promise<void> {
    console.log('Stopping LwM2M Bridge Server...');

    // Stop LwM2M client
    await this.stopLwM2MClient();

    // Clear periodic timers
    this.periodicTimers.forEach(timer => clearInterval(timer));
    this.periodicTimers.clear();

    // Close WebSocket connections
    this.clients.forEach(ws => ws.close());
    this.clients.clear();

    // Close WebSocket server
    if (this.wss) {
      this.wss.close();
    }

    // Close HTTP server
    if (this.server) {
      await new Promise<void>((resolve) => {
        this.server!.close(() => resolve());
      });
    }

    // Cleanup configuration manager
    this.configManager.cleanup();

    console.log('LwM2M Bridge Server stopped');
  }
}

// Export for CLI usage
export default LwM2MBridge;