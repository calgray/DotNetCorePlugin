include(${CMAKE_SOURCE_DIR}/cmake/cxx_filesystem.cmake)
check_for_cxx_filesystem_support()

set(TARGET_NAME SimplePluginFramework)

add_executable(${TARGET_NAME} src/main.cpp)

add_dependencies(${TARGET_NAME} dynamicLinker)
add_dependencies(${TARGET_NAME} CoreCLRHost)

# target_include_directories(${TARGET_NAME}
#         SYSTEM
#         PRIVATE ${DYNAMIC_LINKER_HEADERS})

target_link_libraries(${TARGET_NAME}
    dl
    stdc++fs
    dynamicLinker
    CoreCLRHost
)


# CppSharpConfigure
add_custom_target(CppSharpConfigure #ALL
    COMMAND bash ./build.sh -configuration Release -platform x64
    BYPRODUCTS ${PROJECT_SOURCE_DIR}/external/CppSharp/build/gmake/Makefile
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/CppSharp/build
)

# CppSharp
#TARGET SimplePluginFramework POST_BUILD
#MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/external/CppSharp/build/gmake/Makefile
add_custom_target(CppSharp #ALL
    DEPENDS CppSharpConfigure SimplePluginFramework
    BYPRODUCTS ${PROJECT_SOURCE_DIR}/external/CppSharp/bin/Release_x64/CppSharp.CLI
    COMMAND make -j
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/CppSharp/build/gmake
)
