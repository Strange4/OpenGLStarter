#pragma once

#include "pch.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

class VertexArray
{
private:
    GLuint m_id;
    
public:
    VertexArray();
    ~VertexArray();

    /// <summary>
    /// binds and enables the given buffers to this vertex array with the given index buffer
    /// </summary>
    /// <param name="vertex_buffer"></param>
    /// <param name="index_buffer"></param>
    /// <param name="layout"></param>
    void bindBuffer(const VertexBuffer& vertex_buffer, const IndexBuffer& index_buffer, const VertexBufferLayout& layout) const;

    /// <summary>
    /// binds and enables the given buffers to this vertex array without indexing the buffer
    /// </summary>
    /// <param name="vertex_buffer"></param>
    /// <param name="index_buffer"></param>
    /// <param name="layout"></param>
    void bindBuffer(const VertexBuffer& vertex_buffer, const VertexBufferLayout& layout) const;

    /// <summary>
    /// Binds this vertex array to the current context
    /// </summary>
    void bind() const;

    /// <summary>
    /// unbinds all vertex arrays from the current context
    /// </summary>
    void unbind() const;
};
