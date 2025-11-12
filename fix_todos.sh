#!/bin/bash

# Script to fix TODO compilation errors while preserving developer intent
# This replaces TODO placeholders with default values and comments out problematic calls

echo "🔧 Fixing TODO compilation errors in generated LwM2M objects..."

# Find all generated object files
OBJECT_FILES=$(find wpp/registry/objects/o_3* -name "*.cpp" 2>/dev/null)

for file in $OBJECT_FILES; do
    echo "Processing: $file"
    
    # Fix different data types with appropriate defaults and preserve TODO comments
    sed -i \
        -e 's/set<BOOL_T>(\s*\/\*\s*TODO\s*\*\/\s*);/set<BOOL_T>(false); \/\/ TODO: Set appropriate boolean value/g' \
        -e 's/set<FLOAT_T>(\s*\/\*\s*TODO\s*\*\/\s*);/set<FLOAT_T>(0.0f); \/\/ TODO: Set appropriate float value/g' \
        -e 's/set<STRING_T>(\s*\/\*\s*TODO\s*\*\/\s*);/set<STRING_T>(""); \/\/ TODO: Set appropriate string value/g' \
        -e 's/set<TIME_T>(\s*\/\*\s*TODO\s*\*\/\s*);/set<TIME_T>(0); \/\/ TODO: Set appropriate time value/g' \
        -e 's/set<INT_T>(\s*\/\*\s*TODO\s*\*\/\s*);/set<INT_T>(0); \/\/ TODO: Set appropriate integer value/g' \
        -e 's/setDataVerifier(\s*\/\*\s*TODO\s*\*\/\s*);/\/\/ setDataVerifier(nullptr); \/\/ TODO: Add data verifier function/g' \
        "$file"
done

# Also handle EXECUTE_T resources which don't need set() calls
for file in $OBJECT_FILES; do
    # Comment out EXECUTE_T set calls as they don't make sense
    sed -i 's/resource(\([^)]*\))->set<EXECUTE_T>(\s*\/\*\s*TODO\s*\*\/\s*);/\/\/ resource(\1) is executable - no default value needed \/\/ TODO: Implement execute handler/g' "$file"
done

echo "✅ Completed fixing TODO compilation errors"
echo "📝 Note: TODO comments preserved - developers should review and set appropriate values"

# Show summary
echo ""
echo "📊 Summary of changes:"
grep -r "// TODO:" wpp/registry/objects/o_3* --include="*.cpp" 2>/dev/null | wc -l || echo "0"
echo " TODO comments preserved for developer attention"