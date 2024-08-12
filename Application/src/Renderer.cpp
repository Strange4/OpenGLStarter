#include "pch.h"
#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer(int window_width, int window_height)
    : m_view(glm::mat4(1.0f)), m_width(window_width), m_height(window_height)
{
    this->m_projection = glm::perspective(glm::radians(45.0f), (float)window_width / (float)window_height, 0.01f, 100.0f);
}

void Renderer::drawModels(ShaderProgram& shader_program) const
{
    shader_program.setUniformMatrix4f("u_projection", this->m_projection);
    shader_program.setUniformMatrix4f("u_view", this->m_view);
    for (const std::shared_ptr<Model>& model : this->m_models)
    {
        shader_program.setUniformMatrix4f("u_model", model.get()->getTransform());
        model.get()->draw(shader_program);
    }
}

void Renderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(const VertexArray& vertex_array, GLsizei triangle_count, const ShaderProgram& shader_program)
{
    shader_program.bind();
    vertex_array.bind();
    glDrawElements(GL_TRIANGLES, triangle_count, GL_UNSIGNED_INT, nullptr);
}

void Renderer::setWiremesh()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer::unsetWiremesh()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::setViewTransform(glm::mat4 view)
{
    this->m_view = view;
}

void Renderer::tryResize(GLFWwindow* window)
{
    int window_width = 0;
    int window_height = 0;
    glfwGetWindowSize(window, &window_width, &window_height);

    // don't resize if they are equal
    if (window_width == this->m_width && window_height == this->m_height)
        return;
    this->m_projection = glm::perspective(glm::radians(45.0f), (float)window_width / (float)window_height, 0.01f, 100.0f);
}

void Renderer::addModel(std::shared_ptr<Model> model)
{
    this->m_models.emplace_back(model);
}
