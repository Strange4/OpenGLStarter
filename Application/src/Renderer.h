#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
    static void clear();
    static void draw(const VertexArray& vertex_array, GLsizei triangle_count, const ShaderProgram& shader_program);
    static void setWiremesh();
    static void unsetWiremesh();
};
