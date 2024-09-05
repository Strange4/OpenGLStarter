#pragma once

#include "pch.h"
#include <unordered_map>

class Shader
{
private:
    GLuint m_id;
    GLenum m_type;
    std::string m_path;
public:
    Shader(const std::string& file_path, GLenum shader_type);

    GLuint getId() const { return this->m_id; };
    GLenum getType() const { return this->m_type; }

private:
    /// <summary>
    /// Reads the file from the file system and returns it as text
    /// </summary>
    /// <param name="file_path">the path of the file</param>
    /// <returns>the text inside the file</returns>
    static std::string readFromFile(const std::string& file_path);
};



class ShaderProgram
{
private:
    GLuint m_id;
    std::vector<Shader> m_shaders;
    std::unordered_map<std::string, GLuint> m_locationCache;

public:
    ShaderProgram(const std::vector<Shader>& shaders);
    ShaderProgram();
    ~ShaderProgram();

    GLuint getId() const { return this->m_id; }

    std::vector<Shader> getShaders();

    void deleteShaderType(GLenum type);

    void attachShader(Shader shader);

    void attachAllShaders(const std::vector<Shader>& shaders);
    void link() const;

    /// <summary>
    /// binds this shader program to the opengl context
    /// </summary>
    void bind() const;

    /// <summary>
    /// removes the binding of any program to this opengl context
    /// </summary>
    void unbind() const;

    /// <summary>
    /// Caches and binds the uniform with the given name to the specified values.
    /// </summary>
    /// <param name="uniform_name">the name of the uniform as stated in the shader</param>
    void setUniform1f(const std::string& uniform_name, GLfloat v0);
    void setUniform3f(const std::string& uniform_name, GLfloat v0, GLfloat v1, GLfloat v2);
    void setUniform3f(const std::string& uniform_name, const glm::vec3& vec);
    void setUniform4f(const std::string& uniform_name, const glm::vec4& vec);
    void setUniform4f(const std::string& uniform_name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

    /// <summary>
    /// Caches and binds the uniform with the given name to the specified values.
    /// Note: The program must be bound or else OpenGL will throw an error
    /// </summary>
    /// <param name="uniform_name">the name of the uniform as stated in the shader</param>
    /// <param name="value">the value of the uniform</param>
    void setUniform1i(const std::string& uniform_name, GLint value);

    void setUniformMatrix3f(const std::string& uniform_name, const glm::mat3& matrix);
    void setUniformMatrix4f(const std::string& uniform_name, const glm::mat4& matrix);

private:
    GLint getUniformLocation(const std::string& uniform_name);
};