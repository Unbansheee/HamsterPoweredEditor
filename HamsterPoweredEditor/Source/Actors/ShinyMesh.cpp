#include "ShinyMesh.h"

ShinyMesh::ShinyMesh()
{
    m_mesh.Load("Resources/Meshes/MonkeySmooth.obj");
    m_mesh.SetShader(Shader::Create("Resources/Shaders/VertexColor.vs", "Resources/Shaders/Shiny.fs"));
    m_mesh.SetTexture("Resources/Textures/MonkeySpecular.png", 1);
    m_mesh.SetTexture("Resources/Textures/Brick.jpg", 2);
}

void ShinyMesh::Begin()
{
    DynamicMeshActor::Begin();
}

void ShinyMesh::Draw()
{
    //DynamicMeshActor::Draw();
    m_mesh.Draw(m_transform, [&](std::shared_ptr<Shader> shader)
    {
        //shader->SetUniform1i("u_ReflectionMap", m_lightPos);
    });
    
}
