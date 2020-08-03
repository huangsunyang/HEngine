#include "ui/Text.hpp"
#include "ui/Common.hpp"

void Text::drawSelf() {
    // this->Widget::drawSelf();
    static FontAtlas * fontAtlas = new FontAtlas(getFont(), m_fontSize);
    UICharacter temp;
    temp.setTexture({fontAtlas->getTexture()});
    vec2 pos = m_pos, size;
    vector<Quad> quads;
    for (auto ch: m_text) {
        fontAtlas->loadChar(ch);
        auto fontInfo = fontAtlas->getCharSizeInfo(ch);
        auto rectInfo = fontAtlas->getCharRectInfo(ch);
        Quad quad;
        quad.size = {fontInfo.width / 600.0f, fontInfo.height / 600.0f};
        quad.pos = pos + vec2{fontInfo.offsetx / 600.0f, fontInfo.offsety / 600.0f};
        quad.startxp = rectInfo.startxp, quad.startyp = rectInfo.startyp;
        quad.endxp = rectInfo.endxp, quad.endyp = rectInfo.endyp;
        quads.push_back(quad);
        pos[0] += fontInfo.advancex / 600.0f;
    }
    temp.setFontQuads(quads);
    temp.draw();
}