/*
 *  Copyright (c) Hubert Jarosz. All rights reserved.
 *  Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#if not defined (__unix__) && not defined(__unix) && not defined (unix) \
&& ( (not defined (__APPLE__) || not defined (__MACH__)) )
    #error THIS SOFTWARE IS ONLY FOR UNIX-LIKE SYSTEMS!
#endif

#include "simpleCoreCLRHost.hpp"
//#include "coreclr.hpp"

#include <iostream>
#include <filesystem>
#include <string>
#include <utility>
#include <stdexcept>
#include <thread>

int main(int /*argc*/, char* argv[])
{
    auto currentExePath = std::string(argv[0]);
    auto currentExeDir = std::filesystem::path(currentExePath).parent_path();

    std::string assemblyName = "Managed";
    std::filesystem::path managedAssemblyAbsoluteDir = std::filesystem::current_path() / currentExeDir;
    std::filesystem::path assemblyPath = managedAssemblyAbsoluteDir / (assemblyName + ".dll");
    if(!std::filesystem::exists(assemblyPath))
    {
        throw std::runtime_error("assemblyPath does not exist");
    }


    auto clrFilesAbsolutePath = "/usr/share/dotnet/shared/Microsoft.NETCore.App/5.0.1";
    if(!std::filesystem::exists(clrFilesAbsolutePath))
    {
        throw std::runtime_error("clr not installed at specified path");
    }

    auto entryPointType = "Trivial";

    auto clrHost = CoreCLRHost(
        currentExePath,
        clrFilesAbsolutePath,
        managedAssemblyAbsoluteDir);

    clrHost.invokeDotNetCLR(assemblyName, entryPointType, "HelloWorld");

    
    interop_class tmp = interop_class();
    tmp.question();
    clrHost.invokeDotNetCLRCallback(assemblyName, entryPointType, "runActionMethod", tmp, &interop_class::print);
    clrHost.invokeDotNetCLRCallback(assemblyName, entryPointType, "runActionMethodInt", tmp, &interop_class::set);
    clrHost.invokeDotNetCLRCallback(assemblyName, entryPointType, "runActionMethodIntInt", tmp, &interop_class::set_sum);
    clrHost.invokeDotNetCLRCallback(assemblyName, entryPointType, "runActionMethod", tmp, &interop_class::print);

    //clrHost.invokeDotNetCLR(assemblyName, entryPointType, "HelloGtk");
    
    return 0;
}