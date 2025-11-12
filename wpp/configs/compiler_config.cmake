# Setup compiler
# This function sets up the compiler for the target
# Parameters:
#   TARGET: The target to setup the compiler for
#   WITH_EXCEPTIONS: Enable exceptions
#   WITH_RTTI: Enable RTTI
#   FOR_64_BIT: Build for 64 bit system
function(wpp_setup_compiler TARGET WITH_EXCEPTIONS WITH_RTTI FOR_64_BIT INDEPENDENT_CODE)
    # Optimization comiler options
    target_compile_options(${TARGET} PRIVATE
        -ffunction-sections
        -fdata-sections
        -flto
    )
    target_link_options(${TARGET} PRIVATE
        -Wl,--gc-sections
        -flto
        -Wl,-Map=output.map
    )
    # Main comiler options
    target_compile_options(${TARGET} PRIVATE
        -Waggregate-return
        -Wall
        -Wcast-align
        -Wextra
        -Wfloat-equal
        -Wpointer-arith
        -Wshadow
        # -Wswitch-default  # Disabled due to external libraries (TinyCBOR)
        -Wwrite-strings
        # Unused parameters are common in this ifdef-littered code-base, but of no danger
        -Wno-unused-parameter
        # Too many false positives
        -Wno-uninitialized
        # Allow usage ##__VA_ARGS__ in macros (GCC/Clang only)
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wno-gnu-zero-variadic-macro-arguments>
        # -pedantic  # Disabled due to _Float16 incompatibility in TinyCBOR
        # Turn (most) warnings into errors
        -Werror
        # Disabled because of existing, non-trivially fixable code
        -Wno-error=cast-align
    )

    # Build for system type (skip -m64/-m32 flags on ARM64 systems)
    if (FOR_64_BIT)
        message(STATUS "Building for 64 bit system")
        # Only add -m64 flag on x86_64 systems, not on ARM64
        if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
            target_compile_options(${TARGET} PRIVATE -m64)
            set_target_properties(${TARGET} PROPERTIES CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m64")
        endif()
    else()
        message(STATUS "Building for 32 bit system")
        # Only add -m32 flag on x86 systems, not on ARM
        if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(i386|i686|x86)$")
            target_compile_options(${TARGET} PRIVATE -m32)
            set_target_properties(${TARGET} PROPERTIES CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
        endif()
    endif()
    # Exceptions comiler options (C++ only)
    if (WITH_EXCEPTIONS)
        message(STATUS "Exceptions are supported")
        target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fexceptions>)
    else()
        message(STATUS "Exceptions are not supported")
        target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>)
        target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fno-asynchronous-unwind-tables>)
    endif()
    # RTTI comiler options (C++ only)
    if (WITH_RTTI)
        message(STATUS "RTTI is supported")
        target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-frtti>)
    else()
        message(STATUS "RTTI is not supported")
        target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
    endif()
    # Independent code comiler options
    if (INDEPENDENT_CODE)
        message(STATUS "Independent code is supported")
        set_target_properties(${TARGET} PROPERTIES POSITION_INDEPENDENT_CODE ON)
    else()
        message(STATUS "Independent code is not supported")
        set_target_properties(${TARGET} PROPERTIES POSITION_INDEPENDENT_CODE OFF)
    endif()
endfunction()