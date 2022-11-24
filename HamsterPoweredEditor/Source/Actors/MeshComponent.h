#pragma once
#include "Component.h"
#include "Rendering/DynamicMesh.h"

class MeshComponent : public Component
{
public:
    MeshComponent(GameObject* owner) : Component(owner)
    {
        m_Mesh.Load("Resources/Meshes/Monkey.obj");
    }

    MeshComponent(GameObject* owner, const std::string& meshPath) : Component(owner)
    {
        m_Mesh.Load(meshPath);
    }

    void LoadMesh(const std::string& meshPath)
    {
        m_Mesh.Load(meshPath);
    }

    
    
    void Update(Timestep ts) override;
    void Render(const glm::mat4& transform) override;

protected:
    DynamicMesh m_Mesh;
};
