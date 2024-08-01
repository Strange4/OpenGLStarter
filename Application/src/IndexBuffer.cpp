#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned short* data, GLsizei count)
    : m_count(count)
{
    glGenBuffers(1, &this->m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned short), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &this->m_id);
}

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id);
}


void IndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


GLsizei IndexBuffer::getCount() const
{
    return this->m_count;
}