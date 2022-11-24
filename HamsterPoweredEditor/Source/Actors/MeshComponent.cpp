#include "MeshComponent.h"

void MeshComponent::Update(Timestep ts)
{
    Component::Update(ts);
}

void MeshComponent::Render(const glm::mat4& transform)
{
    Component::Render(transform);
    m_Mesh.Draw(transform);
}
