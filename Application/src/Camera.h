#pragma once

#include <utility>

#include "pch.h"
#include "Renderer.h"

class Camera
{
public:
    Camera();

    void handle_keys(GLFWwindow* window, Renderer& renderer, float delta_time);
    void handle_mouse_move(GLFWwindow* window, Renderer& renderer, float x_pos, float y_pos);
    void setMousePosition(glm::vec2 position);
    glm::vec3 getPosition() const { return this->m_position; }
private:
    static constexpr glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 m_position;
    glm::vec3 m_front;
    float m_camera_speed;
    float m_mouse_sensitivity;
    float m_last_mouse_x, m_last_mouse_y;
    float m_pitch, m_yaw;
};