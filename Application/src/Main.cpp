#include "pch.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Setup.h"
#include "Model.h"
#include "Application.h"


int main(void)
{
    GLFWwindow* window = setup_window("Model Viewer", (int) Application::START_WINDOW_WIDTH, (int) Application::START_WINDOW_HEIGHT);
    if (!window)
        return -1;

    std::shared_ptr<Model> lightCube = std::make_shared<Model>("res/models/cube.obj");
    glm::mat4 lightTransform = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(1.2f, 1.5f, 2.0f)), glm::vec3(0.2f));
    
    lightCube->addTransform(lightTransform);
    std::shared_ptr<Model> coloredCube = std::make_shared<Model>("res/models/cube.obj");
    glm::mat4 cubeTransform = glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Application& app = Application::getInstance();
    app.setWindow(window);
    app.setModel(coloredCube);
    app.setLightModel(lightCube);
    app.setMovingState();
    app.mainLoop();
    
    return 0;
}
