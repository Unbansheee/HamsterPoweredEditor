#pragma once
#include "Component.h"
#include "DirectionalLightComponent.generated.h"

SERIALIZEDCLASS(Category = "Lights")

class DirectionalLightComponent : public Component
{
public:

    GENERATED()
    
    DirectionalLightComponent(GameObject* owner)
        : Component(owner)
    {
    }

    void Update(Timestep ts) override;
    void Render(const glm::mat4& transform) override;
    void OnInspectorGUI() override;
    void SerializeCustom(nlohmann::json& j) override;
    void DeserializeCustom(nlohmann::json& j) override;

    void SetColor(const glm::vec3& color);
    void SetIntensity(float intensity);
    glm::vec3 GetColor() const;
    float GetIntensity() const;

private:
    SERIALIZEDVAR DirectionalLightData m_Light = { glm::vec3(0.0f, 1.f, 0.f), glm::vec3(1.0f), 1.0f };
};

#undef GENERATED