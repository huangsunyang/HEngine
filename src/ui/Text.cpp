#include "ui/Text.hpp"

void Text::drawSelf() {
    this->Widget::drawSelf();
    for (auto ch: m_text) {
        UICharacter temp(m_pos, m_size);
        temp.setFontTexture("package/font/arial.ttf", ch);
        temp.draw();
    }
}