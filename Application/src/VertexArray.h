#pragma once

#include <GL/glew.h>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
    GLuint m_id;
public:
    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer& vertex_buffer, const VertexBufferLayout& layout);
    void bind() const;
    void unbind() const;
};
