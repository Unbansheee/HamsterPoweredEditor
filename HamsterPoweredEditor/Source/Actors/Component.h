#pragma once
//
#include <functional>
#include <glm/fwd.hpp>
#include "Core/HamsterPoweredEngine.h"
#include "json.hpp"
#include "Core/Timestep.h"

#define COMPONENT_GEN() \
void Serialize(nlohmann::json& j) override; \
void Deserialize(nlohmann::json& j) override;

class GameObject;
class Component;


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

    virtual void Serialize(nlohmann::json& j) {};
    virtual void Deserialize(nlohmann::json& j) {};
    virtual void SerializeCustom(nlohmann::json& j) {};
    virtual void DeserializeCustom(nlohmann::json& j) {};

    
    
protected:
    GameObject* Owner;
    

};


#define COMPONENT_CONSTRUCTOR(componentName, parentClassName) \
    componentName(GameObject* owner) : parentClassName(owner)