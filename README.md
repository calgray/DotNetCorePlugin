# CoreCLRHost

Disclaimer: This is only [PoC](https://en.wikipedia.org/wiki/Proof_of_concept), do not copy-paste this code into production.

This C++ app allows to run custom C# method from compiled C# .dll on Linux and OS X using coreCLR.
In our example that C# method runs C++ class methods on C++ objects using pointers and delegates.
It's code is based on [example from coreCLR](https://github.com/dotnet/coreclr/tree/master/src/coreclr/hosts/unixcoreruncommon)
and was done with big help from [**@janvorli**](https://github.com/janvorli), who patiently answered my questions.

## Prerequisites

* CMake 3.10+
* compiler that supports C++17 Filesystem TS
* .NET Core (with `dotnet` command in your PATH)

## Compilation

```sh
mkdir -p build && cd ./build
cmake -DCMAKE_CXX_COMPILER=g++-8  ..
cmake --build .
```
Then *Managed.dll* and *CoreCLRHost* will appear in bin/ directory in this repo.

## Usage

`./SimplePluginFramework`

*DLL_PATH* - **absolute** path to coreFX/coreCLR DLL dir ( with libcoreclr.so, mscorlib.dll, and other used by program ).

*ASSEMBLY_PATH* - **relative** path to assembly to run.

*ENTRY_POINT_TYPE* - EntryPoint type, often this will be it's class name ( check example ).

*ENTRY_POINT_NAME* - method name to run from given assembly.

**You will get 0x80131040 error with assembly is in DLL_PATH dir!** - this is due to TPA restrictions and this app being *simple*.

### Why we will get 0x80131040 error? What's the solution if I must have them in the same dir?

You will get 0x80131040 error, because your assembly will get listed in Trusted Platform Assemblies. And every requested assemby from TPA list must have valid version string. Most ootb mcs compiled programs have version 0.0.0.0, so In our example instead of requesting "Square" assembly app need to request "Square, Version=0.0.0.0" assembly ( I'm talking about executeAssembly's assemblyName string ). Another workaround is to modify function that add DLLs to TPA list to not add our assembly - I've just used function from coreCLR code which adds all files.

## Example

```sh
cd ./bin
./SimplePluginFramework
```

will print:

```text
Hello World from C#
Value: 42
Value: 8
got pi: 3.14159
got version: v1.2.3
CorePlugin Constructor
nPlugins : 1
NativeDllPlugin Constructor
nPlugins : 2
NativeDllPlugin Constructor
nPlugins : 3
ManagedPlugin Constructor
nPlugins : 4
CorePlugin Initialize
NativeDllPlugin Initialize
NativeDllPlugin Initialize
ManagedPlugin Initialize
CorePlugin Update
NativeDllPlugin Update
NativeDllPlugin Update
ManagedPlugin Update
CorePlugin Destructor
NativeDllPlugin Destructor
NativeDllPlugin Destructor
ManagedPlugin Dispose
```/