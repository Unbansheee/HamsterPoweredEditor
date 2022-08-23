#pragma once
#include <cstdint>
#include <glm/vec2.hpp>

struct FrameBufferSpecification
{
    uint32_t Width, Height;
    uint32_t Samples = 1;
    bool SwapChainTarget = false;
};

class GLFrameBuffer
{
public:
    virtual const FrameBufferSpecification& GetSpecification() const {return m_Specification;}
    
    GLFrameBuffer(const FrameBufferSpecification& spec);
    virtual ~GLFrameBuffer();

    virtual void Bind();
    virtual void Unbind();

    uint32_t GetColorAttachmentRendererID() const {return m_ColorAttachment;}
    
    void Invalidate();
    void Resize(uint32_t width, uint32_t height);
private:
    FrameBufferSpecification m_Specification;
    uint32_t m_RendererID = 0;
    uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
    
};
