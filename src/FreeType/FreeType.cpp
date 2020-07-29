#include "FreeType/FreeType.hpp"

FreeTypeLibrary * FreeTypeLibrary::m_instance = nullptr;

FreeTypeLibrary * FreeTypeLibrary::instance() {
    if (!m_instance) {
        m_instance = new FreeTypeLibrary;
    }
    return m_instance;
}

FreeTypeFace * FreeTypeLibrary::loadFont(string name) {
    if (m_fonts.find(name) == m_fonts.end()) {
        FreeTypeFace * font = new FreeTypeFace(name);
        m_fonts[name] = font;
    }
    return m_fonts[name];
}