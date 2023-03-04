#pragma once
#include <iostream>
#include <string>

#include "Component.h"
#include "NameComponent.generated.h"

SERIALIZEDCLASS(Category = "Core")

class NameComponent : public Component
{
public:
    GENERATED()
    
    COMPONENT_CONSTRUCTOR(NameComponent, Component)
    {
        SetName("Unnamed Object");
        m_initialized = true;
    }

    NameComponent(GameObject* owner, const std::string& name) : Component(owner)
    {
        SetName(name);
        m_initialized = true;
    }

    ~NameComponent()
    {

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
    SERIALIZEDVAR std::string m_name;
    
};

#undef GENERATED