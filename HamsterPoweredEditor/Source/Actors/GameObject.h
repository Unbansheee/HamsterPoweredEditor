#pragma once
#include <string>
#include <vector>
#include "Core/HamsterPoweredEngine.h"
#include "Component.h"


class TransformComponent;

class GameObject
{
private:
    Scene* m_Scene;
    
    
public:
    friend class Scene;
    
    
    GameObject();
    ~GameObject();

    virtual void Begin();
    virtual void FixedUpdate(double ts);
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
    TransformComponent* GetTransform();

    void Serialize(nlohmann::json& j);
    void Deserialize(nlohmann::json& j);

    Scene* GetScene() const { return m_Scene; }
    

    // operator ==
    bool operator==(const GameObject& other) const
    {
        return id == other.id;
    }
    
    // Add component with custom constructor
    template <typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        T* component = new T(this, std::forward<Args>(args)...);
        components.push_back(component);
        if (initialized)
        {
            component->Begin();
            component->m_initialized = true;
        }
        return component;
    }

    // Get component of type
    template <typename T>
    T* GetComponent()
    {
        for (Component* component : components)
        {
            if (T* casted = dynamic_cast<T*>(component))
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
    int id = -1;

    static inline int nextId = 0;
    bool initialized = false;
    TransformComponent* transform = nullptr;

};
