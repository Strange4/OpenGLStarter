#pragma once
#include "pch.h"
#include "Vertex.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Mesh 
{

public:
    Mesh(std::vector<Vertex> vertices,
        std::vector<unsigned int> indices,
        std::vector<Texture> textures);

    void draw(ShaderProgram& shader_program) const;

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;

    VertexBuffer m_vertex_buffer;
    IndexBuffer m_index_buffer;
    VertexArray m_vertex_array;
};
