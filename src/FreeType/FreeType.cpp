#include "FreeType/FreeType.hpp"

FreeTypeLibrary * FreeTypeLibrary::m_instance = nullptr;

FreeTypeLibrary * FreeTypeLibrary::instance() {
    if (!m_instance) {
        m_instance = new FreeTypeLibrary;
    }
    return m_instance;
}

FreeTypeFace * FreeTypeLibrary::loadFont(string name, int height) {
    if (m_fonts[name].find(height) == m_fonts[name].end()) {
        FreeTypeFace * font = new FreeTypeFace(name, height);
        m_fonts[name][height] = font;
    }
    return m_fonts[name][height];
}