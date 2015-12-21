/*
 *  Copyright (c) Hubert Jarosz. All rights reserved.
 *  Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#if not defined (__unix__) && not defined(__unix) && not defined (unix) && ( (not defined (__APPLE__) || not defined (__MACH__)) )
  #error THIS SOFTWARE IS ONLY FOR UNIX-LIKE SYSTEMS!
#endif

#if not defined ( __GNUC__ ) || __GNUC__ < 5 || ( __GNUC__ == 5 && __GNUC_MINOR__ < 3 )
  #error THIS SOFTWARE CURRENTLY BUILDS ONLY ON GCC 5.3 OR NEWER!
#endif

#pragma once

#include <functional>
#include <iostream>
#include "dynamicLinker/dynamicLinker.hpp"

// getcwd on Linux
#include <unistd.h>

// PATH_MAX on Linux
#include <limits.h>

#if not defined PATH_MAX
  #warning Is this GNU/Hurd? Then this code could occupy a lot of memory.
  #include <stdio.h>
  #define PATH_MAX FILENAME_MAX
#endif

#if defined(__APPLE__)
  std::string coreClrDll = "libcoreclr.dylib";
#else
  std::string coreClrDll = "libcoreclr.so";
#endif

class myClass {
private:
  int value;
public:
  void question() { value = 42; }
  void print() { std::cout << "Value: " <<  value << std::endl; }
};

typedef void (csharp_runIt_t)( myClass&, std::mem_fun_ref_t<void, myClass> );
