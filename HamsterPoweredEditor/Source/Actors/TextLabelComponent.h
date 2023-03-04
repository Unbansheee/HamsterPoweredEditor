#pragma once
#include "Component.h"
#include "Core/Font.h"
#include "TextLabelComponent.generated.h"

struct Bounds
{
    float x;
    float y;
    float width;
    float height;
};

SERIALIZEDCLASS(Category = "2D")

class TextLabelComponent : public Component
{

    GENERATED()
    
public:
    TextLabelComponent(GameObject* owner);
    void Begin() override;
    void Update(Timestep ts) override;
    void Render(const glm::mat4& transform) override;
    void OnInspectorGUI() override;
    void SerializeCustom(nlohmann::json& j) override;
    void DeserializeCustom(nlohmann::json& j) override;

    void UpdateBuffers();

    void SetText(const std::string& text);
    void SetCurrentColor(const glm::vec3& color);
    void SetBaseColor(const glm::vec3& color);
    void SetHoverColor(const glm::vec3& color);
    void SetClickColor(const glm::vec3& color);
    void SetFont(Font* font);
    Font* GetFont() const;

protected:
    SERIALIZEDVAR std::string m_Text = "Text Label";
    std::string m_FontName;
    SERIALIZEDVAR int m_FontSize = 100;
    SERIALIZEDVAR glm::vec3 m_Color;
    SERIALIZEDVAR glm::vec3 m_HoverColor = {0, 1, 0};
    SERIALIZEDVAR glm::vec3 m_PressedColor = {0, 0, 1};
    SERIALIZEDVAR glm::vec3 m_BaseColor = {1, 1, 1};
    bool mouseOverlapping = false;
    std::shared_ptr<GLVertexArray> m_VAO;
    std::shared_ptr<GLVertexBuffer> m_VBO;
    std::shared_ptr<GLIndexBuffer> m_IBO;
    std::shared_ptr<Shader> m_Shader;
    Font* m_Font;
    SERIALIZEDVAR std::string m_FontPath = "Resources/Fonts/Roboto-Regular.ttf";
    glm::mat4 m_Projection;
    glm::mat4 m_View;
    SERIALIZEDVAR bool m_ScreenSpace = true;
    glm::vec2 mouseoffset = {0, 0};
    bool m_Dragging = false;
    bool m_Initialized = false;
    Bounds m_bounds;
    Bounds m_unscaledBounds;
    std::vector<Texture*> textures;

    SERIALIZEDVAR bool m_scaleBounce = false;
    SERIALIZEDVAR float m_scaleBounceDelta = 0.0f;
    SERIALIZEDVAR bool m_sideScroll = false;
    SERIALIZEDVAR glm::vec2 m_scrollSpeed = {0, 0};
    RenderSettings m_RenderSettings = {GL_TRIANGLES, true, true, false, true};
    
};

#undef GENERATED