#pragma once
#include <cstdint>

#include "VertexBufferLayout.h"

class GLVertexBuffer
{
public:
    GLVertexBuffer(float* vertices, uint32_t size);
    virtual ~GLVertexBuffer();
    virtual void Bind() const;
    virtual void Unbind() const;

    virtual const VertexBufferLayout& GetLayout() const { return m_Layout; }
    virtual void SetLayout(const VertexBufferLayout& layout) { m_Layout = layout; }
    
private:
    uint32_t m_RendererID;
    VertexBufferLayout m_Layout;
};



class GLIndexBuffer
{
public:
    GLIndexBuffer(uint32_t* indices, uint32_t size);
    virtual ~GLIndexBuffer();

    virtual uint32_t GetCount() const {return m_Count;}
    virtual void Bind() const;
    virtual void Unbind() const;

private:
    uint32_t m_RendererID;
    uint32_t m_Count;
};
