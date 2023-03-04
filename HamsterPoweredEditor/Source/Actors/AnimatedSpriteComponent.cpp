#include "UI/HPImGui.h"
#include "AnimatedSpriteComponent.h"
#include "ResourceManagement/Texture.h"

void AnimatedSpriteComponent::Render(const glm::mat4& transform)
{
    SpriteComponent::Render(transform);
    m_Mesh.Draw(transform, [&](std::shared_ptr<Shader> shader) {
        shader->SetUniform2f("UVTransform", uv_x, uv_y);
        shader->SetUniform2f("UVScale", (float)m_columns, (float)m_rows);
    });
}

void AnimatedSpriteComponent::Update(Timestep ts)
{

    SpriteComponent::Update(ts);

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
    int SpriteY = (m_rows - 1) - (int)glm::floor(m_currentFrame / m_columns);

    int SpritePositionX = (int)glm::floor(SpriteX * m_spriteWidth);
    int SpritePositionY = (int)glm::floor(SpriteY * m_spriteHeight);

    // normalized texture coordinates
    if (!m_Mesh.GetTexture()) return;
    uv_x = (float)SpritePositionX / (float)m_Mesh.GetTexture()->GetWidth();
    uv_y = (float)SpritePositionY / (float)m_Mesh.GetTexture()->GetHeight();
    
}

void AnimatedSpriteComponent::SetFPS(float fps)
{
    m_fps = fps;
}

Texture* AnimatedSpriteComponent::SetSpriteSheet(std::string path, int frameCount, int rows, int columns)
{
    m_TexturePath = path;
    auto texture = Texture::CreateTexture(path);
    m_Mesh.SetTexture(texture);
    texture->Bind(0);
    this->m_rows = rows;
    this->m_columns = columns;
    this->m_spriteWidth = texture->GetWidth() / columns;
    this->m_spriteHeight = texture->GetHeight() / rows;
    this->m_frameCount = frameCount;
    return texture;
}

void AnimatedSpriteComponent::OnInspectorGUI()
{
    //SpriteComponent::OnInspectorGUI();
    Component::OnInspectorGUI();

    if (ImGui::OpenFilePath("Spritesheet", m_TexturePath, "Load Spritesheet", "Image File (*.png;*.jpg;*.jpeg){.png,.jpg,.jpeg},.*", "Resources/Textures"))
    {
        m_Mesh.SetTexture(m_TexturePath);
        UpdateSpriteSheetDimensions();
    }
    ImGui::TextureFilteringSelector("Filtering Mode", m_Mesh.GetTexture(0));
    
    if (ImGui::CollapsingHeader("Animation"))
    {
        ImGui::Text("Frame: %d", m_currentFrame);
        ImGui::DragFloat("FPS", &m_fps, 1.f, 1.f);
        
        
        if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
        {
            
            ImVec2 imageStart = ImGui::GetCursorScreenPos();
            ImVec2 imageSize = ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, (ImGui::GetContentRegionAvail().x * 0.5f / (float)m_Mesh.GetTexture(0)->GetWidth()) * (float)m_Mesh.GetTexture(0)->GetHeight());
            ImVec2 spriteSize = ImVec2(imageSize.x / (float)m_columns, imageSize.y / (float)m_rows);

            
            ImGui::ImageScaledH(m_Mesh.GetTexture(0), 0.5f, false);
        
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            for (int row = 0; row < m_rows; row++)
            {
                for (int col = 0; col < m_columns; col++)
                {
                    int frameNum = row * m_columns + col;

                    
                    
                    //draw a box over every frame
                    ImVec2 frameStart = ImGui::Add(imageStart, ImVec2(col * spriteSize.x, row * spriteSize.y));
                    if (frameNum == m_currentFrame)
                    {
                        drawList->AddRectFilled(frameStart, ImGui::Add(frameStart, spriteSize), IM_COL32(255, 255, 255, 50));
                    }
                        drawList->AddRect(frameStart, ImGui::Add(frameStart, spriteSize), IM_COL32(126, 120, 201, 200));
                }
            }
        }
        
        
        if (ImGui::DragInt("Columns", &m_columns, 1.f, 1))
        {
            UpdateSpriteSheetDimensions();
        }
        if (ImGui::DragInt("Rows", &m_rows, 1.f, 1))
        {
            UpdateSpriteSheetDimensions();
        }
        if (ImGui::DragInt("Total Frames", &m_frameCount, 1.f, 1))
        {
            UpdateSpriteSheetDimensions();
        }

        if (m_columns < 1) m_columns = 1;
        if (m_rows < 1) m_rows = 1;
        if (m_frameCount < 1) m_frameCount = 1;
        
    }
}

void AnimatedSpriteComponent::UpdateSpriteSheetDimensions()
{
    auto tex = m_Mesh.GetTexture();
    this->m_spriteWidth = tex->GetWidth() / m_columns;
    this->m_spriteHeight = tex->GetHeight() / m_rows;
}

void AnimatedSpriteComponent::DeserializeCustom(nlohmann::json& j)
{
    SpriteComponent::DeserializeCustom(j);
    UpdateSpriteSheetDimensions();
}
