﻿#pragma once
#include <cstdint>

#include "VertexBufferLayout.h"

class GLVertexBuffer
{
public:
    GLVertexBuffer(float* vertices, uint32_t size, uint32_t drawMode = GL_STATIC_DRAW);
    virtual ~GLVertexBuffer();
    virtual void Bind() const;
    virtual void Unbind() const;

    virtual const VertexBufferLayout& GetLayout() const { return m_Layout; }
    virtual void SetLayout(const VertexBufferLayout& layout) { m_Layout = layout; }

    virtual void SetSubData(float* vertices, uint32_t size, uint32_t offset = 0) ;
private:
    uint32_t m_RendererID;
    VertexBufferLayout m_Layout;
    uint32_t m_DrawMode;
};



class GLIndexBuffer
{
public:
    GLIndexBuffer(uint32_t* indices, uint32_t size, uint32_t drawMode = GL_STATIC_DRAW);
    virtual ~GLIndexBuffer();

    virtual uint32_t GetCount() const {return m_Count;}
    virtual void Bind() const;
    virtual void Unbind() const;

private:
    uint32_t m_RendererID;
    uint32_t m_Count;
    uint32_t m_DrawMode;
};