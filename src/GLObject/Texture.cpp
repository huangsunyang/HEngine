#include "GLObject/Texture.hpp"
#include "stb/stb_image.h"
#include "sb7ktx.h"
#include "LogManager.hpp"

Texture::Texture() {
    glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
}

void Texture::alloc(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height) {
    m_width = width;
    m_height = height;
    glTextureStorage2D(m_texture, levels, internalFormat, width, height);
}

void Texture::bindTexture(int textureBinding) {
    glActiveTexture(GL_TEXTURE0 + textureBinding);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // at least this one, or no texture will take effect...
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::subImage2D(
    GLint level, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height,
    GLenum format, GLenum type, const void * pixels) {
    glTextureSubImage2D(m_texture, level, xOffset, yOffset, width, height, format, type, pixels);
}

void Texture::subImage2D(GLint level, GLenum format, GLenum type, const void * pixels) {
    subImage2D(level, 0, 0, m_width, m_height, format, type, pixels);
}

void Texture::loadKTX(const char * name) {
    sb7::ktx::file::load(name, m_texture);
}

void Texture::loadImage(GLenum internalFormat, GLenum format, GLenum type, const char *name) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(name, &width, &height, &nrChannels, 0);
    if (data)
    {   
        // you could use GL_RGB8 or GL_RGB in glTexImage2D
        // but only GL_RGB8 in glTextureStorage
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        alloc(1, internalFormat, width, height);
        subImage2D(0, format, type, data);
        glGenerateTextureMipmap(m_texture);
    }
    else
    {
        INFO("Failed to load texture");
    }
    stbi_image_free(data);
}