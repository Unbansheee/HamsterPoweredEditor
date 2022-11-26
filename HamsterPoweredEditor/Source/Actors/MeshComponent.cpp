#include "MeshComponent.h"

#include "UI/HPImGui.h"

void MeshComponent::Update(Timestep ts)
{
    Component::Update(ts);
}

void MeshComponent::Render(const glm::mat4& transform)
{
    Component::Render(transform);
    m_Mesh.Draw(transform);
}

void MeshComponent::OnInspectorGUI()
{
    Component::OnInspectorGUI();
    std::string path = m_Mesh.GetPath();
    if (ImGui::OpenFilePath("Mesh", path, "Open File", "Image File (*.obj;*.fbx;){.obj,.fbx},.*", "Resources/Meshes"))
    {
        m_Mesh.Load(path);
    }
}
