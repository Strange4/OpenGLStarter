#include "pch.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Application.h"
#include "Renderer.h"
#include "Camera.h"
#include "Setup.h"


Application::Application()
    : m_window(nullptr), m_window_width((int) this->START_WINDOW_WIDTH), m_window_height((int) this->START_WINDOW_HEIGHT),
    m_shader_program(ShaderProgram({
        Shader("res/shaders/fragment.glsl", GL_FRAGMENT_SHADER),
        Shader("res/shaders/vertex.glsl", GL_VERTEX_SHADER)
        })),
    m_renderer(Renderer((int) this->START_WINDOW_WIDTH, (int) this->START_WINDOW_HEIGHT)),
    m_model_translation(0.0f), m_model_rotation(0.0f)
{
}

void Application::mainLoop()
{
    double previousTime = 0;
    
    if (!this->m_window)
    {
        std::cerr << "There is no window to run the main loop with. Maybe it wasn't initialized" << std::endl;
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(this->m_window))
    {
        Renderer::clear();

        // handle inputs
        double newTime = glfwGetTime();
        this->m_camera.handle_keys(this->m_window, this->m_renderer, (float)(newTime - previousTime));
        previousTime = newTime;

        this->m_shader_program.bind();
        this->m_shader_program.setUniform4f("u_color", sin(previousTime), cos(previousTime), tan(previousTime), 1.0f);

        // contatenating the matrices
        glm::mat4 model = glm::translate(glm::mat4(1.0f), this->m_model_translation);
        model = glm::rotate(model, this->m_model_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        this->m_current_model.get()->setTransform(model);

        this->m_renderer.drawModels(this->m_shader_program);

        this->renderGui();


        /* Swap front and back buffers */
        glfwSwapBuffers(this->m_window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

void Application::setModel(std::shared_ptr<Model> model)
{
    this->m_renderer.removeModel(this->m_current_model);
    this->m_current_model = model;
    this->m_renderer.addModel(model);
}

void Application::setWindow(GLFWwindow* window)
{
    this->m_window = window;
}

void Application::setMovingState()
{
    this->m_state = ApplicationState::Moving;
    glfwSetInputMode(this->m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::setEditingState()
{
    glfwSetInputMode(this->m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    this->m_state = ApplicationState::Editing;
}

void Application::renderGui()
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    this->setGui();

    // render imgui on top
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::setGui()
{
    if (ImGui::Begin("Matrix Settings"))
    {
        ImGui::SliderFloat3("Model Translation", glm::value_ptr(this->m_model_translation), -10.0f, 10.0f);
        ImGui::SliderFloat("Model Rotation", &this->m_model_rotation, -glm::two_pi<float>(), glm::two_pi<float>());
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    
    switch (this->m_state)
    {
    case ApplicationState::Moving:
        ImGui::SetWindowCollapsed(true);
        break;
    default:
        ImGui::SetWindowCollapsed(false);
        break;
    }

    ImGui::End();
}
