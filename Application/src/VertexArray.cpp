#include "pch.h"
#include "VertexArray.h"

#include <iostream>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &this->m_id);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &this->m_id);
}

void VertexArray::bindBuffer(const VertexBuffer& vertex_buffer, const IndexBuffer& index_buffer, const VertexBufferLayout& layout) const
{
    this->bind();
    vertex_buffer.bind();

    const std::vector<VertexAttribute>& elements = layout.getElements();

    unsigned int offset = 0;
    for (GLuint i = 0; i < elements.size(); i++)
    {
        const VertexAttribute& attrib = elements[i];
        glEnableVertexAttribArray(i);

        // compiler warning but I don't know how to handle it
        glVertexAttribPointer(i, attrib.element_count, attrib.type, attrib.normalized, layout.getStride(), (const void*)offset);
        offset += attrib.element_count * get_size_of_type(attrib.type);
    }
    index_buffer.bind();
} 

void VertexArray::bind() const
{
    glBindVertexArray(this->m_id);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}
