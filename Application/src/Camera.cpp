#include "pch.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

Camera::Camera()
    : m_camera_speed(1.0f), m_front(glm::vec3(0.0f, 0.0f, -3.0f)), m_position(glm::vec3(0.0f))
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

    renderer.setViewTransform(glm::lookAt(this->m_position, this->m_position + this->m_front, this->CAMERA_UP));
}
