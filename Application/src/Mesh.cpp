#include "pch.h"
#include "Mesh.h"

#include "Renderer.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    : m_vertex_buffer(VertexBuffer(vertices.data(), vertices.size() * (unsigned int) sizeof(Vertex))), m_index_buffer(indices), m_vertex_array(VertexArray())
{
    this->m_vertex_array.bindBuffer(this->m_vertex_buffer, this->m_index_buffer, Vertex::getLayout());
}

void Mesh::draw(ShaderProgram& shader_program) const
{
    for (GLuint i = 0; i < this->m_textures.size(); i++)
    {
        shader_program.setUniform1i("u_texture" + std::to_string(i), i);
        this->m_textures[i].bind(i);
    }

    Renderer::draw(this->m_vertex_array, this->m_index_buffer.getCount(), shader_program);
}
