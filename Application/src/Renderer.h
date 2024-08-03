#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
    static void clear();
    static void draw(const VertexArray& vertex_array, const IndexBuffer& index_buffer, const ShaderProgram& shader_program);
};
