/**
 * Type definitions for LwM2M Bridge System
 */

export interface LwM2MConfig {
  connection: ConnectionConfig;
  signalk?: SignalKConfig;
  objects: Record<string, ObjectConfig>;
}

export interface ConnectionConfig {
  serverHost: string;
  serverPort: number;
  serverPortSecure?: number;
  clientName: string;
  endpointName: string;
  dtls?: DTLSConfig;
  lifetime?: number;
  bootstrapServer?: BootstrapConfig;
}

export interface DTLSConfig {
  enabled: boolean;
  pskIdentity?: string;
  pskKey?: string;
}

export interface BootstrapConfig {
  enabled: boolean;
  serverHost?: string;
  serverPort?: number;
}

export interface SignalKConfig {
  enabled: boolean;
  host: string;
  port: number;
  protocol: 'ws' | 'wss';
  reconnectInterval: number;
  timeout: number;
}

export interface ObjectConfig {
  description: string;
  instances: Record<string, InstanceConfig>;
  updateInterval?: number;
}

export interface InstanceConfig {
  description: string;
  resources: Record<string, ResourceConfig>;
}

export interface ResourceConfig {
  name: string;
  type: ResourceType;
  access: AccessType;
  value: any;
  description: string;
  units?: string;
  range?: string;
}

export type ResourceType = 'String' | 'Integer' | 'Float' | 'Boolean' | 'Opaque' | 'Time' | 'Objlnk';
export type AccessType = 'R' | 'W' | 'RW' | 'E';

export interface ClientInfo {
  status: ClientStatus;
  uptime: number;
  registrationStatus: 'registered' | 'unregistered' | 'bootstrap' | 'error';
  serverConnection: boolean;
  lastUpdate: string;
}

export type ClientStatus = 'stopped' | 'starting' | 'running' | 'stopping' | 'error';

export interface APIResponse<T = any> {
  success: boolean;
  data?: T;
  error?: string;
  message?: string;
}

export interface ResourceValue {
  objectId: number;
  instanceId: number;
  resourceId: number;
  value: any;
  timestamp: string;
  type: ResourceType;
}

export interface WebSocketMessage {
  type: 'status' | 'resource_update' | 'log' | 'client_control' | 'error';
  data: any;
  timestamp: string;
}

export interface SignalKDelta {
  context: string;
  updates: Array<{
    source: {
      label: string;
      type: string;
    };
    timestamp: string;
    values: Array<{
      path: string;
      value: any;
    }>;
  }>;
}

export interface LogEntry {
  level: 'debug' | 'info' | 'warn' | 'error';
  message: string;
  timestamp: string;
  source?: string;
}

export interface PeriodicUpdateConfig {
  enabled: boolean;
  interval: number;
  objects: string[];
}

export interface CommandResult {
  success: boolean;
  output: string;
  error?: string;
  exitCode: number;
}