/*
 *  Copyright (c) Hubert Jarosz. All rights reserved.
 *  Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#if not defined (__unix__) && not defined(__unix) && not defined (unix) && ( (not defined (__APPLE__) || not defined (__MACH__)) )
    #error THIS SOFTWARE IS ONLY FOR UNIX-LIKE SYSTEMS!
#endif


#include <iostream>
#include <filesystem>
#include <string>

#include "simpleCoreCLRHost.hpp"


int main( int argc, char* argv[] )
{

    if ( argc != 5 ) {
        std::cout << "READ README.md !" << std::endl;
        return 0;
    }

    std::string cwd = std::filesystem::current_path();
    cwd += "/";

    std::string assemblyName(argv[2]);
    std::string assemblyDir(assemblyName);


    if( assemblyName.empty() ) {
        std::cerr << "ERROR: Bad ASSEMBLY_PATH !" << std::endl;
        return 0;
    }

    size_t find = assemblyName.rfind('/');
    if( find == std::string::npos )
        find = 0;

    assemblyName = assemblyName.substr( find+1, assemblyName.size() );

    if( assemblyName.size() < 5 ||
        assemblyName.substr( assemblyName.size()-4,
                             assemblyName.size()) != ".dll" ) {
        std::cerr << "ERROR: Assembly is not .dll !" << std::endl;
        return 0;
    }

    assemblyName = assemblyName.substr( 0, assemblyName.size()-4 );

    assemblyDir.erase(find);  // get dir of assembly
    assemblyDir = cwd + assemblyDir;


    int exitCode = runFromEntryPoint(
            std::string(argv[0]), // path to this exe
            std::string(argv[1]), // absolute path to coreCLR DLLs
            assemblyDir, // absolute path to DLL to run
            assemblyName,
            std::string(argv[3]),
            std::string(argv[4]));

    if ( exitCode < 0 )
        std::cout << "Exit Code: " << exitCode << std::endl;


    return 0;
}