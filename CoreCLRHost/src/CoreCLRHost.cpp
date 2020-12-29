/*
 *  Copyright (c) Hubert Jarosz. All rights reserved.
 *  Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include "CoreCLRHost.hpp"
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

