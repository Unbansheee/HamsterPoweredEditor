#pragma once
#include "Component.h"
#include "MeshComponent.h"
#include "ResourceManagement/Cubemap.h"

#include "SkyboxComponent.generated.h"


SERIALIZEDCLASS(Category = "3D")

class SkyboxComponent : public MeshComponent
{

GENERATED()

public:
SkyboxComponent(GameObject* owner);
    ~SkyboxComponent();
    
void SerializeCustom(nlohmann::json& j) override;
void Update(Timestep ts) override;
void Render(const glm::mat4& transform) override;
void OnInspectorGUI() override;
void DeserializeCustom(nlohmann::json& j) override;

private:
    std::shared_ptr<Cubemap> cubemap;
    SERIALIZEDVAR std::string cubemapPath = "Resources/Textures/Skybox/Hilltop.png";
    SERIALIZEDVAR float brightness = 1.0f;

    friend class Renderer;
};

#undef GENERATED
