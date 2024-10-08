#include "pch.h"

#include <stb_image.h>
#include "Texture.h"

Texture::Texture(const std::string& file_path)
    : m_id(0), m_width(0), m_height(0), m_bitsPerPixel(0), m_file_path(file_path)
{
    // flip vertically since (0,0) is bottom left in opengl but top left on PNGs
    stbi_set_flip_vertically_on_load(true);

    // load using rgba
    unsigned char* local_buffer = stbi_load(file_path.c_str(), &this->m_width, &this->m_height, &this->m_bitsPerPixel, 4);

    glGenTextures(1, &this->m_id);
    glBindTexture(GL_TEXTURE_2D, this->m_id);


    // texture paramters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->m_width, this->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (local_buffer)
        stbi_image_free(local_buffer);
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