
set(CS_STANDARD netstandard2.1)
set(CONFIGURATION Debug)

# add_custom_target(ManagedPlugin ALL
#         COMMAND dotnet publish
#         COMMAND ${CMAKE_COMMAND} -E copy_directory bin/${CONFIGURATION}/${CS_STANDARD}/publish ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(ManagedPlugin ALL
        COMMAND dotnet publish
        COMMAND ${CMAKE_COMMAND} 
        -Dsrc_dir=${CMAKE_CURRENT_SOURCE_DIR}/bin/${CONFIGURATION}/${CS_STANDARD}/publish
        -Ddst_dir=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        -P ${CMAKE_SOURCE_DIR}/cmake/update_directory.cmake
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_dependencies(ManagedPlugin Managed)
add_dependencies(ManagedPlugin ManagedPluginAPI)