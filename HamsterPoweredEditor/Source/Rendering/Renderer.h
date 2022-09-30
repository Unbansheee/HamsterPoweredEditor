#pragma once
#include <glad/glad.h>

#include "OrthographicCamera.h"

#include "ResourceManagement/Shader.h"
#include "GLFrameBuffer.h"
#include "GLVertexArray.h"
#include "UI/RendererSettings.h"


struct RenderSettings
{
    uint32_t DrawMode = GL_TRIANGLES;
    bool Blending = false;
    bool DepthTest = true;
    bool Culling = true;
    bool DepthWrite = true;
};

class Renderer
{
public:


    
    enum class RenderMode
    {
        UNLIT,
        WIREFRAME
    };
    
    friend class OpenGLRendererAPI;
    friend class RendererAPI;
    friend class Viewport;
    friend class Stats;
    
    static void Init();
    static void BeginScene(View& camera);
    static void EndScene();
    static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<GLVertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f), const RenderSettings& settings = RenderSettings());
    static void SetClearColor(const glm::vec4& color);
    static const glm::vec4& GetClearColor() {return clearColor;}
    static double AspectRatio();
    static unsigned int GetRenderTexture() {return renderTexture;}
    static void SetRenderMode(RenderMode mode);
    static RenderMode GetRenderMode() {return m_renderMode;}
    static glm::vec2 GetViewportSize() {return {m_Width, m_Height};}
private:
    static void Clear();
    
    static void DrawIndexed(const std::shared_ptr<GLVertexArray>& vertexArray, RenderSettings settings);
    static void Resize(int width, int height);

    
private:
    inline static unsigned int frameBuffer;
    inline static unsigned int depthrenderbuffer;
    inline static unsigned int renderTexture;
    inline static std::shared_ptr<GLFrameBuffer> m_FrameBuffer;
    inline static GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    inline static int drawCalls;
    inline static int m_Width;
    inline static int m_Height;
    inline static glm::vec4 clearColor;
    inline static RenderMode m_renderMode;
    inline static std::shared_ptr<Shader> m_WireShader;
    
    struct SceneData
    {
        glm::mat4 ViewProjectionMatrix;
    };

    static SceneData* m_SceneData;


};

