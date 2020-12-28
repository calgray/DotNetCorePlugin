

set(CS_STANDARD netstandard2.1)
set(CONFIGURATION Debug)

add_custom_target(ManagedAPI ALL
        COMMAND dotnet publish
        COMMAND ${CMAKE_COMMAND} -E copy_directory bin/${CONFIGURATION}/${CS_STANDARD}/publish ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
