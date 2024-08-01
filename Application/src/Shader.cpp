#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>


#include "Shader.h"

/// <summary>
/// Compiles the shader and returns the id of the shader
/// </summary>
/// <param name="type">the type of shader to be compiled</param>
/// <param name="source">the source code of the shader</param>
/// <returns>the id of the shader</returns>
static GLuint compile_shader(GLenum type, const std::string& source)
{
    GLuint id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        // stack allocated message
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cerr << "Failed to compile shader!" << std::endl;

        std::cerr << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

int create_shader(const std::string& vertex_shader_src, const std::string& fragment_shader_src)
{
    GLuint program = glCreateProgram();

    if (program == 0)
    {
        std::cerr << "Failed to create the program!" << std::endl;
        return -1;
    }
    
    GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_shader_src);
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_src);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);


    // we can delete the shader after we link them into the program
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

std::string read_from_file(const char* file_name)
{
    std::ifstream file(file_name);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
