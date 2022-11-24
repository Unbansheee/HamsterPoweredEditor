#include "RimLitActor.h"

RimLitActor::RimLitActor()
{
    m_mesh.Load("Resources/Meshes/SphereHP.obj");
    m_mesh.SetShader(Shader::Create("Resources/Shaders/VertexColor.vs", "Resources/Shaders/RimLight.fs"));
}
