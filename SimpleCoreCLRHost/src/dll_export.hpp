
#pragma once

// Exporting all to c++
// #ifdef __cplusplus
// #define EXTERNC extern "C"
// #else
#define EXTERNC
// #endif

#if defined(__NT__)
    
    // Microsoft switching header
    #if defined(SimpleCoreCLRHost_EXPORTS)
    #define DLLEXPORT               __declspec(dllexport)
    #else
    #define DLLEXPORT               __declspec(dllimport)
    #endif

    #define idaapi
    #define ida_shared              EXTERNC DLLEXPORT
    #define ida_local

#elif defined(__unix__)
    #define idaapi

    #if defined(__MAC__)
        #define ida_shared          EXTERNC __attribute__((visibility("default")))
        #define ida_local           EXTERNC __attribute__((visibility("hidden")))
    #else // Linux
        #if __GNUC__ >= 4
            #define ida_shared      EXTERNC __attribute__((visibility("default")))
            #define ida_local       EXTERNC __attribute__((visibility("hidden")))
        #else
            #define ida_shared      EXTERNC
            #define ida_local
        #endif
    #endif
    
#endif