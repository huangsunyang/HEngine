#include "3D/Skin.hpp"
#include "utils/StringUtils.hpp"
#include <fstream>

using Utils::string_space_split;
using std::fstream;

void Skin::loadFromFile(string name) {
    fstream f(name, std::ios::in);
    string line;
    while (std::getline(f, line)) {
        
    }
    f.close();
}