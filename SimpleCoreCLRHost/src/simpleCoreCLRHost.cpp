/*
 *  Copyright (c) Hubert Jarosz. All rights reserved.
 *  Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include "simpleCoreCLRHost.hpp"
#include "interop_class.hpp"
#include "coreclr.hpp"

#include <dynamicLinker.hpp>
#include <climits>
#include <stdexcept>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#ifdef WIN32
constexpr char coreClrDll[] = "libcoreclr.dll";
#endif
#ifdef __APPLE__
constexpr char coreClrDll[] = "libcoreclr.dylib";
#endif
#ifdef __unix__
constexpr char coreClrDll[] = "libcoreclr.so";
#endif

void runFromEntryPoint(
    const std::string& currentExePath,
    const std::string& clrFilesAbsolutePath,
    const std::string& managedAssemblyAbsoluteDir,
    const std::string& assemblyName,
    const std::string& entryPointType,
    const std::string& entryPointName)
{
    std::string coreClrDllPath = clrFilesAbsolutePath + "/" + coreClrDll;
    if(coreClrDllPath.size() >= PATH_MAX)
    {
        throw std::invalid_argument("Path to libcoreclr.dll too long!");
    }

    CoreCLR clr(coreClrDllPath, managedAssemblyAbsoluteDir, clrFilesAbsolutePath, currentExePath);

    void* handle = clr.getCSharpFunctionPtr(assemblyName, entryPointType, entryPointName);
    if(!handle)
    {
        return;
    }

    myClass tmp = myClass();
    tmp.question();

    /*
    *  If arguments are in in different order then second arg is 0 in C#.
    *  probably something with padding/offset/ptr byte size
    */
    reinterpret_cast<void(*)(myClass&, void (myClass::*)())>(handle)(tmp, &myClass::print);
}

CoreCLRHost::CoreCLRHost(
    const std::string& currentExePath,
    const std::string& clrFilesAbsolutePath,
    const std::string& managedAssemblyAbsoluteDir)
    {
        std::string coreClrDllPath = clrFilesAbsolutePath + "/" + coreClrDll;
        if(coreClrDllPath.size() >= PATH_MAX)
        {
            throw std::invalid_argument("Path to libcoreclr.dll too long!");
        }

        m_clr = std::make_unique<CoreCLR>(coreClrDllPath, managedAssemblyAbsoluteDir, clrFilesAbsolutePath, currentExePath);
    }

void CoreCLRHost::invokeDotNetCLR(
    const std::string& assemblyName,
    const std::string& entryPointType,
    const std::string& entryPointName)
{
    void* handle = m_clr->getCSharpFunctionPtr(assemblyName, entryPointType, entryPointName);
    if(!handle)
    {
        return;
    }

    reinterpret_cast<void(*)()>(handle)();
}

void CoreCLRHost::invokeDotNetCLRMethodPtr(
    const std::string& assemblyName,
    const std::string& entryPointType,
    const std::string& entryPointName)
{
    void* handle = m_clr->getCSharpFunctionPtr(assemblyName, entryPointType, entryPointName);
    if(!handle)
    {
        return;
    }

    myClass tmp = myClass();
    tmp.question();

    /*
    *  If arguments are in in different order then second arg is 0 in C#.
    *  probably something with padding/offset/ptr byte size
    */
    reinterpret_cast<void(*)(myClass&, void (myClass::*)())>(handle)(tmp, &myClass::print);
}
