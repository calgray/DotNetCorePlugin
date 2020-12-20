/*
 *  Copyright (c) Hubert Jarosz. All rights reserved.
 *  Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include "simpleCoreCLRHost.hpp"

#include <dynamicLinker.hpp>
#include <climits>
#ifndef PATH_MAX
#   define PATH_MAX 4096
#endif

#include "interop_class.hpp"
#include "coreclr.hpp"


#if defined(__APPLE__)
constexpr char coreClrDll[] = "libcoreclr.dylib";
#else
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
  if ( coreClrDllPath.size() >= PATH_MAX ) {
      std::cerr << "Path to libcoreclr.so too long!" << std::endl;
      return;
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
  reinterpret_cast< void(*)(myClass&, void (myClass::*)()) >(handle)(tmp, &myClass::print);
}
