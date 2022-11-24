#pragma once
#include <string>

#include "Component.h"

class NameComponent : public Component
{
public:
    COMPONENT_CONSTRUCTOR(NameComponent, Component)
    {
        SetName("Unnamed Object");
    }

    NameComponent(GameObject* owner, const std::string& name) : Component(owner)
    {
        SetName(name);
    }

    void SetName(const std::string& name)
    {
        m_name = name;
    }

    const std::string& GetName() const
    {
        return m_name;
    }

    void OnInspectorGUI() override;
    
    
private:
    std::string m_name;
    
};
