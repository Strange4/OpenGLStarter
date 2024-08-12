#include "pch.h"
#include <iostream>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Setup.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Vertex.h"
#include "Model.h"
#include "Camera.h"

static void bounce_color(float* p_color, float* p_increment)
{
    float increment = *p_increment;
    *p_color += increment;

    // if you've hit the end of the range. bounce it
    if ((*p_color >= 1.0 && increment > 0.0) || (*p_color <= 0.0 && increment < 0.0))
        *p_increment = -increment;
}

int main(void)
{

    constexpr int WINDOW_WIDTH = 640;
    constexpr int WINDOW_HEIGHT = (16 / 9) * WINDOW_WIDTH;
    GLFWwindow* window = setup_window("Triangle Demo", WINDOW_WIDTH, WINDOW_HEIGHT);

    if (!window)
        return -1;

    std::shared_ptr<Model> myModel = std::make_shared<Model>("res/models/cube.obj");

    ShaderProgram shaderProgram({
        Shader("res/shaders/fragment.glsl", GL_FRAGMENT_SHADER),
        Shader("res/shaders/vertex.glsl", GL_VERTEX_SHADER)
    });

    shaderProgram.bind();

    // application stuff
    Renderer renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
    renderer.addModel(myModel);
    Camera camera;

    // model view projection
    glm::vec3 modelTranslation(0.0f);
    float modelRotation = glm::radians(-55.0f);

    float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    float r_inc = 0.01f;
    float g_inc = 0.01f;
    float b_inc = 0.01f;

    bool showDemoWindow = false;

    double previousTime = glfwGetTime();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        Renderer::clear();

        double newTime = glfwGetTime();
        // handle inputs
        camera.handle_keys(window, renderer, (float) (newTime - previousTime));
        previousTime = newTime;

        // ImGui stuff
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(showDemoWindow)
            ImGui::ShowDemoWindow(&showDemoWindow);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Matrix Settings");
            ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our window open/close state

            ImGui::SliderFloat3("Model Translation", glm::value_ptr(modelTranslation), -10.0f, 10.0f);
            ImGui::SliderFloat("Model Rotation", &modelRotation, -glm::two_pi<float>(), glm::two_pi<float>());

            if (ImGui::Button("Button"))
                counter++;
            ImGui::SameLine();

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        bounce_color(&r, &r_inc);
        bounce_color(&g, &g_inc);
        bounce_color(&b, &b_inc);

        shaderProgram.bind();
        shaderProgram.setUniform4f("u_color", r, g, b, 1.0f);

        // contatenating the matrices
        glm::mat4 model = glm::translate(glm::mat4(1.0f), modelTranslation);
        model = glm::rotate(model, modelRotation, glm::vec3(0.0f, 1.0f, 0.0f));
        myModel.get()->setTransform(model);

        renderer.drawModels(shaderProgram);
        
        // render imgui on top
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
