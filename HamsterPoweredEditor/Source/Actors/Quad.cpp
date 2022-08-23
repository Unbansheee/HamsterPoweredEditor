#include "Quad.h"

#include "Core/App.h"


Quad::Quad()
{
    VertexBufferLayout layout = {
        {ShaderDataType::Float3, "Position"},
        {ShaderDataType::Float3, "Color"},
        {ShaderDataType::Float2, "TexCoord"}
    };


    
    
    vb.reset(new GLVertexBuffer(vertices, sizeof(vertices)));
    va.reset(new GLVertexArray());
    ib.reset(new GLIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));
    
    vb->SetLayout(layout);
    va->AddVertexBuffer(vb);
    va->SetIndexBuffer(ib);
    
    shader.reset(new Shader("Resources/Shaders/VertexColor.vs", "Resources/Shaders/TexturedShader.fs"));
    shader->Bind();

    texture.reset(new Texture("Resources/Textures/beetho.png"));
    texture->Bind(0);
    
    vb->Unbind();
    va->Unbind();
    ib->Unbind();
    shader->Unbind();
}

Quad::~Quad()
{
}

void Quad::Draw()
{
    texture->Bind(0);
    Renderer::Submit(shader, va, m_transform, m_DrawMode);

}

void Quad::Update(Timestep ts)
{
}

void Quad::Begin()
{
}

void Quad::OnDestroy()
{
}

void Quad::SetTexture(const std::string& path)
{
    texture.reset(new Texture(path));
}
