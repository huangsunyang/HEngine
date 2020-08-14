#pragma once

#include <string>

using std::string;

class Skin {
public:
    Skin() {}
    ~Skin() {}
    void loadFromFile(string);
};