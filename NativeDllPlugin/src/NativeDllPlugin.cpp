
#include "dll_export.hpp"

#include <iostream>

extern "C" __attribute__((visibility("default"))) void HelloWorld()
{
    std::cout << "Hello World" << std::endl;
}

class IPlugin
{
    virtual void Initialize() = 0;
    virtual void Update() = 0;
};

class NativeDllPlugin : IPlugin
{
public:
    void Initialize() {}
    void Update() {}
};
