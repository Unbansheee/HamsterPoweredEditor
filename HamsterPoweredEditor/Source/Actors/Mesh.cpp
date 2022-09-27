#include "Mesh.h"
#include "ResourceManagement/Texture.h"
#include <iostream>

#include "UI/HPImGui.h"

Mesh::Mesh()
{
    
    textures.resize(32, nullptr);
    texturepaths.resize(32, "");
    
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


    textures[0] = Texture::CreateTexture("Resources/Textures/beetho.png");
    textures[0]->Bind(0);
    
    vb->Unbind();
    va->Unbind();
    ib->Unbind();
    shader->Unbind();

    SetRenderSettings({GL_TRIANGLES, false, true, false, true});
    
}

Mesh::~Mesh()
{
}

void Mesh::OnInspectorGUI()
{
    Actor::OnInspectorGUI();
    
    if (ImGui::CollapsingHeader("Textures"))
    {
        for (int i = 0; i < textures.size(); i++)
        {
            ImVec2 cursor = ImGui::GetCursorPos();
            bool textureOpen = ImGui::CollapsingHeader((std::string("Slot ") + std::to_string(i)).c_str());
            if (textures[i])
            {
                ImVec2 dim = ImGui::GetItemRectSize();
                float scalingRation =  dim.y / (float)textures[i]->GetHeight();
                ImVec2 imageSize = {(float)textures[i]->GetWidth() * scalingRation, dim.y};
                ImGui::SameLine(ImGui::GetWindowWidth() - imageSize.x*2);
                ImGui::Image(textures[i], imageSize);
            }
            if (textureOpen)
            {
                ImGui::InputText("Path: ", &texturepaths[i]);
                ImGui::SameLine();
                if (ImGui::Button("Load"))
                {
                    SetTexture(texturepaths[i], i);
                }
                if (textures[i])
                {
                    ImGui::ImageScaledH(textures[i], 0.25f);
                }
            }
        }
    }
    
}

void Mesh::Draw()
{
    
    Actor::Draw();
    textures[0]->Bind(0);
    Renderer::Submit(shader, va, m_transform);
    
    
}

void Mesh::Update(Timestep ts)
{
    Actor::Update(ts);
}

void Mesh::Begin()
{
}

void Mesh::OnDestroy()
{
}

Texture* Mesh::SetTexture(const std::string& path, int slot)
{
    //check if slot is valid
    if (slot < 0 || slot > 31)
    {
        std::cout << "Texture slot must be between 0 and 31" << std::endl;
        return nullptr;
    }
    
    textures[slot] = (Texture::CreateTexture(path));
    texturepaths[slot] = path;
    return textures[slot];
}

void Mesh::SetMesh(float* _vertices, uint32_t _size, uint32_t* _indices, uint32_t _isize)
{
    vb.reset(new GLVertexBuffer(_vertices, _size));
    ib.reset(new GLIndexBuffer(_indices, _isize));
}

