#pragma once
//
#include <glm/fwd.hpp>

#include "Core/Timestep.h"

class GameObject;




class Component
{
public:
    Component(GameObject* owner) : Owner(owner) {}
    Component(const Component& other) = delete;
    Component& operator=(const Component& other) = delete;
    virtual ~Component() = default;

    virtual void Update(Timestep ts) {}
    virtual void Render(const glm::mat4& transform) {}

    virtual void OnInspectorGUI() {}

    GameObject* GetOwner() const { return Owner; }
    
protected:
    GameObject* Owner;
};


#define COMPONENT_CONSTRUCTOR(componentName, parentClassName) \
    componentName(GameObject* owner) : parentClassName(owner)