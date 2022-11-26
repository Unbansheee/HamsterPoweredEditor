#pragma once
#include <map>
#include <vector>

#include "SerializedProperties.h"

struct RegisteredComponent
{
    std::vector<SerializedProperty> properties;
    //component constructor function
    void* (*constructor)();
    
};

class ComponentRegistry
{
public:
    //register a component name with a constructor function for instantiating it
    static void RegisterComponent(const std::string& className, void* (*constructor)())
    {
        //check if already registered
        if (m_registry.find(className) != m_registry.end())
        {
            return;
        }
        
        RegisteredComponent component;

        std::vector<SerializedProperty> properties;
        properties = SerializedProperties::GetProperties(className);
        component.constructor = constructor;
        component.properties = properties;
        m_registry[className] = component;
        
    }

    inline static std::map<std::string, RegisteredComponent> m_registry;
    
};
