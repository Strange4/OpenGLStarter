#pragma once

#include "pch.h"
#include "Renderer.h"

class Camera
{
public:
    Camera();

    void handle_input(GLFWwindow* window, Renderer& renderer, float delta_time);
private:
    static constexpr glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 m_position;
    glm::vec3 m_front;
    float m_camera_speed;
    float m_mouse_sensitivity;
    float m_last_mouse_x, m_last_mouse_y;
    float m_pitch, m_yaw;
    


    void handle_keys(GLFWwindow* window, Renderer& renderer, float delta_time);
    void handle_mouse(GLFWwindow* window, Renderer& renderer);
};