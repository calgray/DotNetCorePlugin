
#include <iostream>

class CorePlugin
{
public:
    CorePlugin()
    {
        std::cout << "core plugin constructor" << std::endl;
    }

    ~CorePlugin()
    {
        std::cout << "core plugin destructor" << std::endl;
    }

    void update()
    {
        std::cout << "core plugin update" << std::endl;
    }
};
