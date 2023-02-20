#include "PointLight.h"


PointLight::PointLight()
{
    m_Mesh.Load("Resources/Meshes/Sphere.obj");
    m_Mesh.SetShader(Shader::Create("Resources/Shaders/VertexColor.vs", "Resources/Shaders/Unlit.fs"));
}

void PointLight::Update(Timestep ts)
{
    Actor::Update(ts);
    m_Light.position = m_transform[3];
}

void PointLight::Draw()
{
    Actor::Draw();
    Renderer::Submit(m_Light);
    m_Mesh.Draw(m_transform, [&](std::shared_ptr<Shader> shader)
    {
        shader->SetUniform3f("u_Color", m_Light.color.x, m_Light.color.y, m_Light.color.z);
    });
    
}

void PointLight::Begin()
{
    Actor::Begin();
}

void PointLight::OnDestroy()
{
    Actor::OnDestroy();
}

nlohmann::json PointLight::Serialize()
{
    auto j = Actor::Serialize();
    //j["Color"] = m_Light.color;
    j["Intensity"] = m_Light.intensity;
    j["Radius"] = m_Light.radius;
    return j;
}

void PointLight::Deserialize(nlohmann::json& j)
{
    Actor::Deserialize(j);
    //m_Light.color = j["Color"];
    m_Light.intensity = j["Intensity"];
    m_Light.radius = j["Radius"];
    
}

void PointLight::SetColor(const glm::vec3& color)
{
    m_Light.color = color;
}

void PointLight::SetIntensity(float intensity)
{
    m_Light.intensity = intensity;
}

void PointLight::SetRadius(float radius)
{
    m_Light.radius = radius;
}

glm::vec3 PointLight::GetColor() const
{
    return m_Light.color;
}

float PointLight::GetIntensity() const
{
    return m_Light.intensity;
}

float PointLight::GetRadius() const
{
    return m_Light.radius;
}

void PointLight::OnInspectorGUI()
{
    Actor::OnInspectorGUI();
    ImGui::ColorEdit3("Color", &m_Light.color[0]);
    ImGui::DragFloat("Intensity", &m_Light.intensity, 0.1f, 0.0f, 100.0f);
    ImGui::DragFloat("Radius", &m_Light.radius, 0.1f, 0.0f, 100.0f);
    
}
