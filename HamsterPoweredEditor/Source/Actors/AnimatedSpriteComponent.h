#pragma once
#include "SpriteComponent.h"
#include "AnimatedSpriteComponent.generated.h"

SERIALIZEDCLASS(Category = "2D")

class AnimatedSpriteComponent : public SpriteComponent
{
public:
    GENERATED()

    AnimatedSpriteComponent(GameObject* owner)
        : SpriteComponent(owner)
    {
        m_Mesh.SetShader(Shader::Create("Resources/Shaders/VertexColor.vs", "Resources/Shaders/AnimatedTexture.fs"));
    }

    void Render(const glm::mat4& transform) override;

    void Update(Timestep ts) override;
    
    void SetFPS(float fps);
    Texture* SetSpriteSheet(std::string path, int frameCount, int rows, int columns);

    void OnInspectorGUI() override;

    void UpdateSpriteSheetDimensions();
    void DeserializeCustom(nlohmann::json& j) override;
    

private:
    SERIALIZEDVAR float m_fps = 30.f;
    SERIALIZEDVAR int m_rows = 1;
    SERIALIZEDVAR int m_columns = 1;
    SERIALIZEDVAR int m_spriteWidth = 1;
    SERIALIZEDVAR int m_spriteHeight = 1;
    SERIALIZEDVAR int m_currentFrame = 0;
    SERIALIZEDVAR float m_animationTime = 0.f;
    SERIALIZEDVAR int m_frameCount = 1;
    

    float uv_x;
    float uv_y;
};

#undef GENERATED