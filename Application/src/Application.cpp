#include "pch.h"

#include <iostream>
#include <type_traits>

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
    : m_window(nullptr),
    m_shader_program(std::make_unique<ShaderProgram>(std::vector<Shader>{
            Shader("res/shaders/fragment.glsl", GL_FRAGMENT_SHADER),
            Shader("res/shaders/vertex.glsl", GL_VERTEX_SHADER)
        })),
    m_renderer(Renderer((int) this->START_WINDOW_WIDTH, (int) this->START_WINDOW_HEIGHT)),
    m_model_scale(1.0f), m_model_rotation(0.0f), m_state(ApplicationState::Moving)
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

        this->m_shader_program->bind();

        // contatenating the matrices
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), this->m_model_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(this->m_model_scale));
        this->m_current_model.get()->setTransform(model);

        this->m_renderer.drawModels(*this->m_shader_program);

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
    glfwSetCursorPos(this->m_window, 1.0, 1.0);
    this->getCamera().setMousePosition(glm::vec2(1.0f));
    glfwSetInputMode(this->m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}

void Application::setEditingState()
{
    this->m_state = ApplicationState::Editing;
    glfwSetInputMode(this->m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
    // only render if it's open
    if (ImGui::Begin("Matrix Settings"))
    {
        ImGui::SliderFloat("Model Scale", &this->m_model_scale, 0.0f, 10.0f);
        ImGui::SliderFloat("Model Rotation", &this->m_model_rotation, -glm::two_pi<float>(), glm::two_pi<float>());
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        if (ImGui::Button("Load model"))
        {
            IGFD::FileDialogConfig config;
            config.path = "./res/models/";
            config.flags = ImGuiFileDialogFlags_DisableCreateDirectoryButton;
            ImGuiFileDialog::Instance()->OpenDialog("model_file_picker", "Select a new model to load", "{.obj,.fbx}", config);
        }
        ImGui::SameLine();
        if (ImGui::Button("Load fragment shader"))
        {
            IGFD::FileDialogConfig config;
            config.path = "./res/shaders/";
            config.flags = ImGuiFileDialogFlags_DisableCreateDirectoryButton;
            ImGuiFileDialog::Instance()->OpenDialog("fragment_file_picker", "Select a new fragment shader to load", ".glsl", config);
        }
        ImGui::SameLine();
        if (ImGui::Button("Load vertex shader"))
        {
            IGFD::FileDialogConfig config;
            config.path = "./res/shaders/";
            config.flags = ImGuiFileDialogFlags_DisableCreateDirectoryButton;
            ImGuiFileDialog::Instance()->OpenDialog("vertex_file_picker", "Select a new vertex shader to load", ".glsl", config);
        }
    }

    // model picker
    if (ImGuiFileDialog::Instance()->Display("model_file_picker"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
            std::shared_ptr<Model> newModel = std::make_shared<Model>(filePath);
            this->setModel(newModel);
        }
        ImGuiFileDialog::Instance()->Close();
    }
    // fragment picker
    if (ImGuiFileDialog::Instance()->Display("fragment_file_picker"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
            this->replaceShader(filePath, GL_FRAGMENT_SHADER);
        }
        ImGuiFileDialog::Instance()->Close();
    }
    // vertex picker
    if (ImGuiFileDialog::Instance()->Display("vertex_file_picker"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
            this->replaceShader(filePath, GL_VERTEX_SHADER);
        }
        ImGuiFileDialog::Instance()->Close();
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

void Application::replaceShader(const std::string& file_path, GLenum shader_type)
{
    this->m_shader_program->deleteShaderType(shader_type);
    std::vector<Shader> shaders = this->m_shader_program->getShaders();
    shaders.push_back(Shader(file_path, shader_type));
    GLsizei count = 0;
    GLuint attachedShaders[5];
    glGetAttachedShaders(this->m_shader_program->getId(), 5, &count, attachedShaders);

    std::unique_ptr<ShaderProgram> newProgram = std::make_unique<ShaderProgram>(shaders);

    this->m_shader_program.reset();
    this->m_shader_program = std::move(newProgram);

    glGetAttachedShaders(this->m_shader_program->getId(), 5, &count, attachedShaders);
}
