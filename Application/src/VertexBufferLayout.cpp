#include "VertexBufferLayout.h"



VertexBufferLayout::VertexBufferLayout(std::vector<VertexAttribute> attributes)
    : m_attributes(attributes)
{
    GLsizei size = 0;
    for (size_t i = 0; i < attributes.size(); i++)
    {
        VertexAttribute& attrib = attributes[i];
        size += get_size_of_type(attrib.type) * attrib.element_count;
    }
    this->m_stride = size;
}