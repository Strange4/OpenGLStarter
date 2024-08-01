#pragma once

#include <GL/glew.h>
class VertexBuffer
{
private:
    GLuint m_id;

public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
};