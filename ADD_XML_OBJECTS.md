# Adding LwM2M Objects from XML Definitions

This guide shows how to use the object_maker tool to add the LwM2M objects defined in `/config/*.xml` files to the LwM2M binary.

## 🎯 Overview

The `/config` directory contains XML definitions for various IPSO Alliance and OMA LwM2M objects:

- **Object 3**: Device (already exists)
- **Object 6**: Location  
- **Object 3201**: Digital Output
- **Object 3202**: Analog Input
- **Object 3203**: Analog Output
- **Object 3300**: Generic Sensor
- **Object 3303**: Temperature
- **Object 3311**: Light Control
- **Object 3316**: Voltage
- **Object 3317**: Current
- **Object 3322**: Load
- **Object 3323**: Pressure
- **Object 3328**: Power
- **Object 3336**: Location (IPSO)
- **Object 3342**: On/Off Switch
- **Object 3347**: Push Button

## 🔧 Object Maker Tool Usage

The object_maker tool has two main steps:

### Step 1: Generate Object Code from XML
```bash
cd utils/object_maker
python3 object_maker.py -g file ../../config/lwm2m-object-XXXX.xml
```

### Step 2: Integrate Generated Object into WPP
```bash
python3 object_maker.py -i ./o_XXXX_object_name_vYY
```

### Combined Generation and Integration
```bash
python3 object_maker.py -g file -i ../../config/lwm2m-object-XXXX.xml
```

## 🚀 Complete Process for All Objects

### 1. Navigate to Object Maker Directory
```bash
cd /home/pi/Friendly-LwM2M-Client-1/utils/object_maker
```

### 2. Generate and Integrate Each Object

#### Location (Object 6)
```bash
python3 object_maker.py -g file -i ../../config/lwm2m-object-6.xml
```

#### Digital Output (Object 3201)
```bash
python3 object_maker.py -g file -i ../../config/lwm2m-object-3201.xml
```

#### Analog Input (Object 3202)  
```bash
python3 object_maker.py -g file -i ../../config/lwm2m-object-3202.xml
```

#### Analog Output (Object 3203)
```bash
python3 object_maker.py -g file -i ../../config/lwm2m-object-3203.xml
```

#### Generic Sensor (Object 3300)
```bash
python3 object_maker.py -g file -i ../../config/lwm2m-object-3300.xml
```

#### Temperature (Object 3303)
```bash
python3 object_maker.py -g file -i ../../config/lwm2m-object-3303.xml
```

#### Light Control (Object 3311)
```bash
python3 object_maker.py -g file -i ../../config/lwm2m-object-3311.xml
```

#### Voltage (Object 3316)
```bash
python3 object_maker.py -g file -i ../../config/lwm2m-object-3316.xml
```

#### Current (Object 3317)
```bash
python3 object_maker.py -g file -i ../../config/lwm2m-object-3317.xml
```

#### Load (Object 3322)
```bash
python3 object_maker.py -g file -i ../../config/lwm2m-object-3322.xml
```

#### Pressure (Object 3323)
```bash
python3 object_maker.py -g file -i ../../config/lwm2m-object-3323.xml
```

#### Power (Object 3328)
```bash
python3 object_maker.py -g file -i ../../config/lwm2m-object-3328.xml
```

#### Location IPSO (Object 3336)
```bash
python3 object_maker.py -g file -i ../../config/lwm2m-object-3336.xml
```

#### On/Off Switch (Object 3342)
```bash
python3 object_maker.py -g file -i ../../config/lwm2m-object-3342.xml
```

#### Push Button (Object 3347)
```bash
python3 object_maker.py -g file -i ../../config/lwm2m-object-3347.xml
```

## 📋 What the Object Maker Does

For each XML file, the tool will:

1. **Parse XML Definition** - Extract object structure, resources, and metadata
2. **Generate C++ Code** - Create header and implementation files
3. **Create CMake Config** - Add build configuration
4. **Update Registry** - Add to ObjectID.h and WppRegistry.h
5. **Enable Compilation** - Add preprocessor flags (e.g., `OBJ_O_3300_GENERIC_SENSOR`)

## 🏗️ Generated Structure

After running the tool, each object will have:

```
wpp/registry/objects/o_XXXX_object_name_vYY/
├── CMakeLists.txt
├── ObjectName.h              # Object class definition
├── ObjectName.cpp            # Object implementation  
├── ObjectNameInfo.h          # Object metadata
├── ObjectNameConfig.h        # Configuration flags
└── README.md                 # Generated documentation
```

## 🔧 Build Configuration

The tool automatically updates:

- **`wpp/registry/ObjectID.h`** - Adds object ID definitions
- **`wpp/registry/WppRegistry.h`** - Adds object registry entries
- **`wpp/registry/objects/CMakeLists.txt`** - Adds build targets
- **`wpp/configs/`** - Adds configuration options

## 🚀 Enable Objects in Build

After generation, enable objects in your build by defining preprocessor flags:

### Option 1: CMake Configuration
```cmake
# Add to CMakeLists.txt
add_definitions(
    -DOBJ_O_3300_GENERIC_SENSOR
    -DOBJ_O_3303_TEMPERATURE  
    -DOBJ_O_3311_LIGHT_CONTROL
    # ... other objects
)
```

### Option 2: Build-time Definition
```bash
cmake -DCMAKE_CXX_FLAGS="-DOBJ_O_3300_GENERIC_SENSOR -DOBJ_O_3303_TEMPERATURE" ..
```

### Option 3: Configuration Header
Create a configuration file with all desired objects:
```cpp
// wpp_config.h
#define OBJ_O_6_LOCATION
#define OBJ_O_3201_DIGITAL_OUTPUT
#define OBJ_O_3202_ANALOG_INPUT
#define OBJ_O_3203_ANALOG_OUTPUT
#define OBJ_O_3300_GENERIC_SENSOR
#define OBJ_O_3303_TEMPERATURE
#define OBJ_O_3311_LIGHT_CONTROL
#define OBJ_O_3316_VOLTAGE
#define OBJ_O_3317_CURRENT
#define OBJ_O_3322_LOAD
#define OBJ_O_3323_PRESSURE
#define OBJ_O_3328_POWER
#define OBJ_O_3336_LOCATION_IPSO
#define OBJ_O_3342_ON_OFF_SWITCH
#define OBJ_O_3347_PUSH_BUTTON
```

## 🧪 Testing Generated Objects

After integration, test the objects:

### 1. Build the Project
```bash
cd /home/pi/Friendly-LwM2M-Client-1
mkdir -p build && cd build
cmake ..
make -j4
```

### 2. Check Object Registration
The objects should appear in the client output when running:
```bash
./examples/WppExample
# Look for "Registered object XXXX" messages
```

### 3. Test via TypeScript Bridge
```bash
npm run cli list  # Should show new objects
npm run cli read 3300 0 5700  # Read sensor value
npm run cli write 3311 0 5850 true  # Control light
```

## 🔍 Verification

After adding objects, verify they're properly integrated:

### Check Object Directory
```bash
ls -la wpp/registry/objects/
# Should show new o_XXXX_* directories
```

### Check Registry Headers
```bash
grep -n "OBJ_O_" wpp/registry/ObjectID.h
grep -n "OBJ_O_" wpp/registry/WppRegistry.h
```

### Check Build Configuration
```bash
grep -r "OBJ_O_3300" wpp/registry/objects/
```

## 🎯 Marine/IoT Specific Benefits

These objects are particularly useful for marine and IoT applications:

- **3300 (Generic Sensor)** - Any sensor data
- **3303 (Temperature)** - Engine, cabin, water temperature
- **3316 (Voltage)** - Battery monitoring
- **3317 (Current)** - Power consumption
- **3322 (Load)** - Weight sensors, anchor load
- **3323 (Pressure)** - Hydraulic pressure, tire pressure
- **3328 (Power)** - Power consumption monitoring
- **3336 (Location)** - GPS coordinates
- **3342 (On/Off Switch)** - Equipment control
- **3347 (Push Button)** - Manual controls

The objects will integrate seamlessly with your TypeScript bridge and SignalK system! 🚀