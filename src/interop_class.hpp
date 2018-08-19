/*
 *  Copyright (c) Hubert Jarosz. All rights reserved.
 *  Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <iostream>

class myClass {
private:
    int value;
public:
    void question() { value = 42; }
    void print() { std::cout << "Value: " <<  value << std::endl; }
};