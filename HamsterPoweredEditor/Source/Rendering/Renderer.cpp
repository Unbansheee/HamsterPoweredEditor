#include "Renderer.h"

#include "Core/App.h"
#include "Core/Timer.h"


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
}

void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<GLVertexArray>& vertexArray, const glm::mat4& transform, const RenderSettings& settings)
{
    vertexArray->Bind();
    shader->Bind();
    shader->SetUniform1i("Wireframe", m_renderMode == RenderMode::WIREFRAME);
    
    shader->SetUniformMat4f("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
    shader->SetUniformMat4f("u_Transform", transform);
    shader->SetUniform1f("CurrentTime", App::Instance().GetTime());
    
    
    DrawIndexed(vertexArray, settings);
    drawCalls++;
}

void Renderer::SetRenderMode(RenderMode mode)
{
    m_renderMode = mode;
    switch (mode)
    {
    case RenderMode::WIREFRAME:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case RenderMode::UNLIT:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
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
    return 16.f/9.f;
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
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_WRITEMASK);
}


void Renderer::Resize(int width, int height)
{
    m_Width = width;
    m_Height = height;

    Renderer::m_FrameBuffer->Resize(width, height);
}



