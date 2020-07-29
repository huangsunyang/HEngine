#pragma once
#include "ui/Widget.hpp"
#include "GLObject/FontTexture.hpp"
#include "GLObject/Geometry.hpp"

class Text: public Widget {
public:
    Text(): Widget() {}
    Text(vec2 pos, vec2 size): Widget(pos, size) {}
    Text(vec2 pos, vec2 size, string name): Widget(pos, size, name) {}
    Text(vec2 pos, vec2 size, string name, Widget * parent): Widget(pos, size, name, m_parent) {}

public:
    void setText(string text) {m_text = text;}
    void setFont(string font) {m_font = font;}

    string getText() {return m_text;}
    string getFont() {return m_font;}

    virtual void drawSelf();

protected:
    string m_text;
    string m_font;
    vector<UICharacter *> m_charDrawers;
};