#include "3D/MtlLoader.hpp"
#include "Utils/StringUtils.hpp"
#include "Utils/FileUtils.hpp"
#include "utils/LogManager.hpp"
#include <sstream>

using std::stringstream;
using namespace Utils;


MtlLoader::MtlLoader(string filename) {
    fstream f;
    f.open(filename, std::ios::in);
    parse(f);
    f.close();
}


Material * MtlLoader::getMaterial(string name) {
    return materials.find(name)->second;
}


void MtlLoader::parse(fstream& f) {
    string line;

    while(getline(f, line)) {
        stringstream lineBuffer(line);
        string head, s0, s1, s2;

        if (string_starts_with(line, "#")) {
            // comments
        } else if (string_starts_with(line, "newmtl")) {
            lineBuffer >> head >> s0;
            setCurMaterial(s0);
        } else if (string_starts_with(line, "map_Kd")) {
            lineBuffer >> head >> s0;
            curMaterial->map_Kd = path_ensure_dir(s0, "package/res");
            INFO("%s\n", curMaterial->map_Kd);
        }

        if (f.eof()) {
            break;
        }
    }
}


void MtlLoader::addNewMaterial(string name) {
    if (materials.find(name) == materials.end()) {
        Material * newMtl = new Material;
        materials[name] = newMtl;
    }
}


void MtlLoader::setCurMaterial(string name) {
    addNewMaterial(name);
    curMaterial = materials.find(name)->second;
}