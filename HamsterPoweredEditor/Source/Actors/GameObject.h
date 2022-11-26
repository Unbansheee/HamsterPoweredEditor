#pragma once
#include <string>
#include <vector>
#include "Core/HamsterPoweredEngine.h"
#include "Component.h"


class GameObject
{
private:
    
    
    
public:
    friend class Scene;
    
    GameObject();

    virtual void Update(Timestep ts);
    virtual void SetupComponents();
    virtual void Render();
    virtual void OnInspectorGUI();

    void Destroy();
    

    //Wrapper Functions for transform component
    GameObject* GetParent();
    void SetParent(GameObject* parent);
    void AddChild(GameObject* child);
    void RemoveChild(GameObject* child);
    void RemoveAllChildren();
    void RemoveFromParent();

    void Serialize(nlohmann::json& j);
    void Deserialize(nlohmann::json& j);
    
    
    // Add component with custom constructor
    template <typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        T* component = new T(this, std::forward<Args>(args)...);
        components.push_back(component);
        return component;
    }

    // Get component of type
    template <typename T>
    T* GetComponent()
    {
        for (Component* component : components)
        {
            T* casted = dynamic_cast<T*>(component);
            if (casted)
            {
                return casted;
            }
        }
        return nullptr;
    }

    // Get all components of type
    template <typename T>
    std::vector<T*> GetComponents()
    {
        std::vector<T*> componentsOfType;
        for (Component* component : components)
        {
            T* casted = dynamic_cast<T*>(&component);
            if (casted)
            {
                componentsOfType.push_back(casted);
            }
        }
        return componentsOfType;
    }

    std::vector<Component*> components = {};

};
