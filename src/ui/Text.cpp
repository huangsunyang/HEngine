#include "ui/Text.hpp"

void Text::drawSelf() {
    // this->Widget::drawSelf();
    auto pos = m_pos;
    for (auto ch: m_text) {
        UICharacter temp(pos, m_fontSize);
        temp.setFontTexture(getFont(), ch);
        temp.draw();
        pos += vec2{temp.getFontWidth(), 0};
    }
}