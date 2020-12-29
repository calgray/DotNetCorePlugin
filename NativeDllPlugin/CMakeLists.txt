
set(TARGET_NAME NativeDllPlugin)

add_library(${TARGET_NAME} SHARED
        src/NativeDllPlugin.cpp)

message(${CMAKE_CURRENT_BINARY_DIR})
message(${CMAKE_CURRENT_SOURCE_DIR})
message(bin/${CONFIGURATION}/${CS_STANDARD})

add_custom_target(NativeDllPluginCopyRuntime ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "lib${TARGET_NAME}.so" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS ${TARGET_NAME})
