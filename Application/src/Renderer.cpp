#include "pch.h"
#include "Renderer.h"

void Renderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::draw(const VertexArray& vertex_array, GLsizei triangle_count, const ShaderProgram& shader_program)
{
    shader_program.bind();
    // index_buffer.bind();
    vertex_array.bind();

    glDrawElements(GL_TRIANGLES, triangle_count, GL_UNSIGNED_SHORT, nullptr);
}
