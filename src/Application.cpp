#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "My Journey", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR: GLEW INIT" << std::endl;
    }

    {
        float positions[] = {
            -0.5f, -0.5f, //0
             0.5f, -0.5f, //1
             0.0f,  0.5f, //2
        };

        unsigned int indices[] = {

            0, 1, 2
        };

        VertexArray va;
        VertexBuffer vb(positions, 3 * 2 * sizeof(float)); // 4 is number of unrepeating verticies 

        VertexBufferLayout layout;
        layout.Push<float>(2); // 2 is number of coordinates a point has
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 3);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind(); // Binding my shader
        shader.SetUniform4f("u_color", 0.8f, 0.3f, 0.8f, 1.0f); // set data in the shader and retreiving the location of this color uniform


        va.UnBind();
        vb.Unbind();
        ib.Unbind();
        shader.UnBind();

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            // Rendering Here 
            renderer.Clear();

            shader.Bind(); // Binding shader
            shader.SetUniform4f("u_color", r, 0.3f, 0.8f, 1.0f); // r is added to change the color every iteration

            renderer.Draw(va, ib, shader); // Making the openGL draw call 
            
            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }

    } // scope added to terminate the program when the window is closed

    glfwTerminate();
    return 0;
}