
#include <iostream>

/**
 * @brief An extension written in the project library that hooks
 * into the plugin framework
 * 
 */
class CorePlugin
{
public:
    CorePlugin()
    {
        std::cout << "CorePlugin Constructor" << std::endl;
    }

    ~CorePlugin()
    {
        std::cout << "CorePlugin Destructor" << std::endl;
    }

    void update()
    {
        std::cout << "CorePlugin Update" << std::endl;
    }
};
