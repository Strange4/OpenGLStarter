#include "pch.h"
#include <iostream>
#include <stb_image.h>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Setup.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Vertex.h"

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

    Vertex vertices[]{
        Vertex{-0.5f,  0.5f, 0.0f, 1.0f},
        Vertex{-0.5f, -0.5f, 0.0f, 0.0f},
        Vertex{ 0.5f, -0.5f, 1.0f, 0.0f},
        Vertex{ 0.5f,  0.5f, 1.0f, 1.0f},
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    unsigned short vertexIndeces[]{
        0, 1, 2, // first triangle

        2, 3, 0, // second triangle
    };

    
    VertexBuffer vertexBuffer(vertices, 4 * sizeof(Vertex));
    VertexBufferLayout vertexBufferLayout({
        { GL_FLOAT, 2, GL_FALSE },
        { GL_FLOAT, 2, GL_FALSE },
    });


    // this contains the buffer and the layout instead of binding both
    VertexArray vertexArray;
    vertexArray.addBuffer(vertexBuffer, vertexBufferLayout);

    IndexBuffer indexBuffer(vertexIndeces, 6);

    ShaderProgram shaderProgram({
        Shader("res/shaders/fragment.glsl", GL_FRAGMENT_SHADER),
        Shader("res/shaders/vertex.glsl", GL_VERTEX_SHADER)
    });

    shaderProgram.bind();

    const int TEXTURE_SLOT = 0;
    Texture texture("res/textures/brazil.jpg");
    texture.bind(TEXTURE_SLOT);

    // set the uniform for the texture slot
    shaderProgram.setUniform1i("u_texture_slot", TEXTURE_SLOT);

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

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        Renderer::clear();

        bounce_color(&r, &r_inc);
        bounce_color(&g, &g_inc);
        bounce_color(&b, &b_inc);

        shaderProgram.bind();
        shaderProgram.setUniform4f("u_color", r, g, b, 1.0f);

        Renderer::draw(vertexArray, indexBuffer, shaderProgram);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}