#pragma once
#include <string>

#include "Core/Timestep.h"

class Object
{
public:
    virtual ~Object() = default;
    virtual void Begin() = 0;
    virtual void Update(Timestep ts) = 0;
    virtual void OnDestroy() = 0;

    void SetName(std::string name);
    std::string GetName();

protected:
    std::string m_name = "Object";
};
