/*
 *  Copyright (c) Hubert Jarosz. All rights reserved.
 *  Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include "simpleCoreCLRHost.hpp"

#include <dynamicLinker.hpp>

#include "coreclr.hpp"
#include "filesystem_utils.hpp"
#include "interop_class.hpp"

#include <climits>
#ifndef PATH_MAX
#   define PATH_MAX 4096
#endif



int runFromEntryPoint(
            const std::string& currentExeAbsolutePath,
            const std::string& clrFilesAbsolutePath,
            const std::string& managedAssemblyAbsoluteDir,
            const std::string& assemblyName,
            const std::string& entryPointType,
            const std::string& entryPointName)
{

  std::string coreClrDllPath = clrFilesAbsolutePath + "/" + coreClrDll;


  if ( coreClrDllPath.size() >= PATH_MAX ) {
      std::cerr << "Path to libcoreclr.so too long!" << std::endl;
      return -1;
  }

  std::string nativeDllSearchDirs = managedAssemblyAbsoluteDir + ":" + clrFilesAbsolutePath;

  std::string tpaList;
  AddFilesFromDirectoryToTpaList( clrFilesAbsolutePath, tpaList );

  auto dl = dynamicLinker::dynamicLinker::make_new( coreClrDllPath );
  auto coreclr_initialize = dl->getFunction<coreclrInitializeFunction>("coreclr_initialize");
  auto coreclr_shutdown = dl->getFunction<coreclrShutdownFunction>("coreclr_shutdown");
  auto coreclr_create_delegate = dl->getFunction<coreclrCreateDelegateFunction>("coreclr_create_delegate");

  try {
    dl->open();
    coreclr_initialize.init();
    coreclr_shutdown.init();
    coreclr_create_delegate.init();
  } catch ( dynamicLinker::openException& e ) {
    std::cerr << "Cannot find " << coreClrDll << "Path that was searched: "
              << coreClrDllPath << std::endl;
    std::cerr << e.what() << std::endl;
    return -1;
  } catch ( dynamicLinker::symbolException& e ) {
    std::cerr << "Probably your libcoreclr is broken or too old." << std::endl;
    std::cerr << e.what() << std::endl;
    return -1;
  } catch ( dynamicLinker::dynamicLinkerException& e ) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  const char *propertyKeys[] = {
      "TRUSTED_PLATFORM_ASSEMBLIES",
      "APP_PATHS",
      "APP_NI_PATHS",
      "NATIVE_DLL_SEARCH_DIRECTORIES",
      "AppDomainCompatSwitch"
  };

  const char *propertyValues[] = {
      tpaList.c_str(),
      managedAssemblyAbsoluteDir.c_str(),
      managedAssemblyAbsoluteDir.c_str(),
      nativeDllSearchDirs.c_str(),
      "UseLatestBehaviorWhenTFMNotSpecified"
  };

  void* hostHandle = nullptr;
  unsigned int domainId = 0;
  int status;

  // initialize coreclr
  try {
    status = coreclr_initialize (
      currentExeAbsolutePath.c_str(),
      "simpleCoreCLRHost",
      sizeof(propertyKeys) / sizeof(propertyKeys[0]),
      propertyKeys,
      propertyValues,
      &hostHandle,
      &domainId
    );
  } catch ( dynamicLinker::dynamicLinkerException& e ) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  if ( status < 0 ) {
    std::cerr << "ERROR! coreclr_initialize status: 0x" << std::hex << status << std::endl;
    return -1;
  }

  void* handle;

  try {
    // create delegate to our entry point
    status = coreclr_create_delegate (
      hostHandle,
      domainId,
      assemblyName.c_str(),
      entryPointType.c_str(),
      entryPointName.c_str(),
      &handle
    );
  } catch ( dynamicLinker::dynamicLinkerException& e ) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  if ( status < 0 ) {
    std::cerr << "ERROR! coreclr_create_delegate status: 0x" << std::hex << status << std::endl;
    return -1;
  }

  myClass tmp = myClass();
  tmp.question();

  /*
   *  If arguments are in in different order then second arg is 0 in C#.
   *  probably something with padding/offset/ptr byte size
   */
  reinterpret_cast< void(*)(myClass&, void (myClass::*)()) >(handle)(tmp, &myClass::print);

  try {
    status = coreclr_shutdown ( hostHandle, domainId );
  } catch ( dynamicLinker::dynamicLinkerException& e ) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  if ( status < 0 ) {
    std::cerr << "ERROR! coreclr_shutdown status: 0x" << std::hex << status << std::endl;
    return -1;
  }

  return 0;
}

