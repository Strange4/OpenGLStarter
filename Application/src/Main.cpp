#include "pch.h"
#include "Setup.h"
#include "Model.h"
#include "Application.h"

int main(void)
{
    GLFWwindow* window = setup_window("Model Viewer", (int) Application::START_WINDOW_WIDTH, (int) Application::START_WINDOW_HEIGHT);
    std::shared_ptr<Model> myModel = std::make_shared<Model>("res/models/teapot.obj");
    Application& app = Application::getInstance();
    app.setWindow(window);
    app.setModel(myModel);
    app.setMovingState();
    app.mainLoop();
    
    return 0;
}
