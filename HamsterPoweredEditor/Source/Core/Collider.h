#pragma once
#include "Actors/Component.h"
#include "Physics.h"
#include "Collider.generated.h"

SERIALIZEDCLASS

//Collider for physx to use
class Collider : public Component
{
public:
    GENERATED()
    
    Collider(GameObject* gameObject) : Component(gameObject) {}
    ~Collider();

    void Update(Timestep ts) override;
    void Render(const glm::mat4& transform) override;
    void OnInspectorGUI() override;
    void SerializeCustom(nlohmann::json& j) override;
    void DeserializeCustom(nlohmann::json& j) override;

    //Getters


    //Setters


    SERIALIZEDFUNCTION void SetActorLocation(const glm::vec3& location);
    SERIALIZEDFUNCTION void SetActorRotation(const glm::vec3& rotation);
    

protected:


    SERIALIZEDVAR bool m_Active = true;
    
    
};
