add_custom_command(
        OUTPUT dynamicLinker/libdynamicLinker.a
        COMMAND make CXX="${CMAKE_CXX_COMPILER}" CXXFLAGS="-std=c++${CMAKE_CXX_STANDARD}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(buildDependencies DEPENDS dynamicLinker/libdynamicLinker.a)

set(DYNAMIC_LINKER_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/dynamicLinker/libdynamicLinker.a PARENT_SCOPE) #TODO: maybe there is better way than PARENT_SCOPE?
set(DYNAMIC_LINKER_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/dynamicLinker/ PARENT_SCOPE) #TODO: maybe there is better way than PARENT_SCOPE?
