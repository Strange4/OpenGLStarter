#pragma once

#include "pch.h"
#include "Renderer.h"

class Camera
{
public:
    Camera();

    void handle_keys(GLFWwindow* window, Renderer& renderer, float delta_time);

private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    float m_camera_speed;

    static constexpr glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
};