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
        
        # Handle Object 3336 naming conflict with on-the-fly XML modification
        if [[ "$object_name" == *"3336"* ]]; then
            echo "  Applying on-the-fly naming fix for Object 3336 (Location -> IPSO3336)..."
            # Create temporary XML file with modified name
            temp_xml_file="/tmp/$(basename "$xml_file")"
            sed 's/<Name>Location<\/Name>/<Name>IPSO3336<\/Name>/g' "$xml_file" > "$temp_xml_file"
            xml_to_process="$temp_xml_file"
        else
            xml_to_process="$xml_file"
        fi
        
        # Generate the object
        echo "  Generating object..."
        python3 object_maker.py -g file "$xml_to_process"
        
        # Clean up temporary file if created
        if [[ "$object_name" == *"3336"* ]]; then
            rm -f "$temp_xml_file"
        fi
        
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