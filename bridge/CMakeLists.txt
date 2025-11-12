# Bridge Library - Node.js ↔ LwM2M Integration
cmake_minimum_required(VERSION 3.13)

# Bridge library target
add_library(lwm2m_bridge STATIC
    src/signalk_main.cpp
)

# Include directories
target_include_directories(lwm2m_bridge PUBLIC
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/../examples
    ${CMAKE_CURRENT_SOURCE_DIR}/../examples/platform/connection
    ${CMAKE_CURRENT_SOURCE_DIR}/../wpp/registry
    ${CMAKE_CURRENT_SOURCE_DIR}/../wpp/registry/data_model/common
    ${CMAKE_CURRENT_SOURCE_DIR}/../wpp/registry/data_model/object
    ${CMAKE_CURRENT_SOURCE_DIR}/../wpp/registry/data_model/instance
    ${CMAKE_CURRENT_SOURCE_DIR}/../wpp/registry/data_model/resource
    ${CMAKE_CURRENT_SOURCE_DIR}/../wpp/client
    ${CMAKE_CURRENT_SOURCE_DIR}/../wpp/platform
    ${CMAKE_CURRENT_SOURCE_DIR}/../wpp/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/../2305-Wakaama/core
    ${CMAKE_CURRENT_SOURCE_DIR}/../2305-Wakaama/core/er-coap-13
    ${CMAKE_CURRENT_SOURCE_DIR}/../2305-Wakaama/examples/shared
)

# Link with WPP library
target_link_libraries(lwm2m_bridge
    wpp
    pthread
)

# Executable for SignalK LwM2M client
add_executable(signalk_lwm2m_client
    src/signalk_main.cpp
)

target_include_directories(signalk_lwm2m_client PRIVATE
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/../examples
    ${CMAKE_CURRENT_SOURCE_DIR}/../examples/platform/connection
    ${CMAKE_CURRENT_SOURCE_DIR}/../wpp/registry
    ${CMAKE_CURRENT_SOURCE_DIR}/../wpp/registry/data_model/common
    ${CMAKE_CURRENT_SOURCE_DIR}/../wpp/registry/data_model/object
    ${CMAKE_CURRENT_SOURCE_DIR}/../wpp/registry/data_model/instance
    ${CMAKE_CURRENT_SOURCE_DIR}/../wpp/registry/data_model/resource
    ${CMAKE_CURRENT_SOURCE_DIR}/../wpp/client
    ${CMAKE_CURRENT_SOURCE_DIR}/../wpp/platform
    ${CMAKE_CURRENT_SOURCE_DIR}/../wpp/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/../2305-Wakaama/core
    ${CMAKE_CURRENT_SOURCE_DIR}/../2305-Wakaama/core/er-coap-13
    ${CMAKE_CURRENT_SOURCE_DIR}/../2305-Wakaama/examples/shared
)

target_link_libraries(signalk_lwm2m_client
    wpp
    pthread
)

# Compiler flags
set_target_properties(lwm2m_bridge PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(signalk_lwm2m_client PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)