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

std::pair<std::string, std::string> get_assembly(const char* path)
{
    std::string assemblyName = path;
    std::string assemblyDir = assemblyName;

    if(assemblyName.empty())
    {
        std::cerr << "ERROR: Bad ASSEMBLY_PATH !" << std::endl;
        throw std::invalid_argument("path");
    }

    size_t find = assemblyName.rfind('/');
    if(find == std::string::npos)
    {
        find = 0;
    }

    assemblyName = assemblyName.substr(find+1, assemblyName.size());

    if(assemblyName.size() < 5 ||
        assemblyName.substr( assemblyName.size()-4,
                             assemblyName.size()) != ".dll")
    {
        std::cerr << "ERROR: Assembly is not .dll !" << std::endl;
        throw std::invalid_argument("path");
    }

    assemblyName = assemblyName.substr(0, assemblyName.size()-4);

    std::string cwd = std::filesystem::current_path();
    cwd += "/";

    assemblyDir.erase(find);  // get dir of assembly
    assemblyDir = cwd + assemblyDir;

    return std::make_pair(assemblyDir, assemblyName);
}

std::pair<std::string, std::string> get_assembly2(const char* path)
{
    auto assemblyPath = std::filesystem::path(path);
    if(!std::filesystem::exists(assemblyPath))
    {
        throw std::invalid_argument("assemblyPath");
    }
    if(assemblyPath.extension().string() != ".dll")
    {
        throw std::invalid_argument("assemblyPath");
    }

    std::string assemblyName = assemblyPath.filename().replace_extension("").string();
    std::string assemblyDir = std::filesystem::current_path() / assemblyPath.parent_path();

    return std::make_pair(assemblyDir, assemblyName);
}

int main(int argc, char* argv[])
{
    // Check Args
    if (argc != 5)
    {
        std::cout << "Usage : SimpleCoreCLRHost <dotNetPath> <assemblyName> <entryPointType> <entryPointMethod>" << std::endl;
        return 0;
    }

    std::string assemblyName;
    std::string managedAssemblyAbsoluteDir;
    std::tie(managedAssemblyAbsoluteDir, assemblyName) = get_assembly2(argv[2]);

    auto currentExePath = std::string(argv[0]);
    auto clrFilesAbsolutePath = std::string(argv[1]);
    auto entryPointType = std::string(argv[3]);
    auto entryPointName = std::string(argv[4]);

    // std::cout << "currentExePath: " << currentExePath << "\n";
    // std::cout << "clrFilesAbsolutePath: " << clrFilesAbsolutePath << "\n";
    // std::cout << "managedAssemblyAbsoluteDir: " << managedAssemblyAbsoluteDir << "\n";
    // std::cout << "assemblyName: " << assemblyName << "\n";
    // std::cout << "entryPointType: " << entryPointType << "\n";
    // std::cout << "entryPointName : " << entryPointName << "\n";

    auto clrHost = CoreCLRHost(
        currentExePath,
        clrFilesAbsolutePath,
        managedAssemblyAbsoluteDir);

    clrHost.invokeDotNetCLR(assemblyName, entryPointType, "HelloWorld");
    clrHost.invokeDotNetCLR(assemblyName, entryPointType, "HelloGtk");

    //clrHost.invokeDotNetCLRMethodPtr(assemblyName, entryPointType, entryPointName);

    return 0;
}