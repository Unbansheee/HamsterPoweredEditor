#pragma once
#include <map>
#include <string>
#include <vector>

#include "ComponentRegistry.generated.h"
#include "Scene.h"


struct RegisteredComponent
{
    std::string name;
    void* (*constructor)();
    
};

class ComponentRegistry
{
public: 
    template <typename T>
    static void RegisterComponent(const std::string& className)
    {
        //check if already registered
        if (m_registry.find(className) != m_registry.end())
        {
            return;
        }
        
        RegisteredComponent component;


        
        component.name = className;
        component.constructor = &GameObject::AddComponent<T>();
        m_registry[className] = component;
        
    }

    //GENERATED_REGISTRY()

    inline static std::map<std::string, RegisteredComponent> m_registry;
    
};
