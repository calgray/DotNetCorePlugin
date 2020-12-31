
#include "dll_export.hpp"

#include <iostream>

ida_shared void HelloWorld()
{
    std::cout << "Hello World" << std::endl;
}

class IPlugin
{
public:
    //virtual ~IPlugin() = 0;
    virtual void Initialize() = 0;
    virtual void Update() = 0;
};

class NativeDllPlugin final : public IPlugin
{
public:
    NativeDllPlugin()
    {
        std::cout << "NativeDllPlugin Constructor" << std::endl;
    }

    ~NativeDllPlugin()
    {
        std::cout << "NativeDllPlugin Destructor" << std::endl;
    }

    void Initialize()
    {
        std::cout << "NativeDllPlugin Initialize" << std::endl;
    }

    void Update()
    {
        std::cout << "NativeDllPlugin Update" << std::endl;
    }
};

/// Export C interface

ida_shared NativeDllPlugin* CreatePlugin()
{
    return new NativeDllPlugin();
}

ida_shared void DestroyPlugin(NativeDllPlugin& plugin)
{
    delete &plugin;
}

ida_shared void UpdatePlugin(NativeDllPlugin& plugin)
{
    plugin.Update();
}
