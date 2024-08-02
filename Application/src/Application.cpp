#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"
#include "Setup.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

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
    GLFWwindow* window = nullptr;

    if (!setup_window(&window, "Triangle Demo", 640, 480))
        return -1;

    float positions[] = {
        -0.5f,  0.5f, // 0
        -0.5f, -0.5f, // 1
         0.5f, -0.5f, // 2
         0.5f,  0.5f, // 3
    };

    unsigned short vertexIndeces[]{
        0, 1, 2, // first triangle

        2, 3, 0, // second triangle
    };

    
    VertexBuffer vertexBuffer(positions, 4 * 2 * sizeof(float));
    VertexBufferLayout vertexBufferLayout({
        {
            GL_FLOAT, 2, GL_FALSE
        },
    });


    // this contains the buffer and the layout instead of binding both
    VertexArray vertexArray;
    vertexArray.addBuffer(vertexBuffer, vertexBufferLayout);



    //glEnableVertexAttribArray(0);
    // bind the index 0 of the currently bound array buffer to the currently bound vao
    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    IndexBuffer indexBuffer(vertexIndeces, 6);

    ShaderProgram shaderProgram({
        Shader("shaders/fragment.glsl", GL_FRAGMENT_SHADER),
        Shader("shaders/vertex.glsl", GL_VERTEX_SHADER)
    });

    shaderProgram.bind();

    float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    float r_inc = 0.01f;
    float g_inc = 0.01f;
    float b_inc = 0.01f;

    // unbinding everything
    vertexArray.unbind();
    vertexBuffer.unbind();
    indexBuffer.unbind();

    glClearColor(0.6f, 0.0f, 0.0f, 1.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        bounce_color(&r, &r_inc);
        bounce_color(&g, &g_inc);
        bounce_color(&b, &b_inc);

        shaderProgram.setUniform4f("u_color", r, g, b, 1.0f);
        //glUniform4f(colorLocation, r, g, b, 1.0);

        indexBuffer.bind();
        vertexArray.bind();

        glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_SHORT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}