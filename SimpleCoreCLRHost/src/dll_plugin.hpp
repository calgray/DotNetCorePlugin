
#include "dll_export.hpp"
#include <iostream>

class IPlugin
{
public:
    virtual void update() = 0;
};

class ida_shared dll_plugin final //TODO: try virtual method
{
public:
    dll_plugin()
    {
        std::cout << "dll plugin constructor" << std::endl;
    }

    ~dll_plugin()
    {
        std::cout << "dll plugin destructor" << std::endl;
    }

    void update()
    {
        std::cout << "dll plugin update" << std::endl;
    }
};
