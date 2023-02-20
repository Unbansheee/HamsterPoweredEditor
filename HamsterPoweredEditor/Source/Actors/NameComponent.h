#pragma once
#include <iostream>
#include <string>

#include "Component.h"
#include "NameComponent.generated.h"

SERIALIZEDCLASS

class NameComponent : public Component
{
public:
    GENERATED()
    
    COMPONENT_CONSTRUCTOR(NameComponent, Component)
    {
        SetName("Unnamed Object");
        std::cout << "NameComponent constructor" << std::endl;
    }

    NameComponent(GameObject* owner, const std::string& name) : Component(owner)
    {
        SetName(name);
        std::cout << "NameComponent constructor" << std::endl;
    }

    ~NameComponent()
    {
        std::cout << "NameComponent destructor" << std::endl;
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
