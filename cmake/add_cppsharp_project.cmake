
file(WRITE ${project_file} 
"\
<Project Sdk=\"Microsoft.NET.Sdk\">\n\
  <PropertyGroup>\n\
    <TargetFramework>netstandard2.1</TargetFramework>\n\
    <AllowUnsafeBlocks>true</AllowUnsafeBlocks>\n\
    <Nullable>enable</Nullable>\n\
  </PropertyGroup>\n\
  <ItemGroup>\n\
    <ProjectReference Include=\"../../../ManagedPluginAPI/ManagedPluginAPI.csproj\" />\n\
    <Reference Include=\"CppSharp.Runtime\">\n\
     <HintPath>${cppsharp_bin_dir}/CppSharp.Runtime.dll</HintPath>\n\
    </Reference>\n\
  </ItemGroup>\n\
</Project>\n\
")
