#include "TextLabel.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "Core/App.h"
#include "Core/Window.h"
#include "UI/HPImGui.h"

unsigned int indices[3 * 2] = {
    0, 1, 2,   // first triangle
    0, 2, 3    // second triangle
};  

TextLabel::TextLabel(const std::string& text, const std::string& fontPath, float fontSize, const glm::vec3& color,
                     const glm::vec3& position)
{
    VertexBufferLayout layout = {
        {ShaderDataType::Float3, "Position"},
        {ShaderDataType::Float2, "TexCoord"}
    };

    m_BaseColor = color;
    m_Text = text;
    m_FontSize = fontSize;
    m_FontPath = fontPath;
    m_Font = Font::LoadFont(fontPath, fontSize);
    
    SetText(text);
    SetCurrentColor(color);
    SetPosition(position);

    m_Projection = glm::ortho(0.0f, (float)App::Instance().window->GetWidth(), 0.0f, (float)App::Instance().window->GetHeight());
    m_Shader.reset(new Shader("Resources/Shaders/Text.vs", "Resources/Shaders/Text.fs"));

    m_VBO.reset(new GLVertexBuffer(nullptr, sizeof(float) * 6 * 4, GL_DYNAMIC_DRAW));
    m_VAO.reset(new GLVertexArray());
    m_IBO.reset(new GLIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t), GL_DYNAMIC_DRAW));
    
    m_VBO->SetLayout(layout);
    m_VAO->AddVertexBuffer(m_VBO);
    m_VAO->SetIndexBuffer(m_IBO);
    

    m_Initialized = true;

    SetRenderSettings({GL_TRIANGLES, true, false, false, false});
}

void TextLabel::SetText(const std::string& text)
{
    m_Text = text;
}

void TextLabel::SetCurrentColor(const glm::vec3& color)
{
    m_Color = color;
}

void TextLabel::SetBaseColor(const glm::vec3& color)
{
    m_BaseColor = color;
}

void TextLabel::SetHoverColor(const glm::vec3& color)
{
    m_HoverColor = color;
}

void TextLabel::SetClickColor(const glm::vec3& color)
{
    m_PressedColor = color;
}

void TextLabel::SetFont(Font* font)
{
    m_Font = font;
}

Font* TextLabel::GetFont() const
{
    return m_Font;
}

void TextLabel::Begin()
{
}

void TextLabel::OnDestroy()
{
}

void TextLabel::Update(Timestep ts)
{
    Actor::Update(ts);

    glm::vec2 mousePosGLM = App::Instance().window->GetMousePosition();
    mousePosGLM.y = App::Instance().window->GetHeight() - mousePosGLM.y;
    mousePosGLM.y -= 20;
    
    
    //Check if mouse intersects bounds
    if (mousePosGLM.x > m_bounds.x && mousePosGLM.x < m_bounds.x + m_bounds.width &&
        mousePosGLM.y > m_bounds.y && mousePosGLM.y < m_bounds.y + m_bounds.height)
    {
        mouseOverlapping = true;
        SetCurrentColor(m_HoverColor);
        
        if (ImGui::IsMouseClicked(0))
        {
            mouseoffset = mousePosGLM - glm::vec2(m_bounds.x, m_bounds.y);
            m_Dragging = true;
        }
        
    }
    else if (mouseOverlapping)
    {
        SetCurrentColor(m_BaseColor);
        mouseOverlapping = false;
    }
    else SetCurrentColor(m_BaseColor);
    
    
    
    if (m_Dragging)
    {
        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {SetScale(GetScale() * 1.1f); mouseoffset *= 1.1f; }
        if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {SetScale(GetScale() * 0.9f); mouseoffset *= 0.9f; }
        SetPosition({ mousePosGLM.x - mouseoffset.x, mousePosGLM.y - mouseoffset.y, 0.0f });
        SetCurrentColor(m_PressedColor);
    }
    if (!ImGui::IsMouseDown(0) && m_Dragging)
    {
        m_Dragging = false;
        SetCurrentColor(m_BaseColor);
    }
    
}

void TextLabel::Draw()
{
    Actor::Draw();
    m_VBO->Bind();
    m_Projection = glm::ortho(0.0f, Renderer::GetViewportSize().x, 0.0f, Renderer::GetViewportSize().y);
    m_Shader->Bind();
    m_Shader->SetUniformMat4f("ProjectionMat", m_Projection);
    m_Shader->SetUniform3f("TextColor", m_Color.x, m_Color.y, m_Color.z);
    
    Bounds bounds = {0, 0, 0, 0};

    bounds.x = GetPosition().x;
    bounds.y = GetPosition().y;
    glm::vec3 CharacterOrigin = GetPosition();
    
    for (std::string::const_iterator TextCharacter = m_Text.begin(); TextCharacter != m_Text.end(); TextCharacter++)
    {
        Font::FontChar FontCharacter = m_Font->GetChar(*TextCharacter);
        float PosX = CharacterOrigin.x + FontCharacter.Bearing.x * GetScale().x;
        float PosY = CharacterOrigin.y - (FontCharacter.texture->GetHeight() - FontCharacter.Bearing.y) * GetScale().y;
        float Width = FontCharacter.texture->GetWidth() * GetScale().x;
        float Height = FontCharacter.texture->GetHeight() * GetScale().y;
        
        float vertices[4][5] = {
            {PosX, PosY + Height, 0.0f, 0.0f, 0.0f},
            {PosX, PosY, 0.0f, 0.0f, 1.0f},
            {PosX + Width, PosY, 0.0f, 1.0f, 1.0f},
            {PosX + Width, PosY + Height, 0.0f, 1.0f, 0.0f}
        };

        //zero out translation
        glm::mat4 model = glm::mat4(1.0f);
        model[3][0] = 0.0f;
        model[3][1] = 0.0f;
        model[3][2] = 0.0f;
        
        m_VBO->SetSubData(*vertices, sizeof(vertices), 0);
        
        FontCharacter.texture->Bind(0);
        m_Shader->SetUniform1i("TextTexture", 0);
        m_Shader->SetUniform1i("u_ScreenSpace", m_ScreenSpace);
        
        Renderer::Submit(m_Shader, m_VAO, model, {FontCharacter.texture}, m_renderSettings);

        CharacterOrigin.x += (FontCharacter.Advance >> 6) * GetScale().x;

        bounds.height = std::max(bounds.height, Height);
        bounds.width += (FontCharacter.Advance >> 6) * GetScale().x;

    }
    

    m_bounds = bounds;
    m_VBO->Unbind();
}

void TextLabel::OnInspectorGUI()
{
    Actor::OnInspectorGUI();
    if (ImGui::InputText("Text", &m_Text))
    {
        SetText(m_Text);
    }
    
    ImGui::Checkbox("Screen Space", &m_ScreenSpace);
    
    glm::vec3 color = m_BaseColor;
    if (ImGui::ColorEdit3("Base Color", &color[0]))
    {
        SetBaseColor(color);
    }

    glm::vec3 hovercolor = m_HoverColor;
    if (ImGui::ColorEdit3("Hover Color", &hovercolor[0]))
    {
        SetHoverColor(hovercolor);
    }

    glm::vec3 pressedcolor = m_PressedColor;
    if (ImGui::ColorEdit3("Pressed Color", &pressedcolor[0]))
    {
        SetClickColor(pressedcolor);
    }

    Font* font = m_Font;
    if (ImGui::FontSelector("Font", &font))
    {
        SetFont(font);
    }
    
}

TextLabel::~TextLabel()
{
}

nlohmann::json TextLabel::Serialize()
{
    auto json = Actor::Serialize();
    json["Text"] = m_Text;
    json["FontPath"] = m_FontPath;
    json["ScreenSpace"] = m_ScreenSpace;
    json["BaseColor"] = m_BaseColor;
    json["HoverColor"] = m_HoverColor;
    json["PressedColor"] = m_PressedColor;
    return json;
}

void TextLabel::Deserialize(nlohmann::json& j)
{
    Actor::Deserialize(j);
    m_Text = j["Text"];
    SetText(m_Text);
    m_FontPath = j["FontPath"];
    SetFont(Font::LoadFont(m_FontPath, 100));
    m_ScreenSpace = j["ScreenSpace"];
    m_BaseColor = j["BaseColor"];
    m_HoverColor = j["HoverColor"];
    m_PressedColor = j["PressedColor"];
    
}
