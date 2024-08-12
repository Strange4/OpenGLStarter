#pragma once

#include "Vertex.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Model.h"

class Renderer
{
public:
    Renderer(unsigned int window_with, unsigned int window_height);

    void drawModels(ShaderProgram& shader_program) const;
    void setViewTransform(glm::mat4 transformation);
    void resize(unsigned int window_width, unsigned int window_height);
    void addModel(std::shared_ptr<Model> model);

    glm::mat4 getProjection() const { return this->m_projection; }

    static void clear();
    static void draw(const VertexArray& vertex_array, GLsizei triangle_count, const ShaderProgram& shader_program);
    static void setWiremesh();
    static void unsetWiremesh();

private:
    glm::mat4 m_view;
    glm::mat4 m_projection;

    std::vector<std::shared_ptr<Model>> m_models;
    
};
