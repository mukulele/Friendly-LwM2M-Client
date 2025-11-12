#!/usr/bin/env bash

# Script to generate and integrate all XML objects from config/*.xml files
# This script processes all LwM2M object definitions and integrates them into the project

echo "=== Adding XML Objects to LwM2M Binary ==="

# Change to the object_maker directory
cd utils/object_maker

# Find all XML files in the config directory
XML_FILES=(../../config/lwm2m-object-*.xml)

echo "Found ${#XML_FILES[@]} XML object files to process:"
for xml_file in "${XML_FILES[@]}"; do
    echo "  - $(basename "$xml_file")"
done

# Process each XML file
for xml_file in "${XML_FILES[@]}"; do
    if [ -f "$xml_file" ]; then
        object_name=$(basename "$xml_file" .xml)
        echo ""
        echo "Processing $object_name..."
        
        # Generate the object
        echo "  Generating object..."
        python3 object_maker.py -g file "$xml_file"
        
        if [ $? -eq 0 ]; then
            echo "  ✓ Generation successful"
            
            # Find the generated folder (it should be the newest directory starting with 'o_')
            generated_folder=$(ls -1t | grep '^o_' | head -n 1)
            
            if [ -n "$generated_folder" ]; then
                echo "  Integrating $generated_folder..."
                python3 object_maker.py -i "./$generated_folder"
                
                if [ $? -eq 0 ]; then
                    echo "  ✓ Integration successful"
                else
                    echo "  ✗ Integration failed for $object_name"
                fi
            else
                echo "  ✗ Could not find generated folder for $object_name"
            fi
        else
            echo "  ✗ Generation failed for $object_name"
        fi
    fi
done

echo ""
echo "=== XML Object Integration Complete ==="
echo ""
echo "Generated objects:"
ls -1 ../../wpp/registry/objects/ | grep '^o_' | sort