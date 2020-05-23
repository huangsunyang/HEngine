#ifndef __TEXTURE__
#define __TEXTURE__

#include <GL/gl3w.h>

class Texture {
public:
    Texture();
    ~Texture();
    void alloc(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height);
    void bindTexture(int textureBinding=0);
    void subImage2D(GLint level, GLenum format, GLenum type, const void * pixels);
    void subImage2D(GLint level, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height,
        GLenum format, GLenum type, const void * pixels);
    void loadKTX(const char * name);
    void loadImage(const char *name);
protected:
    GLuint m_texture;
    GLsizei m_width;
    GLsizei m_height;
};

#endif //__TEXTURE__
