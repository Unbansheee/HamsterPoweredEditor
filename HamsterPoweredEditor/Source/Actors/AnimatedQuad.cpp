﻿#include "AnimatedQuad.h"

#include "imgui.h"
#include "imgui_stdlib.h"
#include "UI/HPImGui.h"


ImVec2 add(ImVec2 a, ImVec2 b) {
    return ImVec2(a.x + b.x, a.y + b.y);
}

void AnimatedQuad::OnInspectorGUI()
{
    //Quad::OnInspectorGUI();
    if (ImGui::CollapsingHeader("Animation"))
    {
        ImGui::Text("Frame: %d", m_currentFrame);
        ImGui::DragFloat("FPS", &m_fps, 1.f, 1.f);

        ImGui::InputText("SpriteSheet", &m_Path);
        if (ImGui::Button("Load"))
        {
            SetTexture(m_Path);
        }

        
        if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImVec2 imageStart = ImGui::GetCursorScreenPos();
            ImVec2 imageSize = ImVec2(ImGui::GetContentRegionAvail().x * 0.5, (ImGui::GetContentRegionAvail().x*0.5 / texture->GetWidth()) * texture->GetHeight());
            ImVec2 spriteSize = ImVec2(imageSize.x / m_columns, imageSize.y / m_rows);
            
            ImGui::ImageScaledH(texture, 0.5f, false);
        
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            for (int row = 0; row < m_rows; row++)
            {
                for (int col = 0; col < m_columns; col++)
                {
                    int frameNum = row * m_columns + col;

                    
                    
                    //draw a box over every frame
                    ImVec2 frameStart = add(imageStart, ImVec2(col * spriteSize.x, row * spriteSize.y));
                    if (frameNum == m_currentFrame)
                    {
                        drawList->AddRectFilled(frameStart, add(frameStart, spriteSize), IM_COL32(255, 255, 255, 50));
                    }
                        drawList->AddRect(frameStart, add(frameStart, spriteSize), IM_COL32(126, 120, 201, 200));
                }
            }
        }
        
        
        ImGui::DragInt("Columns", &m_columns, 1.f, 1.f);
        ImGui::DragInt("Rows", &m_rows, 1.f, 1.f);
        ImGui::DragInt("Total Frames", &m_frameCount, 1.f, 1.f);

        if (m_columns < 1) m_columns = 1;
        if (m_rows < 1) m_rows = 1;
        if (m_frameCount < 1) m_frameCount = 1;
        
    }
}

AnimatedQuad::AnimatedQuad()
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

    shader.reset(new Shader("Resources/Shaders/VertexColor.vs", "Resources/Shaders/AnimatedTexture.fs"));
    shader->Bind();
    
    texture = (Texture::CreateTexture("Resources/Textures/beetho.png"));
    texture->Bind(0);
    texture->SetFilteringMode(Texture::FilteringMode::NEAREST);

    vb->Unbind();
    va->Unbind();
    ib->Unbind();
    shader->Unbind();

    SetRenderSettings({GL_TRIANGLES, true, true, false, true});
}

AnimatedQuad::~AnimatedQuad()
{
}

void AnimatedQuad::Draw()
{
    Quad::Draw();
}

void AnimatedQuad::Update(Timestep ts)
{
    Quad::Update(ts);
    
    //Run once per frame on the CPU instead of once per pixel on the gpu
    
    // Update the animation time
    m_animationTime += ts.GetSeconds();
    
    if (m_animationTime > 1.f/m_fps)
    {
        m_currentFrame++;
        m_animationTime = 0;
    }
        
    if (m_currentFrame >= m_frameCount)
    {
        m_currentFrame = 0;
        m_animationTime = 0;
    }

    //Find current frame in the texture atlas
    int SpriteX = m_currentFrame % m_columns;
    int SpriteY = (m_rows - 1) - glm::floor(m_currentFrame / m_columns);

    int SpritePositionX = glm::floor(SpriteX * m_spriteWidth);
    int SpritePositionY = glm::floor(SpriteY * m_spriteHeight);

    // normalized texture coordinates
    float uv_x = (float)SpritePositionX / (float)texture->GetWidth();
    float uv_y = (float)SpritePositionY / (float)texture->GetHeight();
    
    shader->Bind();
    shader->SetUniform2f("UVTransform", uv_x, uv_y);
    shader->SetUniform2f("UVScale", m_columns, m_rows);
    shader->Unbind();
    
    
}

void AnimatedQuad::Begin()
{
    Quad::Begin();
}

void AnimatedQuad::OnDestroy()
{
    Quad::OnDestroy();
}

void AnimatedQuad::SetFPS(float fps)
{
    m_fps = fps;
}

Texture* AnimatedQuad::SetSpriteSheet(std::string path, int frameCount, int rows, int columns)
{
    m_Path = path;
    texture = (Texture::CreateTexture(path));
    texture->Bind(0);
    this->m_rows = rows;
    this->m_columns = columns;
    this->m_spriteWidth = texture->GetWidth() / columns;
    this->m_spriteHeight = texture->GetHeight() / rows;
    this->m_frameCount = frameCount;
    return texture;
}
