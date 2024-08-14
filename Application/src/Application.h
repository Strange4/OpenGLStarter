#pragma once

#include "pch.h"

#include <memory>
#include <ImGuiFileDialog.h>

#include "Model.h"
#include "Camera.h"

enum ApplicationState : uint8_t {
    Moving,
    Editing,
};

class Application
{
public:

    void mainLoop();
    void setModel(std::shared_ptr<Model> model);
    void setWindow(GLFWwindow* window);
    void setMovingState();
    void setEditingState();

    Renderer& getRenderer() { return this->m_renderer; }
    Camera& getCamera() { return this->m_camera; }
    ApplicationState getState() { return this->m_state; }

    // starting defaults
    static constexpr float START_WINDOW_WIDTH = 1440.0f;
    static constexpr float START_WINDOW_HEIGHT = (9.0f / 16.0f) * START_WINDOW_WIDTH;

    // singleton stuff
    static Application& getInstance()
    {
        static Application app;
        return app;
    }
    Application(Application const&) = delete;
    void operator=(Application const&) = delete;

private:
    // application state
    ApplicationState m_state;
    Application();

    void renderGui();
    void setGui();


    GLFWwindow* m_window;
    int m_window_width, m_window_height;
    std::shared_ptr<Model> m_current_model;
    ShaderProgram m_shader_program;
    Renderer m_renderer;
    Camera m_camera;

    // settings
    float m_model_scale;
    float m_model_rotation;
    ImGuiFileDialog m_file_dialog;
};