#include "Mesh.h"
#include "ResourceManagement/Texture.h"
#include <iostream>
#include <assimp/Importer.hpp>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "UI/HPImGui.h"
#include "assimp/postprocess.h"

Mesh::Mesh()
{
    textures.resize(32, nullptr);
    texturepaths.resize(32, "");
    
    VertexBufferLayout layout = {
        {ShaderDataType::Float3, "Position"},
        {ShaderDataType::Float3, "Color"},
        {ShaderDataType::Float2, "TexCoord"}
    };
    
    vb.reset(new GLVertexBuffer(vertices.data(), vertices.size() * sizeof(float)));
    va.reset(new GLVertexArray());
    ib.reset(new GLIndexBuffer(indices.data(), indices.size()));
    
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

    ImGui::InputText("Mesh", &meshpath);
    ImGui::SameLine();
    if (ImGui::Button("Load##Mesh"))
    {
        LoadMesh(meshpath);
    }
    
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
                ImGui::InputText((std::string("Path##") + std::to_string(i)).c_str(), &texturepaths[i]);
                ImGui::SameLine();
                if (ImGui::Button((std::string("Load##") + std::to_string(i)).c_str()))
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
    textures[slot]->Bind(slot);
    textures[slot] = (Texture::CreateTexture(path));
    texturepaths[slot] = path;
    textures[slot]->Unbind();
    return textures[slot];
}

void Mesh::SetMesh(float* _vertices, uint32_t _size, uint32_t* _indices, uint32_t _isize)
{
    vb.reset(new GLVertexBuffer(_vertices, _size));
    ib.reset(new GLIndexBuffer(_indices, _isize));
}

void Mesh::LoadMesh(const std::string& path)
{
    
    Assimp::Importer importer;
    scene = *importer.ReadFile(path, aiProcess_Triangulate);


    if (!scene.mMeshes[0])
    {
        std::cout << "Error loading mesh" << std::endl;
        return;
    }

    meshpath = path;


    

    vertices.clear();
    indices.clear();
    
    for (int index = 0; index < sizeof(scene.mMeshes) / sizeof(scene.mMeshes[0]); index++)
    {
        aiMesh* mesh = scene.mMeshes[index];
        for (int i = 0; i < mesh->mNumVertices; i++)
        {
            vertices.push_back(mesh->mVertices[i].x);
            vertices.push_back(mesh->mVertices[i].y);
            vertices.push_back(mesh->mVertices[i].z);
            vertices.push_back(1.0f);
            vertices.push_back(1.0f);
            vertices.push_back(1.0f);
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }

        for (int i = 0; i < mesh->mNumFaces; i++)
        {
            indices.push_back(mesh->mFaces[i].mIndices[0]);
            indices.push_back(mesh->mFaces[i].mIndices[1]);
            indices.push_back(mesh->mFaces[i].mIndices[2]);
        }
    }
    VertexBufferLayout layout = {
        {ShaderDataType::Float3, "Position"},
        {ShaderDataType::Float3, "Color"},
        {ShaderDataType::Float2, "TexCoord"}
    };


    vb.reset(new GLVertexBuffer(vertices.data(), vertices.size() * sizeof(float)));
    va.reset(new GLVertexArray());
    ib.reset(new GLIndexBuffer(indices.data(), indices.size()));
    
    vb->SetLayout(layout);
    va->AddVertexBuffer(vb);
    va->SetIndexBuffer(ib);
    
    
    vb->Unbind();
    va->Unbind();
    ib->Unbind();
    shader->Unbind();
    
    importer.FreeScene();
    
}

