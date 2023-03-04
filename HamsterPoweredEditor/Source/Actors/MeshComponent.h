#pragma once
#include "Component.h"
#include "Rendering/DynamicMesh.h"
#include "MeshComponent.generated.h"

SERIALIZEDCLASS(Category = "3D")

class MeshComponent : public Component
{
public:
    GENERATED()
    
    MeshComponent(GameObject* owner) : Component(owner)
    {
        LoadMesh("Resources/Meshes/Monkey.obj");
    }

    MeshComponent(GameObject* owner, const std::string& meshPath) : Component(owner)
    {
        LoadMesh(meshPath);
    }

    void LoadMesh(const std::string& meshPath)
    {
        m_Mesh.Load(meshPath);
        m_MeshPath = meshPath;
    }
    
    void Update(Timestep ts) override;
    void Render(const glm::mat4& transform) override;

    void OnInspectorGUI() override;

    void DeserializeCustom(nlohmann::json& j) override { LoadMesh(m_MeshPath); }
    //void SerializeCustom(nlohmann::json& j) override { j["MeshPath"] = m_MeshPath; }

protected:
    DynamicMesh m_Mesh;
    SERIALIZEDVAR std::string m_MeshPath;
};

#undef GENERATED