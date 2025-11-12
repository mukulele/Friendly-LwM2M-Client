#!/bin/bash#!/bin/bash



# Script to generate and integrate all XML objects from config/*.xml files# Automated LwM2M Object Integration Script

# This script processes all LwM2M object definitions and integrates them into the project# This script processes all XML files in /config and integrates them into the LwM2M client



echo "=== Adding XML Objects to LwM2M Binary ==="set -e  # Exit on any error



# Change to the object_maker directoryecho "🚀 LwM2M Object Integration Script"

cd utils/object_makerecho "================================="



# Find all XML files in the config directory# Change to the object_maker directory

XML_FILES=(../../config/lwm2m-object-*.xml)cd "$(dirname "$0")/utils/object_maker"



echo "Found ${#XML_FILES[@]} XML object files to process:"# Define the config directory

for xml_file in "${XML_FILES[@]}"; doCONFIG_DIR="../../config"

    echo "  - $(basename "$xml_file")"

done# Array of XML files to process (excluding object 3 which already exists)

XML_FILES=(

# Process each XML file    "lwm2m-object-6.xml"

for xml_file in "${XML_FILES[@]}"; do    "lwm2m-object-3201.xml"

    if [ -f "$xml_file" ]; then    "lwm2m-object-3202.xml"

        object_name=$(basename "$xml_file" .xml)    "lwm2m-object-3203.xml"

        echo ""    "lwm2m-object-3300.xml"

        echo "Processing $object_name..."    "lwm2m-object-3303.xml"

            "lwm2m-object-3311.xml"

        # Generate the object    "lwm2m-object-3316.xml"

        echo "  Generating object..."    "lwm2m-object-3317.xml"

        python3 object_maker.py -g file "$xml_file"    "lwm2m-object-3322.xml"

            "lwm2m-object-3323.xml"

        if [ $? -eq 0 ]; then    "lwm2m-object-3328.xml"

            echo "  ✓ Generation successful"    "lwm2m-object-3336.xml"

                "lwm2m-object-3342.xml"

            # Find the generated folder (it should be the newest directory starting with 'o_')    "lwm2m-object-3347.xml"

            generated_folder=$(ls -1t | grep '^o_' | head -n 1))

            

            if [ -n "$generated_folder" ]; then# Object descriptions for progress tracking

                echo "  Integrating $generated_folder..."declare -A OBJECT_NAMES=(

                python3 object_maker.py -i "./$generated_folder"    ["lwm2m-object-6.xml"]="Location"

                    ["lwm2m-object-3201.xml"]="Digital Output"

                if [ $? -eq 0 ]; then    ["lwm2m-object-3202.xml"]="Analog Input"

                    echo "  ✓ Integration successful"    ["lwm2m-object-3203.xml"]="Analog Output"

                else    ["lwm2m-object-3300.xml"]="Generic Sensor"

                    echo "  ✗ Integration failed for $object_name"    ["lwm2m-object-3303.xml"]="Temperature"

                fi    ["lwm2m-object-3311.xml"]="Light Control"

            else    ["lwm2m-object-3316.xml"]="Voltage"

                echo "  ✗ Could not find generated folder for $object_name"    ["lwm2m-object-3317.xml"]="Current"

            fi    ["lwm2m-object-3322.xml"]="Load"

        else    ["lwm2m-object-3323.xml"]="Pressure"

            echo "  ✗ Generation failed for $object_name"    ["lwm2m-object-3328.xml"]="Power"

        fi    ["lwm2m-object-3336.xml"]="Location (IPSO)"

    fi    ["lwm2m-object-3342.xml"]="On/Off Switch"

done    ["lwm2m-object-3347.xml"]="Push Button"

)

echo ""

echo "=== XML Object Integration Complete ==="# Counter for progress

echo ""TOTAL=${#XML_FILES[@]}

echo "Generated objects:"CURRENT=0

ls -1 ../../wpp/registry/objects/ | grep '^o_' | sortSUCCESS=0
FAILED=0

echo "📋 Found $TOTAL XML object definitions to process"
echo ""

# Function to process XML with on-the-fly name fixing
process_xml_with_fixes() {
    local xml_path="$1"
    
    # Check if the XML contains object names with "/" characters
    if grep -q 'Name="[^"]*/' "$xml_path" 2>/dev/null; then
        echo "   🔧 FIXING: Object name contains '/' character, applying on-the-fly fix..."
        
        # Process the file with sed, replacing "/" with "-" in object names
        # Use process substitution to pipe the fixed content directly to object_maker
        python3 object_maker.py -g file -i <(sed 's/Name="\([^"]*\)\/\([^"]*\)"/Name="\1-\2"/g; s/name="\([^"]*\)\/\([^"]*\)"/name="\1-\2"/g' "$xml_path")
    else
        # Process the original file directly
        python3 object_maker.py -g file -i "$xml_path"
    fi
}

# Function to process a single XML file
process_xml_file() {
    local xml_file="$1"
    local xml_path="$CONFIG_DIR/$xml_file"
    local object_name="${OBJECT_NAMES[$xml_file]}"
    
    ((CURRENT++))
    echo "[$CURRENT/$TOTAL] Processing $object_name ($xml_file)..."
    
    # Check if XML file exists
    if [[ ! -f "$xml_path" ]]; then
        echo "   ❌ ERROR: XML file not found: $xml_path"
        ((FAILED++))
        return 1
    fi
    
    # Process XML file with on-the-fly fixes and run object maker
    if process_xml_with_fixes "$xml_path" 2>&1; then
        echo "   ✅ SUCCESS: $object_name integrated successfully"
        ((SUCCESS++))
        return 0
    else
        echo "   ❌ FAILED: Error processing $object_name"
        ((FAILED++))
        return 1
    fi
}

# Process each XML file
echo "🔄 Starting object integration..."
echo ""

for xml_file in "${XML_FILES[@]}"; do
    if ! process_xml_file "$xml_file"; then
        echo "   ⚠️  Continuing with next object..."
    fi
    echo ""
done

# Summary
echo "================================="
echo "📊 Integration Summary:"
echo "   ✅ Successfully integrated: $SUCCESS objects"
echo "   ❌ Failed to integrate: $FAILED objects"
echo "   📁 Total processed: $CURRENT objects"
echo ""

if [[ $SUCCESS -gt 0 ]]; then
    echo "🎯 Next Steps:"
    echo "1. Build the project:"
    echo "   cd ../../build && make -j4"
    echo ""
    echo "2. Enable objects in your configuration:"
    echo "   Add preprocessor definitions like -DOBJ_O_3300_GENERIC_SENSOR"
    echo ""
    echo "3. Test with TypeScript bridge:"
    echo "   npm run cli list"
    echo ""
fi

if [[ $FAILED -gt 0 ]]; then
    echo "⚠️  Some objects failed to integrate."
    echo "   Check the error messages above for details."
    echo "   You may need to fix XML format or dependency issues."
    echo ""
fi

# Create configuration header with all successfully integrated objects
if [[ $SUCCESS -gt 0 ]]; then
    echo "🔧 Creating configuration header..."
    
    CONFIG_HEADER="../../wpp_objects_config.h"
    cat > "$CONFIG_HEADER" << 'EOF'
#ifndef WPP_OBJECTS_CONFIG_H
#define WPP_OBJECTS_CONFIG_H

/*
 * Auto-generated LwM2M Objects Configuration
 * Generated by: add_xml_objects.sh
 * 
 * Uncomment the objects you want to include in your build
 */

// Mandatory Objects (already included)
// #define OBJ_M_0_LWM2M_SECURITY
// #define OBJ_M_1_LWM2M_SERVER  
// #define OBJ_M_3_DEVICE

// Optional Standard Objects
// #define OBJ_O_2_LWM2M_ACCESS_CONTROL
// #define OBJ_O_4_CONNECTIVITY_MONITORING
// #define OBJ_O_5_FIRMWARE_UPDATE

// IPSO Alliance Objects (from XML definitions)
EOF

    # Add IPSO objects to config file
    for xml_file in "${XML_FILES[@]}"; do
        case "$xml_file" in
            "lwm2m-object-6.xml")
                echo "// #define OBJ_O_6_LOCATION" >> "$CONFIG_HEADER"
                ;;
            "lwm2m-object-3201.xml")
                echo "// #define OBJ_O_3201_DIGITAL_OUTPUT" >> "$CONFIG_HEADER"
                ;;
            "lwm2m-object-3202.xml")
                echo "// #define OBJ_O_3202_ANALOG_INPUT" >> "$CONFIG_HEADER"
                ;;
            "lwm2m-object-3203.xml")
                echo "// #define OBJ_O_3203_ANALOG_OUTPUT" >> "$CONFIG_HEADER"
                ;;
            "lwm2m-object-3300.xml")
                echo "// #define OBJ_O_3300_GENERIC_SENSOR" >> "$CONFIG_HEADER"
                ;;
            "lwm2m-object-3303.xml")
                echo "// #define OBJ_O_3303_TEMPERATURE" >> "$CONFIG_HEADER"
                ;;
            "lwm2m-object-3311.xml")
                echo "// #define OBJ_O_3311_LIGHT_CONTROL" >> "$CONFIG_HEADER"
                ;;
            "lwm2m-object-3316.xml")
                echo "// #define OBJ_O_3316_VOLTAGE" >> "$CONFIG_HEADER"
                ;;
            "lwm2m-object-3317.xml")
                echo "// #define OBJ_O_3317_CURRENT" >> "$CONFIG_HEADER"
                ;;
            "lwm2m-object-3322.xml")
                echo "// #define OBJ_O_3322_LOAD" >> "$CONFIG_HEADER"
                ;;
            "lwm2m-object-3323.xml")
                echo "// #define OBJ_O_3323_PRESSURE" >> "$CONFIG_HEADER"
                ;;
            "lwm2m-object-3328.xml")
                echo "// #define OBJ_O_3328_POWER" >> "$CONFIG_HEADER"
                ;;
            "lwm2m-object-3336.xml")
                echo "// #define OBJ_O_3336_LOCATION_IPSO" >> "$CONFIG_HEADER"
                ;;
            "lwm2m-object-3342.xml")
                echo "// #define OBJ_O_3342_ON_OFF_SWITCH" >> "$CONFIG_HEADER"
                ;;
            "lwm2m-object-3347.xml")
                echo "// #define OBJ_O_3347_PUSH_BUTTON" >> "$CONFIG_HEADER"
                ;;
        esac
    done

    cat >> "$CONFIG_HEADER" << 'EOF'

#endif // WPP_OBJECTS_CONFIG_H
EOF

    echo "   📄 Configuration header created: wpp_objects_config.h"
    echo "   Edit this file to enable specific objects in your build"
fi

echo ""
echo "🎉 Object integration process complete!"

exit 0