#include "PointLightComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

void PointLightComponent::Update(Timestep ts)
{
    Component::Update(ts);
}

void PointLightComponent::Render(const glm::mat4& transform)
{
    Component::Render(transform);
    m_PointLight.position = Owner->GetComponent<TransformComponent>()->GetWorldPosition();
    Renderer::Submit(m_PointLight);
}

void PointLightComponent::OnInspectorGUI()
{
    Component::OnInspectorGUI();
    ImGui::ColorEdit3("Color", &m_PointLight.color.r);
    ImGui::DragFloat("Intensity", &m_PointLight.intensity, 0.1f, 0.0f, 100.0f);
    ImGui::DragFloat("Radius", &m_PointLight.radius, 0.1f, 0.0f, 100.0f);
    
    
}

void PointLightComponent::SetColor(const glm::vec3& color)
{
    m_PointLight.color = color;
}

void PointLightComponent::SetIntensity(float intensity)
{
    m_PointLight.intensity = intensity;
    
}

void PointLightComponent::SetRadius(float radius)
{
    m_PointLight.radius = radius;
    
}

glm::vec3 PointLightComponent::GetColor() const
{
    return m_PointLight.color;
    
}

float PointLightComponent::GetIntensity() const
{
    return m_PointLight.intensity;
    
}

float PointLightComponent::GetRadius() const
{
    return m_PointLight.radius;
    
}
