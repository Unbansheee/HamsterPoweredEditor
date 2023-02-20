#include "Collider.h"

#include "imgui.h"
#include "Actors/GameObject.h"
#include "Actors/TransformComponent.h"


Collider::~Collider()
{
}

void Collider::Update(Timestep ts)
{
    Component::Update(ts);
    // get position of the physics actor
    
    
    
    
}

void Collider::Render(const glm::mat4& transform)
{
    Component::Render(transform);
}

void Collider::OnInspectorGUI()
{
    Component::OnInspectorGUI();
    ImGui::Checkbox("Active", &m_Active);
}


void Collider::SerializeCustom(nlohmann::json& j)
{
    Component::SerializeCustom(j);
}

void Collider::DeserializeCustom(nlohmann::json& j)
{
    Component::DeserializeCustom(j);
}

void Collider::SetActorLocation(const glm::vec3& location)
{
    
    
}

void Collider::SetActorRotation(const glm::vec3& rotation)
{
    
    
}
