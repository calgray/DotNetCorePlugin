
#pragma once

class IPlugin
{
public:
    //virtual ~IPlugin() = 0;
    virtual void Initialize() = 0;
    virtual void Update() = 0;
};