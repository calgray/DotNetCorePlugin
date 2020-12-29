include(${CMAKE_SOURCE_DIR}/cmake/cxx_filesystem.cmake)
check_for_cxx_filesystem_support()

set(TARGET_NAME SimpleCoreCLRHost)

add_executable(${TARGET_NAME}
        src/main.cpp
        src/simpleCoreCLRHost.cpp
        src/coreclr.cpp)

add_dependencies(${TARGET_NAME} dynamicLinker)
add_dependencies(${TARGET_NAME} Managed)
add_dependencies(${TARGET_NAME} ManagedPlugin)
add_dependencies(${TARGET_NAME} NativePlugin)
add_dependencies(${TARGET_NAME} NativeDllPlugin)

# target_include_directories(${TARGET_NAME}
#         SYSTEM
#         PRIVATE ${DYNAMIC_LINKER_HEADERS})

target_link_libraries(${TARGET_NAME}
        dl
        stdc++fs
        dynamicLinker)
