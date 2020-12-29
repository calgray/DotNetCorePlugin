
#pragma once

class IPlugin
{
    virtual void Initialize() = 0;
    virtual void Update() = 0;
};