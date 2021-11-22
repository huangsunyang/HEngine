#pragma once

#include <map>
#include <string>
#include "utils/LogManager.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

using std::map;
using std::string;

class FreeTypeFace;

class FreeTypeLibrary {
public:
    friend class FreeTypeFace;

    static FreeTypeLibrary * instance();
    FreeTypeFace * loadFont(string name, int size);

protected:
    FreeTypeLibrary() {
        if (FT_Init_FreeType(&m_library)) {
            INFO("init free type failed!!\n");
            return;
        }
    }

protected:
    static FreeTypeLibrary * m_instance;

    FT_Library m_library;
    map<string, map<int, FreeTypeFace *>> m_fonts;
};


class FreeTypeFace {
public:
    FreeTypeFace(string name, int height) {
        auto library = FreeTypeLibrary::instance()->m_library;
        if (FT_New_Face(library, name.c_str(), 0, &m_face)) {
            INFO("init font %s failed!!", name.c_str());
            return;
        }
        FT_Set_Pixel_Sizes(m_face, 0, height);
    }

    void loadChar(char c) {
        if (FT_Load_Char(m_face, c, FT_LOAD_RENDER)) {
            INFO("load char %c failed!!!", c);
        }
    }

    FT_Face handle() {return m_face;}

protected:
    FT_Face m_face;
};
