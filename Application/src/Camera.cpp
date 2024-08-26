#include "pch.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Camera.h"

Camera::Camera()
    : m_camera_speed(5.0f), m_front(glm::vec3(0.0f, 0.0f, -1.0f)), 
    m_position(glm::vec3(0.0f, 0.0f, 5.0f)), 
    m_mouse_sensitivity(0.07f),
    m_last_mouse_x(0), m_last_mouse_y(0),
    m_pitch(0), m_yaw(-90.0f)
{
}

void Camera::handle_keys(GLFWwindow* window, Renderer& renderer, float delta_time)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        this->m_position += delta_time * this->m_camera_speed * this->m_front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        this->m_position -= delta_time * this->m_camera_speed * glm::normalize(glm::cross(this->m_front, this->CAMERA_UP));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        this->m_position -= delta_time * this->m_camera_speed * this->m_front;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        this->m_position += delta_time * this->m_camera_speed * glm::normalize(glm::cross(this->m_front, this->CAMERA_UP));
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        this->m_position += delta_time * this->m_camera_speed * this->CAMERA_UP;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        this->m_position -= delta_time * this->m_camera_speed * this->CAMERA_UP;

    renderer.setViewTransform(glm::lookAt(this->m_position, this->m_position + this->m_front, this->CAMERA_UP));
}

void Camera::handle_mouse_move(GLFWwindow* window, Renderer& renderer, float x_pos, float y_pos)
{
    // the first time, set the last position and return
    if (this->m_last_mouse_x == 0.0f || this->m_last_mouse_y == 0.0f)
    {
        this->m_last_mouse_x = x_pos;
        this->m_last_mouse_y = y_pos;
        return;
    }

    float xOffset = (x_pos - this->m_last_mouse_x) * this->m_mouse_sensitivity;
    float yOffset = (this->m_last_mouse_y - y_pos) * this->m_mouse_sensitivity;

    this->m_last_mouse_x = x_pos;
    this->m_last_mouse_y = y_pos;

    this->m_pitch = std::max(std::min(this->m_pitch + yOffset, 89.0f), -89.0f);
    this->m_yaw += xOffset;

    glm::vec3 direction(0.0f);
    direction.x = cos(glm::radians(this->m_yaw)) * cos(glm::radians(this->m_pitch));
    direction.y = sin(glm::radians(this->m_pitch));
    direction.z = sin(glm::radians(this->m_yaw)) * cos(glm::radians(this->m_pitch));
    this->m_front = glm::normalize(direction);
}

void Camera::setMousePosition(glm::vec2 position)
{
    this->m_last_mouse_x = position.x;
    this->m_last_mouse_y = position.y;
}
