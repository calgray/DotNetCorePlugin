//
// Created by Hubert Jarosz on 19/08/2018.
//

#include "coreclr.hpp"

#include <string>

#include <dynamicLinker.hpp>
#include "filesystem_utils.hpp"
#include "coreclr_prototypes.hpp"

CoreCLR::CoreCLR(const std::string& coreCLRPath,
                 const std::string& managedAssemblyAbsoluteDir,
                 const std::string& clrFilesAbsolutePath,
                 const std::string& currentExePath)
{
    //CoreCLR(clrFilesAbsolutePath + "/" + coreClrDll, ...);

    std::cout << "coreCLRPath: " << coreCLRPath << std::endl;
    std::cout << "managedAssemblyAbsoluteDir: " << managedAssemblyAbsoluteDir << std::endl;
    std::cout << "clrFilesAbsolutePath: " << clrFilesAbsolutePath << std::endl;
    std::cout << "currentExePath: " << currentExePath << std::endl;

    initializeCoreCLRDynamicLibrary(coreCLRPath);
    initializeCoreCLR(managedAssemblyAbsoluteDir, clrFilesAbsolutePath, currentExePath);
}

CoreCLR::~CoreCLR()
{

    auto coreclr_shutdown = dl->getFunction<coreclrShutdownFunction>("coreclr_shutdown");
    coreclr_shutdown.init();

    int status = coreclr_shutdown(coreclrHandle.hostHandle, coreclrHandle.domainId);

    if ( status < 0 ) {
        std::cerr << "ERROR! coreclr_shutdown status: 0x" << std::hex << status << std::endl;
    }

}

void CoreCLR::initializeCoreCLRDynamicLibrary(const std::string& coreCLRPath)
{
    dl = dynamicLinker::dynamicLinker::make_new(coreCLRPath);

    try {
        dl->open();
    } catch ( dynamicLinker::openException& e ) {
        std::cerr << "Cannot find CoreCLR library at " << coreCLRPath << std::endl;
        std::cerr << e.what() << std::endl;
        throw e;
    } catch ( dynamicLinker::symbolException& e ) {
        std::cerr << "Probably your libcoreclr is broken or too old." << std::endl;
        std::cerr << e.what() << std::endl;
        throw e;
    } catch ( dynamicLinker::dynamicLinkerException& e ) {
        std::cerr << e.what() << std::endl;
        throw e;
    }
}

void CoreCLR::initializeCoreCLR(const std::string& managedAssemblyAbsoluteDir,
                                const std::string& clrFilesAbsolutePath,
                                const std::string& currentExePath)
{
    std::string nativeDllSearchDirs = managedAssemblyAbsoluteDir + ":" + clrFilesAbsolutePath;
    std::string tpaList;
    AddFilesFromDirectoryToTpaList(clrFilesAbsolutePath, tpaList);
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

    auto coreclr_initialize = dl->getFunction<coreclrInitializeFunction>("coreclr_initialize");
    coreclr_initialize.init();

    // initialize coreclr
    int status = coreclr_initialize(
            currentExePath.c_str(),
            "simpleCoreCLRHost",
            sizeof(propertyKeys) / sizeof(propertyKeys[0]),
            propertyKeys,
            propertyValues,
            &coreclrHandle.hostHandle,
            &coreclrHandle.domainId
    );

    if (status < 0)
    {
        std::cerr << "ERROR! coreclr_initialize status: 0x" << std::hex << status << std::endl;
    }
}

void* CoreCLR::getCSharpFunctionPtr(
        const std::string& assemblyName,
        const std::string& entryPointType,
        const std::string& entryPointName)
{
    void* handle;

    auto coreclr_create_delegate = dl->getFunction<coreclrCreateDelegateFunction>("coreclr_create_delegate");
    coreclr_create_delegate.init();

    // create delegate to our entry point
    int status = coreclr_create_delegate(
            coreclrHandle.hostHandle,
            coreclrHandle.domainId,
            assemblyName.c_str(),
            entryPointType.c_str(),
            entryPointName.c_str(),
            &handle
    );

    if (status < 0)
    {
        std::cerr << "ERROR! coreclr_create_delegate status: 0x" << std::hex << status << std::endl;
        return nullptr;
    }
    return handle;
}

