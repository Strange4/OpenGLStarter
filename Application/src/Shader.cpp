#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>


#include "Shader.h"

Shader::Shader(const std::string& file_path, GLenum shader_type)
    : m_id(0)
{
    const std::string source = Shader::readFromFile(file_path);

    GLuint id = glCreateShader(shader_type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    this->m_id = id;
}

Shader::~Shader()
{
    glDeleteShader(this->m_id);
}

GLuint Shader::getId() const
{
    return this->m_id;
}

std::string Shader::readFromFile(const std::string& file_path)
{
    std::ifstream file(file_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

ShaderProgram::ShaderProgram(const std::vector<Shader> shaders)
    : m_id(0)
{
    GLuint program = glCreateProgram();

    if (program == 0)
    {
        std::cerr << "Failed to create the program!" << std::endl;
        return;
    }

    for (const Shader& shader : shaders)
        glAttachShader(program, shader.getId());

    glValidateProgram(program);
    glLinkProgram(program);

    this->m_id = program;
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(this->m_id);
}

void ShaderProgram::bind() const
{
    glUseProgram(this->m_id);
}

void ShaderProgram::unbind() const
{
    glUseProgram(0);
}

void ShaderProgram::setUniform4f(std::string uniform_name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    if (!this->m_locationCache.contains(uniform_name))
    {
        GLint location = glGetUniformLocation(this->m_id, uniform_name.c_str());
        this->m_locationCache.insert({ uniform_name, location });
    }

    glUniform4f(this->m_locationCache.at(uniform_name), v0, v1, v2, v3);
}
