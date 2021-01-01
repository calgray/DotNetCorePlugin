
set(TARGET_NAME NativeDllPlugin)

add_library(${TARGET_NAME} SHARED
        src/NativeDllPlugin.cpp)

add_dependencies(${TARGET_NAME} SimplePluginFramework)
add_dependencies(${TARGET_NAME} NativePluginAPI)

message(${CMAKE_CURRENT_BINARY_DIR})
message(${CMAKE_CURRENT_SOURCE_DIR})
message(bin/${CONFIGURATION}/${CS_STANDARD})

add_custom_target(NativeDllPluginCopyRuntime ALL
        DEPENDS ${TARGET_NAME}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "lib${TARGET_NAME}.so" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(NativeDllPluginBinding ALL
        DEPENDS ${TARGET_NAME}
        COMMAND ${PROJECT_SOURCE_DIR}/external/CppSharp/bin/Release_x64/CppSharp.CLI
        -iln libNativeDllPlugin.so
        -o NativeDllPlugin.dll
        ${CMAKE_CURRENT_SOURCE_DIR}/src/NativeDllPlugin.cpp
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

set(DOTNET_PROJECT_SRC "xml\
        xml
")

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/gen/NativeDllPlugin.csproj 
[=[
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <TargetFramework>netstandard2.1</TargetFramework>
    <AllowUnsafeBlocks>true</AllowUnsafeBlocks>
    <Nullable>enable</Nullable>
  </PropertyGroup>
  <ItemGroup>
    <Reference Include="CppSharp.Runtime">
     <HintPath>../../../external/CppSharp/bin/Release_x64/CppSharp.Runtime.dll</HintPath>
    </Reference>
  </ItemGroup>
</Project>
]=])


add_custom_target(NativeDllPluginBindingBuild ALL
        DEPENDS NativeDllPluginBinding
        COMMAND dotnet build
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gen)

# add_custom_target(NativeDllPluginBindingBuild ALL
#         DEPENDS NativeDllPluginBinding
#         COMMAND ${CMAKE_COMMAND} -E touch NativeDllPlugin.csproj
#         COMMAND echo ${DOTNET_PROJECT_SRC} > NativeDllPlugin.csproj
#         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gen)

# add_custom_target(NativeDllPluginBindingBuild ALL
#         DEPENDS NativeDllPluginBinding
#         COMMAND dotnet new NativeDllPlugin)