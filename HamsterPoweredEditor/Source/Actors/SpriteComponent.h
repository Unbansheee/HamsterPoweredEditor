#pragma once
#include "Component.h"
#include "SpriteComponent.generated.h"

SERIALIZEDCLASS(Category = "2D")

class SpriteComponent : public Component
{
public: 

    GENERATED()
    
    SpriteComponent(GameObject* owner)
        : Component(owner)
    {
        m_Mesh.Load("Resources/Meshes/Plane.obj");
        RenderSettings settings;
        settings.Culling = false;
        m_Mesh.SetRenderSettings(settings);
    }

    void Update(Timestep ts) override;
    void Render(const glm::mat4& transform) override;
    void OnInspectorGUI() override;
    //void SerializeCustom(nlohmann::json& j) override;
    void DeserializeCustom(nlohmann::json& j) override
    {
        m_Mesh.SetTexture(m_TexturePath);
    };

protected:
    DynamicMesh m_Mesh;
    SERIALIZEDVAR std::string m_TexturePath;
    
    
};

#undef GENERATED