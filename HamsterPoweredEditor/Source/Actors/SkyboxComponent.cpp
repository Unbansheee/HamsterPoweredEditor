#include "SkyboxComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "Core/Scene.h"
#include "UI/HPImGui.h"

SkyboxComponent::SkyboxComponent(GameObject* owner) : MeshComponent(owner)
{
    cubemap.reset(new Cubemap(cubemapPath));
    Owner->GetComponent<TransformComponent>()->SetWorldScale(glm::vec3(100000.f));
    m_Mesh.Cube();
    m_Mesh.SetShader(Shader::Create("Resources/Shaders/Skybox.vs", "Resources/Shaders/Skybox.fs"));
}

SkyboxComponent::~SkyboxComponent()
{
    Renderer::RemoveSkybox(this);
}

void SkyboxComponent::SerializeCustom(nlohmann::json& j)
{
    MeshComponent::SerializeCustom(j);
}

void SkyboxComponent::Update(Timestep ts)
{
    MeshComponent::Update(ts);
}

void SkyboxComponent::Render(const glm::mat4& transform)
{
    //MeshComponent::Render(transform);

    Renderer::SubmitSkybox(this);
    
}

void SkyboxComponent::OnInspectorGUI()
{
    //MeshComponent::OnInspectorGUI();
    if (ImGui::OpenFilePath("Path##Cubemap", cubemapPath, "Load Texture", "Image File (*.png;*.jpg;*.jpeg){.png,.jpg,.jpeg},.*", "Resources/Textures/Skybox"))
    {
        cubemap.reset(new Cubemap(cubemapPath));
    }
    ImGui::DragFloat("Brightness", &brightness, 0.01f, 0.0f, 10.0f);
}

void SkyboxComponent::DeserializeCustom(nlohmann::json& j)
{
    MeshComponent::DeserializeCustom(j);
    cubemap.reset(new Cubemap(cubemapPath));
    m_Mesh.Cube();
    m_Mesh.SetShader(Shader::Create("Resources/Shaders/Skybox.vs", "Resources/Shaders/Skybox.fs"));
}
