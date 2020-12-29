/*
 *  Copyright (c) Hubert Jarosz. All rights reserved.
 *  Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#pragma once

#include <iostream>

class interop_class {
private:
    int value = 1;
public:
    void question()
    {
        value = 42;
    }

    void set(int v)
    {
        value = v;
    }

    void set_sum(int a, int b)
    {
        value = a + b;
    }

    void print()
    {
        std::cout << "Value: " <<  value << std::endl;
    }
};