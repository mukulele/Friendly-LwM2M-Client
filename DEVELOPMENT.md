# TypeScript Development Guide

## 🎯 **Primary Development Focus: TypeScript**

This project now uses **TypeScript as the primary development language** for all Node.js components, providing type safety, better IDE support, and enhanced maintainability.

## 🚀 **Quick Development Workflow**

### Daily Development Commands

```bash
# Start development with auto-reload
npm run dev

# Build TypeScript to JavaScript  
npm run build

# Run CLI commands
npm run cli status
npm run cli start
npm run cli write 3 0 1 "test value"

# Type checking without compilation
npm run lint

# Clean build artifacts
npm run clean
```

### Project Structure for Development

```
src/                          # 🎯 PRIMARY: TypeScript source code
├── types/lwm2m.ts           # Type definitions - START HERE
├── index.ts                 # Main entry point
├── lwm2m-bridge.ts          # Core bridge server
├── lwm2m-cli.ts             # CLI interface  
└── signalk-lwm2m-config-manager.ts

dist/                         # 🔨 Generated JavaScript (auto-built)
legacy/                       # 📦 Legacy JavaScript (reference only)
```

## 💻 **Development Environment Setup**

### Required VS Code Extensions

The project includes `.vscode/settings.json` with optimized TypeScript settings:

- **TypeScript Language Features** (built-in)
- **Error Lens** - Inline error display
- **Auto Import - ES6** - Better import management
- **Prettier** - Code formatting

### Recommended Terminal Workflow

```bash
# Terminal 1: Development server with auto-reload
npm run dev

# Terminal 2: CLI testing
npm run cli <commands>

# Terminal 3: Build/test commands
npm run build
npm run lint
```

## 🎨 **TypeScript Development Patterns**

### 1. Type-First Development

Always define types before implementation:

```typescript
// 1. Define types in src/types/lwm2m.ts
interface NewFeatureConfig {
  enabled: boolean;
  settings: Record<string, any>;
}

// 2. Use types in implementation
class NewFeature {
  constructor(private config: NewFeatureConfig) {}
}
```

### 2. Strict Type Checking

The project uses strict TypeScript settings:

```typescript
// ✅ Good: Explicit types
function processResource(objectId: number, value: string): APIResponse<string> {
  return { success: true, data: value };
}

// ❌ Avoid: Implicit any types
function processResource(objectId, value) {
  return { success: true, data: value };
}
```

### 3. Error Handling Patterns

```typescript
// Use typed error handling
async function safeOperation(): Promise<APIResponse<Data>> {
  try {
    const result = await riskyOperation();
    return { success: true, data: result };
  } catch (error) {
    return { 
      success: false, 
      error: error instanceof Error ? error.message : 'Unknown error' 
    };
  }
}
```

## 🔧 **Adding New Features**

### Step-by-Step Process

1. **Define Types First**
   ```typescript
   // Add to src/types/lwm2m.ts
   export interface NewFeature {
     id: string;
     config: FeatureConfig;
   }
   ```

2. **Implement with Types**
   ```typescript
   // Create src/new-feature.ts
   import { NewFeature, APIResponse } from './types/lwm2m';
   
   export class FeatureManager {
     async createFeature(config: FeatureConfig): Promise<APIResponse<NewFeature>> {
       // Implementation
     }
   }
   ```

3. **Export from Index**
   ```typescript
   // Add to src/index.ts
   export { FeatureManager } from './new-feature';
   export { NewFeature } from './types/lwm2m';
   ```

4. **Build and Test**
   ```bash
   npm run build  # Check compilation
   npm run lint   # Verify types
   ```

### API Endpoint Pattern

```typescript
// Add to lwm2m-bridge.ts
this.app.get('/api/feature/:id', async (req: express.Request, res: express.Response): Promise<void> => {
  try {
    const { id } = req.params;
    const result = await this.featureManager.getFeature(id);
    
    res.json({ 
      success: true, 
      data: result 
    } as APIResponse<NewFeature>);
  } catch (error) {
    res.status(500).json({ 
      success: false, 
      error: (error as Error).message 
    } as APIResponse);
  }
});
```

## 🐛 **Debugging TypeScript**

### VS Code Debugging Setup

The compiled JavaScript includes source maps for debugging:

1. **Set breakpoints** in TypeScript files (`.ts`)
2. **Run npm run build** to ensure latest compilation
3. **Debug compiled JavaScript** with source map support

### Common TypeScript Issues

```typescript
// Issue: Object is possibly 'null'
// ❌ This will cause TypeScript error
const config = getConfig();
const port = config.port; // Error if config might be null

// ✅ Solution: Null checking
const config = getConfig();
const port = config?.port || 3000;

// Issue: Argument of type 'string' is not assignable to parameter of type 'number'
// ❌ This will cause TypeScript error  
writeResource("3", "0", "1", value); // String IDs not allowed

// ✅ Solution: Proper type conversion
writeResource(parseInt("3"), parseInt("0"), parseInt("1"), value);
```

## 📊 **Performance Considerations**

### Build Optimization

```bash
# Development: Fast builds with source maps
npm run dev

# Production: Optimized builds
npm run build

# Type checking only (fastest)
npm run lint
```

### Watch Mode Development

The development workflow uses watch mode for optimal performance:

```bash
npm run dev  # Runs: tsc --watch + nodemon
```

This provides:
- **Incremental compilation** - Only changed files rebuilt
- **Automatic restart** - Server restarts on changes
- **Source maps** - Debugging support
- **Error display** - Real-time type error feedback

## 🔄 **Migration from JavaScript**

When converting JavaScript to TypeScript:

1. **Rename `.js` → `.ts`**
2. **Add type imports**
   ```typescript
   import { LwM2MConfig, APIResponse } from './types/lwm2m';
   ```
3. **Add parameter types**
   ```typescript
   // Before
   function handleRequest(req, res) {
   
   // After  
   function handleRequest(req: express.Request, res: express.Response): void {
   ```
4. **Add return types**
   ```typescript
   async function getData(): Promise<APIResponse<ConfigData>> {
   ```

## 🎯 **Best Practices for This Project**

1. **Always run `npm run lint`** before committing
2. **Use the provided types** in `src/types/lwm2m.ts`
3. **Follow the existing patterns** in the codebase
4. **Add JSDoc comments** for public APIs
5. **Export new types** from the types module
6. **Use strict null checks** - handle undefined/null explicitly
7. **Prefer interfaces over types** for extensibility
8. **Use `APIResponse<T>`** for all API endpoints

## 🚀 **Ready to Develop!**

Your TypeScript development environment is now optimized! Start with:

```bash
npm run dev
```

Then edit files in `src/` and watch the magic happen! ✨