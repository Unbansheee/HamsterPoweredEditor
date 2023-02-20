#include "BoxCollider.h"

#include "imgui.h"
#include "Actors/TransformComponent.h"
#include "Actors/GameObject.h"

BoxCollider::BoxCollider(GameObject* gameObject) : Collider::Collider(gameObject)
{
    TransformComponent* transform = GetOwner()->GetComponent<TransformComponent>();

    //init physx shape
    
}

BoxCollider::~BoxCollider()
{
    
}

void BoxCollider::Update(Timestep ts)
{
    Collider::Update(ts);
    
    
}

void BoxCollider::Render(const glm::mat4& transform)
{
    Collider::Render(transform);
}

void BoxCollider::OnInspectorGUI()
{
    Collider::OnInspectorGUI();
    if (ImGui::DragFloat3("Size", &m_Size.x, 0.1f, 0.0f, 0.0f, "%.1f"))
    {
        
        SetSize(m_Size);
        
    }
}


void BoxCollider::SerializeCustom(nlohmann::json& j)
{
    Collider::SerializeCustom(j);
}

void BoxCollider::DeserializeCustom(nlohmann::json& j)
{
    Collider::DeserializeCustom(j);
}

void BoxCollider::SetSize(const glm::vec3& size)
{
    m_Size = size;
    
    
}

const glm::vec3& BoxCollider::GetSize() const
{
    return m_Size;
}
