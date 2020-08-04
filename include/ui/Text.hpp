#pragma once
#include "ui/Director.hpp"
#include "ui/Widget.hpp"
#include "GLObject/Geometry.hpp"
#include "GLObject/FontAtlas.hpp"

class Text: public Widget {
public:
    Text(): Widget() {}
    Text(vec2 pos, vec2 size): Widget(pos, size) {}
    Text(vec2 pos, vec2 size, string name): Widget(pos, size, name) {}
    Text(vec2 pos, vec2 size, string name, Widget * parent): Widget(pos, size, name, m_parent) {}

public:
    void setText(string text) {m_text = text;}
    void setFont(string font) {m_font = font;}
    void setFontSize(int size) {m_fontSize = size;}

    string getText() {return m_text;}
    string getFont() {return m_font.empty() ? Director::instance()->getDefaultFont() : m_font;}
    int getFontSize() {return m_fontSize;}

    virtual void drawSelf();

protected:
    string m_text;
    string m_font;
    FontAtlas * m_fontAtlas;
    int m_fontSize = 20;
};