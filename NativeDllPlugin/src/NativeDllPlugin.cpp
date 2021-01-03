
#include "dll_export.hpp"

#include <iostream>
#include <IPlugin.hpp>

class ida_shared NativeDllPlugin final : public IPlugin
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

// Export C interface
extern "C"
{
    ida_shared NativeDllPlugin* CreatePlugin()
    {
        return new NativeDllPlugin();
    }

    ida_shared void DestroyPlugin(NativeDllPlugin& plugin)
    {
        delete &plugin;
    }

    ida_shared void InitializePlugin(NativeDllPlugin& plugin)
    {
        plugin.Initialize();
    }

    ida_shared void UpdatePlugin(NativeDllPlugin& plugin)
    {
        plugin.Update();
    }
}
