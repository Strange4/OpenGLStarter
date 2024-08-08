#include "pch.h"

#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const std::vector<unsigned short>& data)
    : m_count(data.size())
{
    glGenBuffers(1, &this->m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned short), data.data(), GL_STATIC_DRAW);
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
