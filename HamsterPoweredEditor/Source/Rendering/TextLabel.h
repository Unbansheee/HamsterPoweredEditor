#pragma once
#include <map>

#include "GLVertexArray.h"
#include "Actors/Actor.h"
#include "Core/Font.h"
#include "ResourceManagement/Shader.h"

class TextLabel : public Actor
{
public:
    TextLabel(const std::string& text = "Text Label", const std::string& fontPath = "Resources/Fonts/Roboto-Regular.ttf", float fontSize = 100.f, const glm::vec3& color = {0, 1, 1}, const glm::vec3& position = {0, 0, 0});
    void SetText(const std::string& text);
    void SetColor(const glm::vec3& color);
    void SetFont(Font* font);
    Font* GetFont() const;

    void Begin() override;
    void OnDestroy() override;
    void Update(Timestep ts) override;
    void Draw() override;
protected:
    void OnInspectorGUI() override;
public:
    ~TextLabel() override;
protected:
    
    
    std::string m_Text = "Text Label";
    std::string m_FontName;
    int m_FontSize;
    glm::vec3 m_Color;
    std::shared_ptr<GLVertexArray> m_VAO;
    std::shared_ptr<GLVertexBuffer> m_VBO;
    std::shared_ptr<GLIndexBuffer> m_IBO;
    std::shared_ptr<Shader> m_Shader;
    Font* m_Font;
    glm::mat4 m_Projection;
    bool m_ScreenSpace = true;

    bool m_Initialized = false;
    
};
