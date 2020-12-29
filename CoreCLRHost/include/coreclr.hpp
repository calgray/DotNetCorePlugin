/*
 *  Copyright (c) Hubert Jarosz. All rights reserved.
 *  Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#pragma once

#include <memory>

namespace dynamicLinker
{
class dynamicLinker;
}

struct CoreclrHandle
{
    void* hostHandle{nullptr};
    unsigned int domainId{0};
};

class CoreCLR
{
private:
    std::shared_ptr<dynamicLinker::dynamicLinker> dl;
    CoreclrHandle coreclrHandle{};

    void initializeCoreCLRDynamicLibrary(const std::string&);
    void initializeCoreCLR(const std::string&, const std::string&, const std::string&);
public:
    explicit CoreCLR(const std::string&, const std::string&, const std::string&, const std::string&);
    ~CoreCLR();

    void* getCSharpFunctionPtr(const std::string&, const std::string&, const std::string&);
};