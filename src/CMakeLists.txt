include(${CMAKE_SOURCE_DIR}/cmake/cxx_filesystem.cmake)
check_for_cxx_filesystem_support()

add_custom_command(
        OUTPUT Managed/bin/Debug/netstandard2.0/Managed.dll
        COMMAND dotnet build
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Managed)

add_custom_command(
        OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Managed.dll
        MAIN_DEPENDENCY Managed/bin/Debug/netstandard2.0/Managed.dll
        COMMAND cmake -E copy Managed/bin/Debug/netstandard2.0/Managed.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Managed.dll
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(csharpDLL
        DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Managed.dll)

add_executable(SimpleCoreCLRHost simpleCoreCLRHost.cpp)
add_dependencies(SimpleCoreCLRHost buildDependencies)
add_dependencies(SimpleCoreCLRHost csharpDLL)

target_include_directories(SimpleCoreCLRHost
        SYSTEM
        PRIVATE ${DYNAMIC_LINKER_HEADERS})

target_link_libraries(SimpleCoreCLRHost
        dl
        stdc++fs
        ${DYNAMIC_LINKER_LIBRARY})
