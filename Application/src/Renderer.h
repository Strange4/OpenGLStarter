#pragma once

#include "Vertex.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Model.h"

class Renderer
{
public:
    Renderer(int window_width, int window_height);

    void drawModels(ShaderProgram& shader_program) const;
    void setViewTransform(glm::mat4 transformation);
    void tryResize(GLFWwindow* window);
    void addModel(std::shared_ptr<Model> model);

    glm::mat4 getProjection() const { return this->m_projection; }

    static void clear();
    static void draw(const VertexArray& vertex_array, GLsizei triangle_count, const ShaderProgram& shader_program);
    static void setWiremesh();
    static void unsetWiremesh();

private:
    glm::mat4 m_view;
    glm::mat4 m_projection;
    int m_width, m_height;

    std::vector<std::shared_ptr<Model>> m_models;
    
};
