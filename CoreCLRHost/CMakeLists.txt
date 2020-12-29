include(${CMAKE_SOURCE_DIR}/cmake/cxx_filesystem.cmake)
check_for_cxx_filesystem_support()

set(TARGET_NAME CoreCLRHost)

add_library(${TARGET_NAME}
        src/CoreCLRHost.cpp
        src/coreclr.cpp)

add_dependencies(${TARGET_NAME} dynamicLinker)

target_include_directories(${TARGET_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(${TARGET_NAME}
        dl
        stdc++fs
        dynamicLinker)

