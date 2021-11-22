#include "GLObject/Texture.hpp"
#include "stb/stb_image.h"
#include "sb7/sb7ktx.h"
#include "utils/LogManager.hpp"
#include "utils/FileUtils.hpp"

Texture2D::Texture2D() {
    glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &m_texture);
}

void Texture2D::alloc(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height) {
    m_width = width;
    m_height = height;
    glTextureStorage2D(m_texture, levels, internalFormat, width, height);
}

void Texture2D::bindTexture(int textureBinding) {
    // good explanation there
    // https://stackoverflow.com/questions/8866904/differences-and-relationship-between-glactivetexture-and-glbindtexture
    glActiveTexture(GL_TEXTURE0 + textureBinding);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    
    setTexParami(GL_TEXTURE_WRAP_S, GL_REPEAT);   
    setTexParami(GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    setTexParami(GL_TEXTURE_MIN_FILTER, GL_LINEAR); // at least this one, or no texture will take effect...
    setTexParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
    

void Texture2D::setTexParami(GLenum pname, GLenum param) {
    /* GL_REPEAR
     * GL_MIRRED_REPEAT
     * GL_CLAMP_TO_EDGE
     * GL_CLAMP_TO_BORDER
     */
    glTexParameteri(GL_TEXTURE_2D, pname, param);
    
}

void Texture2D::subImage2D(
    GLint level, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height,
    GLenum format, GLenum type, const void * pixels) {
    glTextureSubImage2D(m_texture, level, xOffset, yOffset, width, height, format, type, pixels);
}

void Texture2D::subImage2D(GLint level, GLenum format, GLenum type, const void * pixels) {
    subImage2D(level, 0, 0, m_width, m_height, format, type, pixels);
}

void Texture2D::loadKTX(const char * name) {
    sb7::ktx::file::load(name, m_texture);
}

void Texture2D::loadImage(const char *name) {
    // opengl (0, 0) is left-bottom corner, but
    // most picture stores pixel from up to buttom...
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(name, &width, &height, &nrChannels, 0);
    if (!data || width <= 0 || height <= 0) {  
        INFO("Failed to load texture %s\n", name);
        return;
    }
    INFO("load %s(%d %d %d) success!\n", name, width, height, nrChannels);

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