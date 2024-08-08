#pragma once

#include <GL/glew.h>
class IndexBuffer
{
private:
    GLuint m_id;
    GLsizei m_count;

public:
    IndexBuffer(const std::vector<unsigned int>& data);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    GLsizei getCount() const;
};