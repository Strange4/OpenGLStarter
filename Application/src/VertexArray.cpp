#include "pch.h"
#include "VertexArray.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &this->m_id);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &this->m_id);
}

void VertexArray::addBuffer(const VertexBuffer& vertex_buffer, const VertexBufferLayout& layout)
{
    this->bind();
    vertex_buffer.bind();

    const std::vector<VertexAttribute>& elements = layout.getElements();

    unsigned int offset = 0;
    for (GLuint i = 0; i < elements.size(); i++)
    {
        const VertexAttribute& attrib = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, attrib.element_count, attrib.type, attrib.normalized, layout.getStride(), (const void*)offset);
        offset += attrib.element_count * get_size_of_type(attrib.type);
    }
}

void VertexArray::bind() const
{
    glBindVertexArray(this->m_id);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}
