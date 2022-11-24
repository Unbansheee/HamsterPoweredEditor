#include "ProceduralMeshComponent.h"

void ProceduralMeshComponent::SetVertices(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    m_Mesh.Update(vertices, indices);
}

void ProceduralMeshComponent::SetVertices(const std::vector<Vertex>& vertices)
{
    m_Mesh.Update(vertices, m_Mesh.GetIndices());
}

void ProceduralMeshComponent::SetIndices(const std::vector<uint32_t>& indices)
{
    m_Mesh.Update(m_Mesh.GetVertices(), indices);
}
