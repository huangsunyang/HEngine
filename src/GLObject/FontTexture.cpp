#include "GLObject/FontTexture.hpp"

void FontTexture::loadFont(string font, char c, int height) {
    auto face = FreeTypeLibrary::instance()->loadFont(font, height);
    face->loadChar(c);
    m_width = face->handle()->glyph->bitmap.width;
    m_height = face->handle()->glyph->bitmap.rows;
    m_posx = face->handle()->glyph->bitmap_left;
    m_posy = face->handle()->glyph->bitmap_top;
    m_advance = face->handle()->glyph->advance.x;
    alloc(1, GL_R8, m_width, m_height);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    subImage2D(0, GL_RED, GL_UNSIGNED_BYTE, face->handle()->glyph->bitmap.buffer);
    glGenerateTextureMipmap(m_texture);
}