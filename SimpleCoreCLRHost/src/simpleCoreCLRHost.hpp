/*
 *  Copyright (c) Hubert Jarosz. All rights reserved.
 *  Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#pragma once

#include "coreclr.hpp"
#include "interop_class.hpp"
#include <string>
#include <memory>

/**
 * @brief Convenience CLR wrapper for safe invoking of CLR method
 * 
 */
class CoreCLRHost
{
    std::unique_ptr<CoreCLR> m_clr;
    
public:
    CoreCLRHost(
        const std::string& currentExePath,
        const std::string& clrFilesAbsolutePath,
        const std::string& managedAssemblyAbsoluteDir);
    
    ~CoreCLRHost() = default;

    void invokeDotNetCLR(
        const std::string& assemblyName,
        const std::string& entryPointType,
        const std::string& entryPointName);

    void invokeDotNetCLRCallback(
        const std::string& assemblyName,
        const std::string& entryPointType,
        const std::string& entryPointName,
        interop_class& tmp);

    template<typename T>
    void invokeDotNetCLRCallback(
        const std::string& assemblyName,
        const std::string& entryPointType,
        const std::string& entryPointName,
        T& instance,
        void(T::*method)())
    {
        void* handle = m_clr->getCSharpFunctionPtr(assemblyName, entryPointType, entryPointName);
        if(handle)
        {   
            reinterpret_cast<void(*)(T&, void(T::*)())>(handle)(instance, method);
        }
    }

    template<typename T, typename ...Arg>
    void invokeDotNetCLRCallback(
        const std::string& assemblyName,
        const std::string& entryPointType,
        const std::string& entryPointName,
        T& instance,
        void(T::*method)(Arg...))
    {
        void* handle = m_clr->getCSharpFunctionPtr(assemblyName, entryPointType, entryPointName);
        if(handle)
        {   
            reinterpret_cast<void(*)(T&, void(T::*)(Arg...))>(handle)(instance, method);
        }
    }
};


void runFromEntryPoint(
        const std::string& currentExePath,
        const std::string& clrFilesAbsolutePath,
        const std::string& managedAssemblyAbsoluteDir,
        const std::string& assemblyName,
        const std::string& entryPointType,
        const std::string& entryPointName);
