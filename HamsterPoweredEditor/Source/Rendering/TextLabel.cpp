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

    m_Color = color;
    m_Text = text;
    m_FontSize = fontSize;
    m_Font = Font::LoadFont(fontPath, fontSize);
    
    SetText(text);
    SetColor(color);
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

    SetRenderSettings({GL_TRIANGLES, true, false, true, false});
}

void TextLabel::SetText(const std::string& text)
{
    m_Text = text;
}

void TextLabel::SetColor(const glm::vec3& color)
{
    m_Color = color;
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
}

void TextLabel::Draw()
{
    Actor::Draw();
    
    m_Shader->Bind();
    m_Shader->SetUniformMat4f("ProjectionMat", m_Projection);
    m_Shader->SetUniform3f("TextColor", m_Color.x, m_Color.y, m_Color.z);

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
        
        m_VBO->SetSubData(*vertices, sizeof(vertices), 0);
        
        FontCharacter.texture->Bind(0);
        m_Shader->SetUniform1i("TextTexture", 0);
        m_Shader->SetUniform1i("u_ScreenSpace", m_ScreenSpace);

        //glm::mat4 transform = m_ScreenSpace ? glm::mat4(1.0f) : m_transform;
        
        Renderer::Submit(m_Shader, m_VAO, m_transform, m_renderSettings);

        CharacterOrigin.x += (FontCharacter.Advance >> 6) * GetScale().x;
    }
}

void TextLabel::OnInspectorGUI()
{
    Actor::OnInspectorGUI();
    if (ImGui::InputText("Text", &m_Text))
    {
        SetText(m_Text);
    }

    ImGui::Checkbox("Screen Space", &m_ScreenSpace);
    
    glm::vec3 color = m_Color;
    if (ImGui::ColorEdit3("Color", &color[0]))
    {
        SetColor(color);
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
