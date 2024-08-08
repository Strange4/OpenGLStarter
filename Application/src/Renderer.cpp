#include "pch.h"
#include "Renderer.h"

void Renderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(const VertexArray& vertex_array, GLsizei triangle_count, const ShaderProgram& shader_program)
{
    shader_program.bind();
    vertex_array.bind();
    glDrawElements(GL_TRIANGLES, triangle_count, GL_UNSIGNED_SHORT, nullptr);
}

void Renderer::setWiremesh()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer::unsetWiremesh()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}