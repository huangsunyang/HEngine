#ifndef __TEXTURE__
#define __TEXTURE__

#include <GL/gl3w.h>

const int TEXTURE_SHADOW_MAP = 5;

class Texture2D {
public:
    Texture2D();
    ~Texture2D();
    GLuint getHandle() { return m_texture; }
    void alloc(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height);
    virtual void bindTexture(int textureBinding=0);
    void setTexParami(GLenum, GLenum);
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
