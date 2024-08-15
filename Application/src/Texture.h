#pragma once
#include "pch.h"

class Texture
{
private:
    GLuint m_id;
    int m_width, m_height, m_bitsPerPixel;
    std::string m_file_path;

public:
    Texture(const std::string& file_path);
    ~Texture();

    void bind(GLuint slot = 0) const;
    void unbind();

    inline int getWidth() const { return this->m_width; }
    inline int getHeight() const { return this->m_height; }
    inline std::string getFilePath() const { return this->m_file_path; }
};