#include "GLObject/Texture.hpp"
#include "stb/stb_image.h"
#include "sb7ktx.h"
#include "LogManager.hpp"
#include "utils/FileUtils.hpp"

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
    /* GL_REPEAR
     * GL_MIRRED_REPEAT
     * GL_CLAMP_TO_EDGE
     * GL_CLAMP_TO_BORDER
     */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
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

void Texture::loadImage(const char *name) {
    string format_str = Utils::file_format(name);

    // don't know why png files are flipped vertically...
    bool need_filp = format_str == ".png";
    INFO("need flip %d\n", need_filp);
    stbi_set_flip_vertically_on_load(need_filp);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(name, &width, &height, &nrChannels, 0);
    if (!data || width <= 0 || height <= 0) {  
        INFO("Failed to load texture\n");
        return;
    }
    INFO("load %s(%d %d %d) success!\n", name, width, height, nrChannels);

    // jpg file must satisfy: 1) width == height 2) be power of 2
    // png file must satisfy: 1) width == height
    // for easy we use stricter rule
    if (width != height || width & (width - 1)) {
        INFO("width has to be equal to height and be power of 2!\n");
        // return;
    }

    // determine channel num to opengl
    GLenum internalFormat, format, type;
    if (nrChannels == 3) {
        internalFormat = GL_RGB8;
        format = GL_RGB;
        type = GL_UNSIGNED_BYTE;
    } else if (nrChannels == 4) {
        internalFormat = GL_RGBA8;
        format = GL_RGBA;
        type = GL_UNSIGNED_BYTE;
    }

    // you could use GL_RGB8 or GL_RGB in glTexImage2D
    // but only GL_RGB8 in glTextureStorage
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    alloc(1, internalFormat, width, height);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    subImage2D(0, format, type, data);
    glGenerateTextureMipmap(m_texture);
    
    stbi_image_free(data);
}