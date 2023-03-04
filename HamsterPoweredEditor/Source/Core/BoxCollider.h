#pragma once
#include "Collider.h"
#include "glm/glm.hpp"
#include "BoxCollider.generated.h"

SERIALIZEDCLASS(Category = "Physics")

class BoxCollider : public Collider
{
public:

    GENERATED()
    
    BoxCollider(GameObject* gameObject);
    
    ~BoxCollider() override;
    void Update(Timestep ts) override;
    void Render(const glm::mat4& transform) override;
    void OnInspectorGUI() override;
    void SerializeCustom(nlohmann::json& j) override;
    void DeserializeCustom(nlohmann::json& j) override;

    void SetSize(const glm::vec3& size);
    const glm::vec3& GetSize() const;

protected:
    SERIALIZEDVAR glm::vec3 m_Size = { 1.0f, 1.0f, 1.0f };
    
    
    
    
    
};

#undef GENERATED