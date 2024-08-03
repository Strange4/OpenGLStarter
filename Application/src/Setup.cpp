#include "pch.h"

#include <iostream>

static void handle_error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* param)
{
    const char* _source;
    const char* _type;
    const char* _severity;

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        _source = "API";
        break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        _source = "WINDOW SYSTEM";
        break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        _source = "SHADER COMPILER";
        break;

    case GL_DEBUG_SOURCE_THIRD_PARTY:
        _source = "THIRD PARTY";
        break;

    case GL_DEBUG_SOURCE_APPLICATION:
        _source = "APPLICATION";
        break;

    case GL_DEBUG_SOURCE_OTHER:
        _source = "UNKNOWN";
        break;

    default:
        _source = "UNKNOWN";
        break;
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        _type = "ERROR";
        break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        _type = "DEPRECATED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        _type = "UDEFINED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_PORTABILITY:
        _type = "PORTABILITY";
        break;

    case GL_DEBUG_TYPE_PERFORMANCE:
        _type = "PERFORMANCE";
        break;

    case GL_DEBUG_TYPE_OTHER:
        _type = "OTHER";
        break;

    case GL_DEBUG_TYPE_MARKER:
        _type = "MARKER";
        break;

    default:
        _type = "UNKNOWN";
        break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        _severity = "HIGH";
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
        _severity = "MEDIUM";
        break;

    case GL_DEBUG_SEVERITY_LOW:
        _severity = "LOW";
        break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
        _severity = "NOTIFICATION";
        break;

    default:
        _severity = "UNKNOWN";
        break;
    }

    std::cout << "there was an error" << std::endl;

    fprintf(stderr, "CODE %d: %s of %s severity, raised from %s.\nMessage: %s\n", id, _type, _severity, _source, message);
    __debugbreak(); // MSVC compiler intrisic but it's okay since we're all using it
}

static void resize_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;

    switch (key)
    {
    case GLFW_KEY_ESCAPE: 
        glfwWindowShouldClose(window);
        break;
    default:
        break;
    }
}


bool setup_window(GLFWwindow** window, const std::string& title, int width, int height)
{
    /* Initialize the library */
    if (!glfwInit())
        return false;

    // when running in debug, set the context to debug to see the messages
#ifdef _DEBUG
    std::cout << "Running in debug" << std::endl;
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    // setting the OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // the core profile adds more restrictions

    // Create a windowed mode window and its OpenGL context
    *window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window)
    {
        std::cerr << "There was an error while creating the window" << std::endl;
        glfwTerminate();
        return false;
    }

    glViewport(0, 0, width, height);

    // Make the window's context current
    glfwMakeContextCurrent(*window);

    // swap everytime there can be a new frame and don't swap before
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "There was an error intializing glew" << std::endl;
        glfwTerminate();
        return false;
    }

    // callbacks
    glDebugMessageCallback(handle_error, 0);
    glfwSetFramebufferSizeCallback(*window, resize_callback);
    glfwSetKeyCallback(*window, key_callback);

    glClearColor(1.0f, 0.06f, 0.94f, 1.0f);

    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_VENDOR) << std::endl;
    return true;
}