#include "ui/Text.hpp"

void Text::drawSelf() {
    // this->Widget::drawSelf();
    static FontAtlas * fontAtlas = new FontAtlas(getFont(), m_fontSize);
    vec2 pos = m_pos, size;
    for (auto ch: m_text) {
        fontAtlas->loadChar(ch);
        auto rect = fontAtlas->getRectInfo(ch);
        size = {rect.width / 600.0f, rect.height / 600.0f};
        vec2 offset = {rect.offsetx / 600.0f, rect.offsety / 600.0f};
        UICharacter temp(pos + offset, size);
        temp.setTexture({ fontAtlas->getTexture() });
        pos[0] += rect.advancex / 600.0f;
        temp.setFontRect(rect);
        temp.draw();
    }
}