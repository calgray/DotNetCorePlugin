/*
 *  Copyright (c) Hubert Jarosz. All rights reserved.
 *  Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#pragma once

#include "coreclr.hpp"
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

    template<typename... Args>
    void InvokeDotNetCLR(
        const std::string& assemblyName,
        const std::string& entryPointType,
        const std::string& entryPointName,
        Args... args)
    {
        void* handle = m_clr->getCSharpFunctionPtr(assemblyName, entryPointType, entryPointName);
        if(handle)
        {
            reinterpret_cast<void(*)(Args...)>(handle)(args...);
        }
    }

    template<typename T, typename... Args>
    void InvokeDotNetCLRCallback(
        const std::string& assemblyName,
        const std::string& entryPointType,
        const std::string& entryPointName,
        T& instance,
        void(T::*method)(Args...))
    {
        void* handle = m_clr->getCSharpFunctionPtr(assemblyName, entryPointType, entryPointName);
        if(handle)
        {   
            reinterpret_cast<void(*)(T&, void(T::*)(Args...))>(handle)(instance, method);
        }
    }

    template<typename T, typename... Args,
        typename = std::enable_if_t<
        std::is_fundamental<T>::value || std::is_same<char*, T>::value>>
    T InvokeDotNetCLRFunc(
        const std::string& assemblyName,
        const std::string& entryPointType,
        const std::string& entryPointName,
        Args... args)
    {
        void* handle = m_clr->getCSharpFunctionPtr(assemblyName, entryPointType, entryPointName);
        if(!handle)
        {
            throw std::runtime_error("invalid handle");
        }

        return reinterpret_cast<T(*)(Args...)>(handle)(args...);
        
    }
};
