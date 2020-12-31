
set(TARGET_NAME NativeDllPlugin)

add_library(${TARGET_NAME} SHARED
        src/NativeDllPlugin.cpp)

add_dependencies(${TARGET_NAME} SimplePluginFramework)
add_dependencies(${TARGET_NAME} NativePluginAPI)

message(${CMAKE_CURRENT_BINARY_DIR})
message(${CMAKE_CURRENT_SOURCE_DIR})
message(bin/${CONFIGURATION}/${CS_STANDARD})

add_custom_target(NativeDllPluginCopyRuntime ALL
        DEPENDS ${TARGET_NAME}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "lib${TARGET_NAME}.so" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(NativeDllPluginBinding ALL
        DEPENDS ${TARGET_NAME}
        COMMAND ${PROJECT_SOURCE_DIR}/external/CppSharp/bin/Release_x64/CppSharp.CLI
        -iln libNativeDllPlugin.so
        -o NativeDllPlugin.dll
        ${CMAKE_CURRENT_SOURCE_DIR}/src/NativeDllPlugin.cpp
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# add_custom_target(NativeDllPluginBindingBuild ALL
#         DEPENDS NativeDllPluginBinding
#         COMMAND dotnet new NativeDllPlugin)