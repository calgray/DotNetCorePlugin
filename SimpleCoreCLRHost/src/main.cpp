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


    auto clrHost = CoreCLRHost(
        currentExePath,
        clrFilesAbsolutePath,
        managedAssemblyAbsoluteDir);

    auto entryPointType = "Managed.Trivial";

    // C# Action
    clrHost.InvokeDotNetCLR(assemblyName, entryPointType, "HelloWorld");

    // C++ callback

    bool delegateCallback = false;
    if (delegateCallback)
    {
        interop_class tmp = interop_class();
        tmp.question();
        clrHost.InvokeDotNetCLRCallback(assemblyName, entryPointType, "runActionMethod", tmp, &interop_class::print);
        clrHost.InvokeDotNetCLRCallback(assemblyName, entryPointType, "runActionMethodInt", tmp, &interop_class::set);
        clrHost.InvokeDotNetCLRCallback(assemblyName, entryPointType, "runActionMethodIntInt", tmp, &interop_class::set_sum);
        clrHost.InvokeDotNetCLRCallback(assemblyName, entryPointType, "runActionMethod", tmp, &interop_class::print);
    }

    // C# Func
    bool func = true;
    if(func)
    {
        std::cout << "got pi: "
        << clrHost.InvokeDotNetCLRFunc<double>(assemblyName, entryPointType, "GetPi")
        << std::endl;

        std::cout << "got version: "
        << clrHost.InvokeDotNetCLRFunc<char*>(assemblyName, entryPointType, "GetVersion")
        << std::endl;
    }


    // C# singleton
    bool singleton = false;
    if (singleton)
    {
        clrHost.InvokeDotNetCLR(assemblyName, "Managed.Plugin1Singleton", "RunInstance");
        clrHost.InvokeDotNetCLR(assemblyName, "Managed.Plugin1Singleton", "Dispose");
    }

    //clrHost.InvokeDotNetCLR(assemblyName, entryPointType, "HelloGtk");
    
    return 0;
}