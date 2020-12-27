
#include <iostream>

class native_plugin
{
public:
    native_plugin()
    {
        std::cout << "native plugin constructor" << std::endl;
    }

    ~native_plugin()
    {
        std::cout << "native plugin destructor" << std::endl;
    }

    void update()
    {
        std::cout << "native plugin update" << std::endl;
    }
};
