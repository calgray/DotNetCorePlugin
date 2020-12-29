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
        CoreCLRHost)
