#include "pch.h"
#include "Renderer.h"

void Renderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::draw(const VertexArray& vertex_array, const IndexBuffer& index_buffer, const ShaderProgram& shader_program)
{
    shader_program.bind();
    index_buffer.bind();
    vertex_array.bind();

    glDrawElements(GL_TRIANGLES, index_buffer.getCount(), GL_UNSIGNED_SHORT, nullptr);
}
