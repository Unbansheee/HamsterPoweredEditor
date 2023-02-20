#pragma once
#include "Component.h"
#include "PointLight.h"
#include "PointLightComponent.generated.h"

SERIALIZEDCLASS

class PointLightComponent : public Component
{
public:
    COMPONENT_CONSTRUCTOR(PointLightComponent, Component)
    {
        
    }

    GENERATED()
    
    
    void Update(Timestep ts) override;
    void Render(const glm::mat4& transform) override;
    void OnInspectorGUI() override;

    void SetColor(const glm::vec3& color);
    void SetIntensity(float intensity);
    void SetRadius(float radius);

    glm::vec3 GetColor() const;
    float GetIntensity() const;
    float GetRadius() const;

    
    
private:
    SERIALIZEDVAR PointLightData m_PointLight;
};
