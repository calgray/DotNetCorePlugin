
#include "dll_export.hpp"

#include <iostream>

ida_shared void HelloWorld()
{
    std::cout << "Hello World" << std::endl;
}

class IPlugin
{
public:
    virtual ~IPlugin() = 0;
    virtual void Initialize() = 0;
    virtual void Update() = 0;
};

class NativeDllPlugin
{
public:
    NativeDllPlugin()
    {
        std::cout << "native dll plugin constructor" << std::endl;
    }

    ~NativeDllPlugin()
    {
        std::cout << "native dll plugin destructor" << std::endl;
    }

    void Initialize()
    {
        std::cout << "native dll plugin initialize" << std::endl;
    }

    void Update()
    {
        std::cout << "native dll plugin update" << std::endl;
    }
};


ida_shared NativeDllPlugin* CreatePlugin()
{
    return new NativeDllPlugin();
}

ida_shared void DeletePlugin(NativeDllPlugin& plugin)
{
    delete &plugin;
}

ida_shared void UpdatePlugin(NativeDllPlugin& plugin)
{
    plugin.Update();
}
