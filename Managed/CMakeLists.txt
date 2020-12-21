

set(CS_STANDARD netstandard2.1)
set(CONFIGURATION Debug)

add_custom_command(
        OUTPUT bin/${CONFIGURATION}/${CS_STANDARD}/publish/Managed.dll
        COMMAND dotnet publish
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(copy-runtime-files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CONFIGURATION}/${CS_STANDARD}/publish ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        DEPENDS bin/${CONFIGURATION}/${CS_STANDARD}/publish/Managed.dll)

add_custom_target(csharpDLL
        DEPENDS copy-runtime-files)
