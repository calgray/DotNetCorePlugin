
set(CS_STANDARD netstandard2.1)
set(CONFIGURATION Release)

add_custom_target(ManagedPlugin ALL
        COMMAND dotnet publish -c ${CONFIGURATION}
        COMMAND ${CMAKE_COMMAND} 
        -Dsrc_dir=./bin/${CONFIGURATION}/${CS_STANDARD}/publish
        -Ddst_dir=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        -P ${CMAKE_SOURCE_DIR}/cmake/update_directory.cmake
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_dependencies(ManagedPlugin ManagedPluginAPI)
