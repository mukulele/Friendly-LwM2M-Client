# Modern CMake for websocket static library
file(GLOB WEBSOCKET_SOURCES *.cpp */*.cpp)
add_library(websocket STATIC ${WEBSOCKET_SOURCES})
target_include_directories(websocket PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/external/nlohmann_json/single_include
	${CMAKE_SOURCE_DIR}/external/ixwebsocket/ixwebsocket
)
