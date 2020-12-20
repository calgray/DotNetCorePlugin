include(${CMAKE_SOURCE_DIR}/cmake/cxx_filesystem.cmake)
check_for_cxx_filesystem_support()

set(TARGET_NAME SimpleCoreCLRHost)

set(CS_STANDARD netstandard2.1)

add_custom_command(
        OUTPUT Managed/bin/Debug/${CS_STANDARD}/Managed.dll
        COMMAND dotnet build
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Managed)

add_custom_command(
        OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Managed.dll
        MAIN_DEPENDENCY Managed/bin/Debug/${CS_STANDARD}/Managed.dll
        COMMAND cmake -E copy Managed/bin/Debug/${CS_STANDARD}/Managed.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Managed.dll
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(csharpDLL
        DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Managed.dll)

add_executable(${TARGET_NAME} main.cpp simpleCoreCLRHost.cpp coreclr.cpp)
add_dependencies(${TARGET_NAME} dynamicLinker)
add_dependencies(${TARGET_NAME} csharpDLL)

# target_include_directories(${TARGET_NAME}
#         SYSTEM
#         PRIVATE ${DYNAMIC_LINKER_HEADERS})

target_link_libraries(${TARGET_NAME}
        dl
        stdc++fs
        dynamicLinker)
