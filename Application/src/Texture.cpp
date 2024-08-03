#include "pch.h"

#include <stb_image.h>
#include "Texture.h"

Texture::Texture(const std::string& file_path)
    : m_id(0), m_localBuffer(nullptr), m_width(0), m_height(0), m_bitsPerPixel(0)
{
    // flip vertically since (0,0) is bottom left in opengl but top left on PNGs
    stbi_set_flip_vertically_on_load(1);

    // load using rgba
    this->m_localBuffer = stbi_load(file_path.c_str(), &this->m_width, &this->m_height, &this->m_bitsPerPixel, 4);

    glGenTextures(1, &this->m_id);
    glBindTexture(GL_TEXTURE_2D, this->m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->m_width, this->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->m_localBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (this->m_localBuffer)
        stbi_image_free(this->m_localBuffer);
}

Texture::~Texture()
{
    glDeleteTextures(1, &this->m_id);
}

void Texture::bind(GLuint slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->m_id);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
