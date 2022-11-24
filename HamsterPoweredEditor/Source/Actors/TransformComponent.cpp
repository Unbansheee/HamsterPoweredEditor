#include "TransformComponent.h"

#include <iostream>
#include <ostream>
#include <glm/gtx/quaternion.hpp>

#include "GameObject.h"
#include "imgui.h"




void TransformComponent::Update(Timestep ts)
{
    if (isDirty)
    {
        isDirty = false;
        UpdateTransform();
    }
}

void TransformComponent::Render(const glm::mat4& transform)
{
}

void TransformComponent::OnInspectorGUI()
{
    Component::OnInspectorGUI();

        glm::vec3 pos = LocalPosition;
        if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
        {
            SetLocalPosition(pos);
        }

        glm::vec3 rot = GetLocalRotation();
        if(ImGui::DragFloat3("Rotation", &rot.x))
        {
            SetLocalRotation(rot);
        }
        
        glm::vec3 scale = GetLocalScale();
        if (m_lockedScale)
        {
            if (ImGui::DragFloat("Scale", &scale.x, 0.1f))
            {
                float deltaScale = scale.x - GetLocalScale().x;
                if (scale.x == 0.f)
                {
                    scale.x = 0.000001f * glm::sign(deltaScale);
                }
                
                float yscale = scale.x * GetLocalScale().y / GetLocalScale().x;
                float zscale = scale.x * GetLocalScale().z / GetLocalScale().x;
                SetLocalScale({scale.x, yscale, zscale});
            }
        }
        else
        {
            if(ImGui::DragFloat3("Scale", &scale.x, 0.1f))
            {
                float deltaScaleX = scale.x - GetLocalScale().x;
                if (scale.x == 0.f)
                {
                    scale.x = 0.000001f * glm::sign(deltaScaleX);
                }

                float deltaScaleY = scale.y - GetLocalScale().y;
                if (scale.y == 0.f)
                {
                    scale.y = 0.000001f * glm::sign(deltaScaleY);
                }

                float deltaScaleZ = scale.z - GetLocalScale().z;
                if (scale.z == 0.f)
                {
                    scale.z = 0.000001f * glm::sign(deltaScaleZ);
                }
                
                SetLocalScale(scale);
            }
        }
        ImGui::Checkbox("Lock Scale", &m_lockedScale);

}

void TransformComponent::SetWorldPosition(const glm::vec3& position)
{
    // set local position against parent transform
    if (m_Parent)
    {
        LocalPosition = m_Parent->GetWorldTransform() * glm::vec4(position, 1.0f);
    }
    else
    {
        LocalPosition = position;
    }
    isDirty = true;
}

void TransformComponent::SetWorldRotation(const glm::vec3& rotation)
{
    
    if (m_Parent)
    {
        glm::vec3 parentRotation = m_Parent->GetWorldRotation();
        LocalRotation = rotation - parentRotation;
    }
    else
    {
        LocalRotation = rotation;
    }
    isDirty = true;
}

void TransformComponent::SetWorldScale(const glm::vec3& scale)
{
    
    if (m_Parent)
    {
        glm::vec3 parentScale = m_Parent->GetWorldScale();
        LocalScale = glm::vec3(scale.x / parentScale.x, scale.y / parentScale.y, scale.z / parentScale.z);
    }
    else
    {
        LocalScale = scale;
    }
    
    isDirty = true;
}

void TransformComponent::SetLocalPosition(const glm::vec3& position)
{
    LocalPosition = position;
    isDirty = true;
}

void TransformComponent::SetLocalRotation(const glm::vec3& rotation)
{
    // convert to radians and update Orientation quaternion
    LocalRotation = rotation;
    
    
    isDirty = true;
}


void TransformComponent::SetLocalScale(const glm::vec3& scale)
{
    LocalScale = scale;
    isDirty = true;
}

void TransformComponent::SetParent(TransformComponent* parent)
{
    m_Parent->AddChild(this);
    isDirty = true;
}

void TransformComponent::AddChild(TransformComponent* child)
{
    m_Children.push_back(child);
    child->m_Parent = this;
    isDirty = true;
}

void TransformComponent::RemoveChild(TransformComponent* child)
{
    auto it = std::find(m_Children.begin(), m_Children.end(), child);
    if (it != m_Children.end())
    {
        m_Children.erase(it);
        child->m_Parent = nullptr;
    }
    isDirty = true;
    
}

void TransformComponent::RemoveFromParent()
{
    if (m_Parent)
    {
        m_Parent->RemoveChild(this);
    }
}

void TransformComponent::RemoveAllChildren()
{
    for (auto child : m_Children)
    {
        child->m_Parent = nullptr;
    }
    m_Children.clear();
    isDirty = true;
}

const glm::vec3& TransformComponent::GetWorldPosition() const
{
    return glm::vec3(GetWorldTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

const glm::vec3& TransformComponent::GetWorldRotation() const
{
    // get rotation quaternion against world transform
    glm::quat rotation = glm::quat_cast(GetWorldTransform());
    return glm::eulerAngles(rotation);
}

const glm::vec3& TransformComponent::GetWorldScale() const
{
    // get scale against world transform
    return glm::vec3(glm::length(GetWorldTransform()[0]), glm::length(GetWorldTransform()[1]), glm::length(GetWorldTransform()[2]));
}

const glm::vec3& TransformComponent::GetLocalPosition() const
{
    return LocalPosition;
}

const glm::vec3& TransformComponent::GetLocalRotation() const
{
    return LocalRotation;
}

const glm::vec3& TransformComponent::GetLocalScale() const
{
    return LocalScale;
}

const glm::mat4& TransformComponent::GetWorldTransform() const
{
    if (m_Parent)
    {
        return m_Parent->GetWorldTransform() * LocalTransform;
    }
    return LocalTransform;
}

void TransformComponent::UpdateTransform()
{
    LocalTransform = glm::mat4(1.0f);
    LocalTransform = glm::translate(LocalTransform, LocalPosition);

    LocalTransform = glm::rotate(LocalTransform, glm::radians(LocalRotation.x), glm::vec3(1.0,0.0,0.0));
    LocalTransform = glm::rotate(LocalTransform, glm::radians(LocalRotation.y), glm::vec3(0.0,1.0,0.0));
    LocalTransform = glm::rotate(LocalTransform, glm::radians(LocalRotation.z), glm::vec3(0.0,0.0,1.0));
    
    LocalTransform = glm::scale(LocalTransform, LocalScale);
}
