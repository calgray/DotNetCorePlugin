

set(CS_STANDARD netstandard2.1)
set(CONFIGURATION Debug)

add_custom_command(
        OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Managed.dll
        BYPRODUCTS OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/publish/Managed.dll
        COMMAND dotnet publish && cmake -E copy bin/${CONFIGURATION}/${CS_STANDARD}/publish/Managed.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Managed.dll
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(csharpDLL
        DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Managed.dll)
