#pragma once

#include <GL/glew.h>
#include <vector>

struct VertexAttribute
{
    GLenum type;
    GLint element_count;
    GLboolean normalized;
};

static GLsizei get_size_of_type(GLenum type)
{
    switch (type)
    {
    case GL_FLOAT:
        return sizeof(GLfloat);
    case GL_UNSIGNED_INT:
        return sizeof(GLuint);
    case GL_UNSIGNED_SHORT:
        return sizeof(GLshort);
    }
    return 0;
}

class VertexBufferLayout
{
private:
    GLsizei m_stride;
    std::vector<VertexAttribute> m_attributes;
public:
    VertexBufferLayout(std::vector<VertexAttribute> a_attributes);
    
    const std::vector<VertexAttribute>& getElements() const { return this->m_attributes; }
    GLsizei getStride() const { return this->m_stride; }
};