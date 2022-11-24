﻿#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Component.h"

class TransformComponent : public Component
{
public:
    TransformComponent(GameObject* owner)
        : Component(owner)
    {
    }

    void Update(Timestep ts) override;
    void Render(const glm::mat4& transform) override;
    void OnInspectorGUI() override;

    void SetWorldPosition(const glm::vec3& position);
    void SetWorldRotation(const glm::vec3& rotation);
    void SetWorldScale(const glm::vec3& scale);

    void SetLocalPosition(const glm::vec3& position);
    void SetLocalRotation(const glm::vec3& rotation);
    void SetLocalScale(const glm::vec3& scale);

    const std::vector<TransformComponent*>& GetChildren() const { return m_Children; }
    TransformComponent* GetParent() const { return m_Parent; }
    void SetParent(TransformComponent* parent);
    void AddChild(TransformComponent* child);
    void RemoveChild(TransformComponent* child);
    void RemoveFromParent();
    void RemoveAllChildren();
    

    const glm::vec3& GetWorldPosition() const;
    const glm::vec3& GetWorldRotation() const;
    const glm::vec3& GetWorldScale() const;

    const glm::vec3& GetLocalPosition() const;
    const glm::vec3& GetLocalRotation() const;
    const glm::vec3& GetLocalScale() const;

    const glm::mat4& GetWorldTransform() const;

private:
    void UpdateTransform();
    
    TransformComponent* m_Parent = nullptr;
    std::vector<TransformComponent*> m_Children;
    
    bool isDirty = true;

    glm::mat4 LocalTransform;
    
    glm::vec3 LocalPosition;
    glm::vec3 LocalRotation;
    glm::vec3 LocalScale = {1, 1, 1};

    bool m_lockedScale = true;

    
};
