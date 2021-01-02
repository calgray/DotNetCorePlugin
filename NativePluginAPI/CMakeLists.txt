include(${CMAKE_SOURCE_DIR}/cmake/cxx_filesystem.cmake)
check_for_cxx_filesystem_support()

set(TARGET_NAME NativePluginAPI)
set(CS_STANDARD netstandard2.1)
set(CONFIGURATION Release)

set(sources
    src/IPlugin.cpp
)

add_library(${TARGET_NAME} SHARED
        ${sources})

target_include_directories(${TARGET_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)


add_custom_target(${TARGET_NAME}CopyRuntime ALL
        DEPENDS ${TARGET_NAME}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "lib${TARGET_NAME}.so" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

get_target_property(PROJECT_INCLUDE_DIRS ${TARGET_NAME} INCLUDE_DIRECTORIES)
add_custom_target(${TARGET_NAME}BindingConfigure ALL
        DEPENDS ${TARGET_NAME}
        COMMAND ${PROJECT_SOURCE_DIR}/external/CppSharp/bin/Release_x64/CppSharp.CLI
        -a x64
        -iln lib${TARGET_NAME}.so
        -o ${TARGET_NAME}.dll
        -I=${PROJECT_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/${sources}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(${TARGET_NAME}BindingProject ALL
        COMMAND ${CMAKE_COMMAND}
        -Dproject_file=${CMAKE_CURRENT_BINARY_DIR}/gen/${TARGET_NAME}.csproj
        -Dcppsharp_bin_dir=../../../external/CppSharp/bin/Release_x64
        -P ${CMAKE_SOURCE_DIR}/cmake/add_cppsharp_project.cmake
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(${TARGET_NAME}Wrapper ALL
        COMMAND ${CMAKE_COMMAND}
        -Dplugin_name=${TARGET_NAME}
        -P ${CMAKE_SOURCE_DIR}/cmake/generate_plugin_wrapper.cmake
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(${TARGET_NAME}BindingBuild ALL
        DEPENDS ${TARGET_NAME}BindingConfigure ${TARGET_NAME}BindingProject
        COMMAND dotnet publish -c ${CONFIGURATION}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gen
)

add_custom_target(${TARGET_NAME}Binding ALL
        DEPENDS ${TARGET_NAME}BindingBuild
        COMMAND ${CMAKE_COMMAND} 
        -Dsrc_dir=${CMAKE_CURRENT_BINARY_DIR}/gen/bin/${CONFIGURATION}/${CS_STANDARD}/publish
        -Ddst_dir=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        -P ${CMAKE_SOURCE_DIR}/cmake/update_directory.cmake
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gen
)
