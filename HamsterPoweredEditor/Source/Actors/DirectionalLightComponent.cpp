#include "DirectionalLightComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

void DirectionalLightComponent::Update(Timestep ts)
{
    Component::Update(ts);
    glm::vec3 forwardVector;
    //convert m_Transform to a forward vector
    TransformComponent* transform = GetOwner()->GetComponent<TransformComponent>();
    forwardVector.x = transform->GetWorldTransform()[2][0];
    forwardVector.y = transform->GetWorldTransform()[2][1];
    forwardVector.z = transform->GetWorldTransform()[2][2];
    //set the direction of the light to the forward vector
    m_Light.direction = forwardVector;
}

void DirectionalLightComponent::Render(const glm::mat4& transform)
{
    Component::Render(transform);
    
    Renderer::Submit(m_Light);
}

void DirectionalLightComponent::OnInspectorGUI()
{
    Component::OnInspectorGUI();
    ImGui::ColorEdit3("Color", &m_Light.color[0]);
    ImGui::DragFloat("Intensity", &m_Light.intensity, 0.1f, 0.0f, 100.0f);
}

void DirectionalLightComponent::SerializeCustom(nlohmann::json& j)
{
    Component::SerializeCustom(j);
}

void DirectionalLightComponent::DeserializeCustom(nlohmann::json& j)
{
    Component::DeserializeCustom(j);
}

void DirectionalLightComponent::SetColor(const glm::vec3& color)
{
    m_Light.color = color;
}

void DirectionalLightComponent::SetIntensity(float intensity)
{
    m_Light.intensity = intensity;
}

glm::vec3 DirectionalLightComponent::GetColor() const
{
    return m_Light.color;
}

float DirectionalLightComponent::GetIntensity() const
{
    return m_Light.intensity;
}
