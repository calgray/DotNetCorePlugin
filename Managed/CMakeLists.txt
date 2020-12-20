

set(CS_STANDARD netstandard2.1)
set(CONFIGURATION Debug)

add_custom_command(
        OUTPUT bin/Debug/${CS_STANDARD}/Managed.dll
        COMMAND dotnet build
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(
        OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Managed.dll
        MAIN_DEPENDENCY bin/${CONFIGURATION}/${CS_STANDARD}/Managed.dll
        COMMAND cmake -E copy bin/${CONFIGURATION}/${CS_STANDARD}/Managed.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Managed.dll
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(csharpDLL
        DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Managed.dll)
