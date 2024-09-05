#include "pch.h"

#include <iostream>
#include <type_traits>

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
    m_object_shader(std::make_unique<ShaderProgram>()),
    m_light_shader(std::make_unique<ShaderProgram>()),
    m_renderer(Renderer((int) this->START_WINDOW_WIDTH, (int) this->START_WINDOW_HEIGHT)),
    m_model_scale(1.0f), m_model_rotation(0.0f), m_state(ApplicationState::Moving)
{
    Shader fragment("res/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    Shader vertex("res/shaders/vertex.glsl", GL_VERTEX_SHADER);
    Shader light("res/shaders/light.glsl", GL_FRAGMENT_SHADER);

    this->m_object_shader->attachAllShaders({ fragment, vertex });
    this->m_light_shader->attachAllShaders({ light, vertex });
    this->m_object_shader->link();
    this->m_light_shader->link();

    this->m_material = {
        .ambient = glm::vec3(1.0f, 0.5f, 0.31f),
        .ambient_strength = 0.5f,
        .diffuse = glm::vec3(1.0f, 0.5f, 0.31f),
        .diffuse_strength = 0.5f,
        .specular = glm::vec3(0.5f, 0.5f, 0.5f),
        .specular_strength = 0.5f,
        .shininess = 32,
    };
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

        // contatenating the matrices
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), this->m_model_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(this->m_model_scale));
        this->m_current_model.get()->setTransform(model);


        // lighting stuff

        this->m_object_shader->setUniform3f("u_light_color", 1.0f, 1.0f, 1.0f);
        this->m_object_shader->setUniform3f("u_light_pos", 1.2f, 1.5f, 2.0f);
        this->m_object_shader->setUniform3f("u_camera_pos", this->m_camera.getPosition());

        // materials
        this->m_object_shader->setUniform3f("u_material.ambient", this->m_material.ambient);
        this->m_object_shader->setUniform1f("u_material.ambient_strength", this->m_material.ambient_strength);
        this->m_object_shader->setUniform3f("u_material.diffuse", this->m_material.diffuse);
        this->m_object_shader->setUniform1f("u_material.diffuse_strength", this->m_material.diffuse_strength);
        this->m_object_shader->setUniform3f("u_material.specular", this->m_material.specular);
        this->m_object_shader->setUniform1f("u_material.specular_strength", this->m_material.specular_strength);
        this->m_object_shader->setUniform1i("u_material.shininess", this->m_material.shininess);
        
        // actually rendering
        this->m_renderer.renderModel(this->m_current_model, *this->m_object_shader);
        this->m_renderer.renderModel(this->m_light_model, *this->m_light_shader);
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
    this->m_current_model = model;
}

void Application::setLightModel(std::shared_ptr<Model> model)
{
    this->m_light_model = model;
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

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::setGui()
{
    // only render if it's open
    if (ImGui::Begin("Matrix Settings"))
    {
        // transform edit
        ImGui::SliderFloat("Model Scale", &this->m_model_scale, 0.0f, 10.0f);
        ImGui::SliderFloat("Model Rotation", &this->m_model_rotation, -glm::two_pi<float>(), glm::two_pi<float>());

        // material edit
        ImGui::ColorEdit3("Ambient Color", glm::value_ptr(this->m_material.ambient));

        ImGui::SliderFloat("Ambient Strength", &this->m_material.ambient_strength, 0.0f, 1.0f);
        ImGui::ColorEdit3("Diffuse Color", glm::value_ptr(this->m_material.diffuse));

        ImGui::SliderFloat("Diffuse Strength", &this->m_material.diffuse_strength, 0.0f, 1.0f);
        ImGui::ColorEdit3("Specular Color", glm::value_ptr(this->m_material.specular));

        ImGui::SliderFloat("Specular Strength", &this->m_material.specular_strength, 0.0f, 1.0f);

        ImGui::SliderInt("Shininess", &this->m_material.shininess, 0, 50);
       

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

    ImGui::End();
}

void Application::replaceShader(const std::string& file_path, GLenum shader_type)
{
    this->m_object_shader->deleteShaderType(shader_type);
    std::vector<Shader> shaders = this->m_object_shader->getShaders();
    shaders.push_back(Shader(file_path, shader_type));
    GLsizei count = 0;
    GLuint attachedShaders[5];
    glGetAttachedShaders(this->m_object_shader->getId(), 5, &count, attachedShaders);

    std::unique_ptr<ShaderProgram> newProgram = std::make_unique<ShaderProgram>(shaders);

    this->m_object_shader.reset();
    this->m_object_shader = std::move(newProgram);

    glGetAttachedShaders(this->m_object_shader->getId(), 5, &count, attachedShaders);
}
