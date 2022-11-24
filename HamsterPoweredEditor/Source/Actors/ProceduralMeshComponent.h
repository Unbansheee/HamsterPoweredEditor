#pragma once
#include <iostream>

#include "MeshComponent.h"

class ProceduralMeshComponent : public MeshComponent
{
public:
    COMPONENT_CONSTRUCTOR(ProceduralMeshComponent, MeshComponent)
    {
        std::cout << "MADE PROCEDURAL MESH COMPONENT" << std::endl;
    }
    
    void SetVertices(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    void SetVertices(const std::vector<Vertex>& vertices);
    void SetIndices(const std::vector<uint32_t>& indices);
    const std::vector<Vertex>& GetVertices() const { return m_Mesh.GetVertices(); }
    const std::vector<uint32_t>& GetIndices() const { return m_Mesh.GetIndices(); }
};
