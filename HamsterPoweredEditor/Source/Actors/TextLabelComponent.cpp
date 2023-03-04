#include "TextLabelComponent.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "TransformComponent.h"
#include "Core/App.h"
#include "Core/Input.h"
#include "Core/Window.h"
#include "UI/HPImGui.h"

unsigned int indices[3 * 2] = {
    0, 1, 2,   // first triangle
    0, 2, 3    // second triangle
};  

TextLabelComponent::TextLabelComponent(GameObject* owner) : Component(owner)
{
    VertexBufferLayout layout = {
        {ShaderDataType::Float3, "Position"},
        {ShaderDataType::Float3, "Color"},
        {ShaderDataType::Float2, "TexCoord"},
        {ShaderDataType::Float, "TextureID"}
    };


    m_Projection = glm::ortho(0.0f, (float)App::Instance().window->GetWidth(), 0.0f, (float)App::Instance().window->GetHeight());
    m_Shader = (Shader::Create("Resources/Shaders/Text.vs", "Resources/Shaders/Text.fs"));

    m_VBO.reset(new GLVertexBuffer(nullptr, sizeof(float) * 9 * 4 * 1000, GL_DYNAMIC_DRAW));
    m_VAO.reset(new GLVertexArray());
    m_IBO.reset(new GLIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t) * 256, GL_DYNAMIC_DRAW));
    
    m_VBO->SetLayout(layout);
    m_VAO->AddVertexBuffer(m_VBO);
    m_VAO->SetIndexBuffer(m_IBO);
    
    
    m_Initialized = true;
    

    m_VAO->Unbind();
    m_VBO->Unbind();
    m_IBO->Unbind();
}

void TextLabelComponent::Begin()
{
    Component::Begin();
    m_Font = Font::LoadFont(m_FontPath, m_FontSize);
}

void TextLabelComponent::Update(Timestep ts)
{
    Component::Update(ts);

    glm::vec2 MousePosAbsolute = Input::GetMousePositionAbsolute();
    glm::vec2 viewportSize = Renderer::GetViewportSize();
    glm::vec2 viewportLocationAbsolute = Renderer::GetViewportPosition();
    
    glm::vec2 mousePosInViewport = MousePosAbsolute - viewportLocationAbsolute;
    mousePosInViewport.y = viewportSize.y - mousePosInViewport.y;
    
    
    
    
    
    //Check if mouse intersects bounds
    if (mousePosInViewport.x > m_bounds.x && mousePosInViewport.x < m_bounds.x + m_bounds.width &&
        mousePosInViewport.y > m_bounds.y && mousePosInViewport.y < m_bounds.y + m_bounds.height)
    {
        mouseOverlapping = true;
        SetCurrentColor(m_HoverColor);
        
        if (Input::WasMouseButtonPressed(Mouse::Left))
        {
            mouseoffset = mousePosInViewport - glm::vec2(m_bounds.x, m_bounds.y);
            m_Dragging = true;
        }

        
    }
    else if (mouseOverlapping)
    {
        SetCurrentColor(m_BaseColor);
        mouseOverlapping = false;
    }
    else SetCurrentColor(m_BaseColor);
    
    auto transform = Owner->GetComponent<TransformComponent>();
    
    if (m_Dragging)
    {
        transform->SetLocalPosition({ mousePosInViewport.x - mouseoffset.x, mousePosInViewport.y - mouseoffset.y, 0.0f });
        SetCurrentColor(m_PressedColor);
    }
    if (!Input::IsMouseButtonDown(Mouse::Left) && m_Dragging)
    {
        m_Dragging = false;
        SetCurrentColor(m_BaseColor);
    }
    
    m_scaleBounceDelta = (sin(App::Instance().GetTime() * 3.f) + 2.0f) * 0.5f;

    if (m_sideScroll)
    {
        // Scrolling text
        transform->SetLocalPosition({ transform->GetLocalPosition().x + m_scrollSpeed.x * ts.GetSeconds(), transform->GetLocalPosition().y + m_scrollSpeed.y * ts.GetSeconds(), transform->GetLocalPosition().z });
        if (transform->GetLocalPosition().y > Renderer::GetViewportSize().y)
            transform->SetLocalPosition({ transform->GetLocalPosition().x, -m_bounds.height, transform->GetLocalPosition().z });
        if (transform->GetLocalPosition().x > Renderer::GetViewportSize().x)
            transform->SetLocalPosition({ -m_bounds.width, transform->GetLocalPosition().y, transform->GetLocalPosition().z });
    }
}

void TextLabelComponent::Render(const glm::mat4& transform)
{
    Component::Render(transform);

    auto transformComponent = Owner->GetComponent<TransformComponent>();
    
    m_Projection = glm::ortho(0.0f, Renderer::GetViewportSize().x, 0.0f, Renderer::GetViewportSize().y);
    m_Shader->Bind();
    m_Shader->SetUniformMat4f("ProjectionMat", m_Projection);

    m_bounds.x = transformComponent->GetLocalPosition().x;
    m_bounds.y = transformComponent->GetLocalPosition().y;
    m_bounds.height = m_unscaledBounds.height * transformComponent->GetLocalScale().y;
    m_bounds.width = m_unscaledBounds.width * transformComponent->GetLocalScale().x;

    glm::mat4 scaleTransform = transformComponent->GetWorldTransform();
    if (!m_Dragging && mouseOverlapping && m_scaleBounce)
    {

        scaleTransform = glm::translate(scaleTransform, glm::vec3(m_unscaledBounds.width / 2.f, m_unscaledBounds.height / 2.f, 0.0f));     // Translate to center
        scaleTransform = glm::scale(scaleTransform, m_scaleBounceDelta * glm::vec3(1, 1, 0));                                              // Scale 
        scaleTransform = glm::translate(scaleTransform, glm::vec3(-m_unscaledBounds.width / 2.f, -m_unscaledBounds.height / 2.f, 0.0f));   // Translate back to original position
        
    }
    m_Shader->Unbind();
    
    Renderer::Submit(m_Shader, m_VAO, scaleTransform, textures, m_RenderSettings);
}

void TextLabelComponent::OnInspectorGUI()
{
    Component::OnInspectorGUI();
    if (ImGui::InputText("Text", &m_Text))
    {
        SetText(m_Text);
    }

    ImGui::Checkbox("Bounce", &m_scaleBounce);
    ImGui::Checkbox("Scroll", &m_sideScroll);
    ImGui::DragFloat2("Scroll Speed", &m_scrollSpeed.x, 1.f, 0.0f, 10000.0f);
    
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

void TextLabelComponent::SerializeCustom(nlohmann::json& j)
{
    Component::SerializeCustom(j);
}

void TextLabelComponent::DeserializeCustom(nlohmann::json& j)
{
    Component::DeserializeCustom(j);
}

void TextLabelComponent::UpdateBuffers()
{
    float maxwidth = 0.f, maxheight = 0.f;
    glm::vec3 CharacterOrigin = {0, 0, 0};
    textures.clear();

    m_IBO->Bind();
    m_VBO->Bind();
    
    int index = 0;
    for (std::string::const_iterator TextCharacter = m_Text.begin(); TextCharacter != m_Text.end(); TextCharacter++)
    {
        Font::FontChar FontCharacter = m_Font->GetChar(*TextCharacter);
        float PosX = CharacterOrigin.x + FontCharacter.Bearing.x;
        float PosY = CharacterOrigin.y - (FontCharacter.texture->GetHeight() - FontCharacter.Bearing.y);
        float Width = (float)FontCharacter.texture->GetWidth();
        float Height = (float)FontCharacter.texture->GetHeight();

        int texindex = 0;
        for (int i = 0; i < textures.size(); i++)
        {
            if (textures[i] == FontCharacter.texture)
            {
                texindex = i;
                break;
            }
        }
        if (texindex == 0)
        {
            textures.push_back(FontCharacter.texture);
            texindex = (int)textures.size() - 1;
        }
        
        float vertices[4][9] = {
            {PosX, PosY + Height, 0.0f, m_Color.x, m_Color.y, m_Color.z, 0.0f, 0.0f, (float)texindex},
            {PosX, PosY, 0.0f, m_Color.x, m_Color.y, m_Color.z,  0.0f, 1.0f, (float)texindex},
            {PosX + Width, PosY, 0.0f, m_Color.x, m_Color.y, m_Color.z, 1.0f, 1.0f, (float)texindex},
            {PosX + Width, PosY + Height, 0.0f, m_Color.x, m_Color.y, m_Color.z, 1.0f, 0.0f, (float)texindex}
        };

        // offset indices
        uint32_t _indices[6];
        for (int i = 0; i < 6; i++)
        {
            _indices[i] = indices[i] + index * 4;
        }
        
        m_VBO->UpdateData(*vertices, sizeof(vertices), index * 4 * sizeof(float) * 9);
        m_IBO->UpdateData(_indices, sizeof(_indices) / sizeof(uint32_t), index * 6 * sizeof(uint32_t));


        CharacterOrigin.x += (FontCharacter.Advance >> 6);

        maxheight = std::max(m_bounds.height, Height);
        maxwidth += (FontCharacter.Advance >> 6);
        index++;
    }
    //m_VBO->Unbind();
   // m_IBO->Unbind();
    
    
    m_unscaledBounds.height = maxheight;
    m_unscaledBounds.width = maxwidth;
}

void TextLabelComponent::SetText(const std::string& text)
{
    m_Text = text;
    UpdateBuffers();
}

void TextLabelComponent::SetCurrentColor(const glm::vec3& color)
{
    m_Color = color;
    UpdateBuffers();
}

void TextLabelComponent::SetBaseColor(const glm::vec3& color)
{
    m_BaseColor = color;
}

void TextLabelComponent::SetHoverColor(const glm::vec3& color)
{
    m_HoverColor = color;
}

void TextLabelComponent::SetClickColor(const glm::vec3& color)
{
    m_PressedColor = color;
}

void TextLabelComponent::SetFont(Font* font)
{
    m_Font = font;
    UpdateBuffers();

}
