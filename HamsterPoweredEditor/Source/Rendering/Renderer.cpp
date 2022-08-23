#include "Renderer.h"

#include "Core/App.h"


Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

void Renderer::Init()
{
    m_Width = 1280;
    m_Height = 720;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

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

void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<GLVertexArray>& vertexArray, const glm::mat4& transform, uint32_t drawMode)
{
    if (m_renderMode == RenderMode::WIREFRAME)
    {
        m_WireShader->Bind();
    }
    else
    {
        shader->Bind();
    }
    vertexArray->Bind();
    shader->SetUniformMat4f("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
    shader->SetUniformMat4f("u_Transform", transform);
    shader->SetUniform1f("CurrentTime", App::Instance().GetTime());
    
    
    DrawIndexed(vertexArray, drawMode);
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

void Renderer::DrawIndexed(const std::shared_ptr<GLVertexArray>& vertexArray, uint32_t drawMode)
{
    Renderer::m_FrameBuffer->Bind();
    glDrawElements(drawMode, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    Renderer::m_FrameBuffer->Unbind();
}


void Renderer::Resize(int width, int height)
{
    m_Width = width;
    m_Height = height;

    Renderer::m_FrameBuffer->Resize(width, height);
}



