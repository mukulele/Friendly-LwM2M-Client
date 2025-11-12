#!/bin/bash

# Script to fix TODO compilation errors in generated LwM2M objects
# This script replaces problematic TODO placeholders with compilable defaults

echo "=== Fixing TODO Compilation Errors ==="

# Find all C++ files in the objects directory
find /home/pi/Friendly-LwM2M-Client-1/wpp/registry/objects -name "*.cpp" | while read file; do
    echo "Processing: $file"
    
    # Count TODOs before
    todos_before=$(grep -c "/* TODO */" "$file" 2>/dev/null || echo 0)
    
    if [ "$todos_before" -gt 0 ]; then
        # Create backup
        cp "$file" "$file.bak"
        
        # Fix various TODO patterns with appropriate defaults
        
        # Float/Double set operations - use 0.0f
        sed -i 's/->set<FLOAT_T>( \/\* TODO \*\/ )/->set<FLOAT_T>(0.0f); \/\/ TODO: Set appropriate default value/g' "$file"
        sed -i 's/->set<DOUBLE_T>( \/\* TODO \*\/ )/->set<DOUBLE_T>(0.0); \/\/ TODO: Set appropriate default value/g' "$file"
        
        # Integer set operations - use 0
        sed -i 's/->set<INT_T>( \/\* TODO \*\/ )/->set<INT_T>(0); \/\/ TODO: Set appropriate default value/g' "$file"
        sed -i 's/->set<UINT_T>( \/\* TODO \*\/ )/->set<UINT_T>(0); \/\/ TODO: Set appropriate default value/g' "$file"
        
        # String set operations - use empty string
        sed -i 's/->set<STRING_T>( \/\* TODO \*\/ )/->set<STRING_T>(""); \/\/ TODO: Set appropriate default value/g' "$file"
        
        # Boolean set operations - use false
        sed -i 's/->set<BOOL_T>( \/\* TODO \*\/ )/->set<BOOL_T>(false); \/\/ TODO: Set appropriate default value/g' "$file"
        
        # Time set operations - use 0 (epoch)
        sed -i 's/->set<TIME_T>( \/\* TODO \*\/ )/->set<TIME_T>(0); \/\/ TODO: Set appropriate default value/g' "$file"
        
        # Execute operations - comment out (no sensible default)
        sed -i 's/->set<EXECUTE_T>( \/\* TODO \*\/ )/\/\/ ->set<EXECUTE_T>( \/\* TODO: Implement execute handler \*\/ )/g' "$file"
        
        # Data verifier operations - comment out (optional)
        sed -i 's/->setDataVerifier( \/\* TODO \*\/ )/\/\/ ->setDataVerifier( \/\* TODO: Add data verifier if needed \*\/ )/g' "$file"
        
        # Count TODOs after
        todos_after=$(grep -c "/* TODO */" "$file" 2>/dev/null || echo 0)
        fixed=$((todos_before - todos_after))
        
        echo "  ✓ Fixed $fixed TODO placeholders (kept $todos_after TODO comments)"
    else
        echo "  ✓ No TODO placeholders found"
    fi
done

echo ""
echo "=== TODO Fix Complete ==="
echo ""
echo "Summary of files processed:"
find /home/pi/Friendly-LwM2M-Client-1/wpp/registry/objects -name "*.cpp" -exec basename {} \; | sort