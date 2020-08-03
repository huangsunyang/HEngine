#include "GLObject/FontAtlas.hpp"

void FontAtlas::loadChar(char ch) {
    if (m_frameInfo.find(ch) != m_frameInfo.end()) {
        return;
    }

    m_face->loadChar(ch);
    
    auto glyph = m_face->handle()->glyph;
    auto width = glyph->bitmap.width;
    auto height = glyph->bitmap.rows;
    auto offsetx = glyph->bitmap_left;
    auto offsety = glyph->bitmap_top;
    auto advance = glyph->advance.x >> 6;
    auto rect = Rect(offsetx, offsety, width, height);

    if (m_penX + width > TEXTURE_SIZE) {
        m_penY += m_lineHeight;
        m_penX = 0;
    }

    if (m_penY + height > TEXTURE_SIZE) {
        // new texture
    }

    rect.advancex = advance;
    rect.offsetx = offsetx;
    rect.offsety = offsety;
    rect.startxp = float(m_penX) / TEXTURE_SIZE;
    rect.startyp = float(m_penY) / TEXTURE_SIZE;
    rect.endxp = float(m_penX + width) / TEXTURE_SIZE;
    rect.endyp = float(m_penY + height) / TEXTURE_SIZE;
    m_frameInfo.insert(std::make_pair(ch, rect));
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    m_texture->subImage2D(0, m_penX, m_penY, width, height, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
    m_penX += advance;
}
