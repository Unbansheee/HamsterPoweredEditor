#include "Renderer.h"

#include "Core/App.h"
#include "Core/Timer.h"
#include "ResourceManagement/Texture.h"
#include "ResourceManagement/Shader.h"

Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

void Renderer::Init()
{
    m_Width = 1280;
    m_Height = 720;

    m_WireShader.reset(new Shader("Resources/Shaders/VertexColor.vs", "Resources/Shaders/Wireframe.fs"));
    
    FrameBufferSpecification spec;
    spec.Width = m_Width;
    spec.Height = m_Height;
    m_FrameBuffer.reset(new GLFrameBuffer(spec));

    glEnable(GL_MULTISAMPLE);
}

void Renderer::BeginScene(View& camera)
{
    Clear();
    m_FrameBuffer->Bind();
    m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    drawCalls = 0;
}

void Renderer::EndScene()
{
    m_FrameBuffer->Unbind();
    m_PointLights.clear();
    m_DirectionalLights.clear();
    m_RenderObjects.clear();
}

void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<GLVertexArray>& vertexArray, const glm::mat4& transform, const std::vector<Texture*>& textures, const RenderSettings& settings)
{
    m_RenderObjects.push_back({ shader, vertexArray, transform, settings, textures });
 
}

void Renderer::Submit(const PointLightData& light)
{
    m_PointLights.push_back(light);
}

void Renderer::Submit(const DirectionalLightData& light)
{
    m_DirectionalLights.push_back(light);
}

void Renderer::SetRenderMode(RenderMode mode)
{
    m_renderMode = mode;
    switch (mode)
    {
    case RenderMode::WIREFRAME:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_LIGHTING);
        break;
    case RenderMode::UNLIT:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_LIGHTING);
        break;
    case RenderMode::LIT:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_LIGHTING);
        break;
    }
}

void Renderer::Render()
{
    for (auto object : m_RenderObjects)
    {
        auto& shader = object.shader;
        auto& vertexArray = object.vertexArray;
        auto& transform = object.transform;
        auto& settings = object.settings;
        
               
        vertexArray->Bind();
        shader->Bind();
        
        for (int i = 0; i < object.textures.size(); i++)
        {
            if (object.textures[i] != nullptr)
            {
                object.textures[i]->Bind(i);
                shader->SetUniform1i("u_Textures[" + std::to_string(i) + "]", i);
            }
        }
        
        shader->SetUniform1i("Wireframe", m_renderMode == RenderMode::WIREFRAME);
        shader->SetUniform1i("Unlit", m_renderMode == RenderMode::UNLIT);
        
        shader->SetUniform1f("AmbientStrength", m_AmbientLightStrength);
        shader->SetUniform3f("AmbientColour", clearColor.x, clearColor.y, clearColor.z);
        
        shader->SetUniform3f("CameraPosition", m_SceneData->ViewProjectionMatrix[3].x, m_SceneData->ViewProjectionMatrix[3].y, m_SceneData->ViewProjectionMatrix[3].z);
        
        shader->SetUniformMat4f("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
        shader->SetUniformMat4f("u_Transform", transform);
        shader->SetUniform1f("CurrentTime", App::Instance().GetTime());

        for (int i = 0; i < m_PointLights.size() && i < 8; i++)
        {
            shader->SetUniform3f("PointLights[" + std::to_string(i) + "].Position", m_PointLights[i].position.x, m_PointLights[i].position.y, m_PointLights[i].position.z);
            shader->SetUniform3f("PointLights[" + std::to_string(i) + "].Color", m_PointLights[i].color.x, m_PointLights[i].color.y, m_PointLights[i].color.z);
            shader->SetUniform1f("PointLights[" + std::to_string(i) + "].Intensity", m_PointLights[i].intensity);
            shader->SetUniform1f("PointLights[" + std::to_string(i) + "].Radius", m_PointLights[i].radius);
        }
        shader->SetUniform1i("PointLightCount", (int)m_PointLights.size());

        for (int i = 0; i < m_DirectionalLights.size() && i < 8; i++)
        {
            shader->SetUniform3f("DirLights[" + std::to_string(i) + "].Direction", m_DirectionalLights[i].direction.x, m_DirectionalLights[i].direction.y, m_DirectionalLights[i].direction.z);
            shader->SetUniform3f("DirLights[" + std::to_string(i) + "].Color", m_DirectionalLights[i].color.x, m_DirectionalLights[i].color.y, m_DirectionalLights[i].color.z);
            shader->SetUniform1f("DirLights[" + std::to_string(i) + "].Intensity", m_DirectionalLights[i].intensity);
        }
        shader->SetUniform1i("DirLightCount", (int)m_DirectionalLights.size());
        
            
        DrawIndexed(vertexArray, settings);
        vertexArray->Unbind();
        drawCalls++;
    }
}

void Renderer::Clear()
{
    Renderer::m_FrameBuffer->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Renderer::m_FrameBuffer->Unbind();
}

void Renderer::SetClearColor(const glm::vec4& color)
{
    clearColor = color;
    glClearColor(color.r, color.g, color.b, color.a);
}

double Renderer::AspectRatio()
{
    return GetViewportSize().x / GetViewportSize().y;
}

void Renderer::DrawIndexed(const std::shared_ptr<GLVertexArray>& vertexArray, RenderSettings settings)
{
    Renderer::m_FrameBuffer->Bind();
    

        if (settings.Blending)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else
        {
            glDisable(GL_BLEND);
        }

        if (settings.Culling)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        
        }

        if (settings.DepthTest)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }

        if (settings.DepthWrite)
        {
            glEnable(GL_DEPTH_WRITEMASK);
        }
        else
        {
            glEnable(GL_DEPTH_WRITEMASK);
        }
    
    glDrawElements(settings.DrawMode, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    Renderer::m_FrameBuffer->Unbind();

}


void Renderer::Resize(int width, int height)
{
    m_Width = width;
    m_Height = height;

    Renderer::m_FrameBuffer->Resize(width, height);
}



