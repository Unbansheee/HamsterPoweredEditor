#include "SpriteComponent.h"

#include "UI/HPImGui.h"

void SpriteComponent::Update(Timestep ts)
{
    Component::Update(ts);
}

void SpriteComponent::Render(const glm::mat4& transform)
{
    Component::Render(transform);
    m_Mesh.Draw(transform);
}

void SpriteComponent::OnInspectorGUI()
{
    Component::OnInspectorGUI();
    std::string path = m_TexturePath;
    if (ImGui::OpenFilePath("Texture", path, "Open File", "Image File (*.png;*.jpg;){.png,.jpg},.*", "Resources/Textures"))
    {
        m_Mesh.SetTexture(path);
        m_TexturePath = path;
        
    }
}
