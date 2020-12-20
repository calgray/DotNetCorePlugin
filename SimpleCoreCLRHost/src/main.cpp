/*
 *  Copyright (c) Hubert Jarosz. All rights reserved.
 *  Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#if not defined (__unix__) && not defined(__unix) && not defined (unix) \
&& ( (not defined (__APPLE__) || not defined (__MACH__)) )
    #error THIS SOFTWARE IS ONLY FOR UNIX-LIKE SYSTEMS!
#endif


#include "simpleCoreCLRHost.hpp"

#include <iostream>
#include <filesystem>
#include <string>

std::string get_assembly_folder(const char* path)
{
    return std::string(path);
}

std::string get_assembly_name(const char* path)
{
    
    return std::string(path);
}

int main(int argc, char* argv[])
{
    // Check Args
    if (argc != 5)
    {
        std::cout << "Usage : SimpleCoreCLRHost <dotNetPath> <assemblyName> <entryPointType> <entryPointMethod>" << std::endl;
        return 0;
    }

    std::string cwd = std::filesystem::current_path();
    cwd += "/";

    std::string assemblyName(argv[2]);
    std::string assemblyDir(assemblyName);


    if(assemblyName.empty())
    {
        std::cerr << "ERROR: Bad ASSEMBLY_PATH !" << std::endl;
        return 0;
    }

    size_t find = assemblyName.rfind('/');
    if( find == std::string::npos )
    {
        find = 0;
    }

    assemblyName = assemblyName.substr(find+1, assemblyName.size());

    if(assemblyName.size() < 5 ||
        assemblyName.substr( assemblyName.size()-4,
                             assemblyName.size()) != ".dll")
    {
        std::cerr << "ERROR: Assembly is not .dll !" << std::endl;
        return 0;
    }

    assemblyName = assemblyName.substr( 0, assemblyName.size()-4 );

    assemblyDir.erase(find);  // get dir of assembly
    assemblyDir = cwd + assemblyDir;

    auto currentExePath = std::string(argv[0]);
    auto coreClrPath = std::string(argv[1]);
    auto entryPointType = std::string(argv[3]);
    auto entryPointName = std::string(argv[4]);

    runFromEntryPoint(
            currentExePath, // path to this exe
            coreClrPath, // absolute path to coreCLR DLLs
            assemblyDir, // absolute path to DLL to run
            assemblyName,
            entryPointType,
            entryPointName);

    return 0;
}