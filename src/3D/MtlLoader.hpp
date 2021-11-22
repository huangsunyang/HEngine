#pragma once

#include <map>
#include <string>
#include <fstream>
#include "glm/glm.hpp"

using std::map;
using std::string;
using std::fstream;


struct Material {
    glm::vec3 ka;
    glm::vec3 kd;
    glm::vec3 ks;
    float Tr;
    float Ns;
    string map_Kd;
};


class MtlLoader {
public:
    MtlLoader(string filename);
    ~MtlLoader();
    Material * getMaterial(string mtlname);

protected:
    map<string, Material*> materials;
    Material * curMaterial;

private:
    void parse(fstream& f);
    void addNewMaterial(string name);
    void setCurMaterial(string name);
};