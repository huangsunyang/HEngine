//
// Created by hansen on 2022/2/25.
//

#ifndef HENGINE_IMGUI_H
#define HENGINE_IMGUI_H

#include <string>

using std::string;

class ImGui {
public:
    static bool Button(const string&);
    static void Text(const string&);
};


#endif //HENGINE_IMGUI_H
