#pragma once

#include <map>
#include <string>
#include <fstream>
#include "sb7/vmath.h"

using std::map;
using std::string;
using std::fstream;


struct Material {
    vmath::vec3 ka;
    vmath::vec3 kd;
    vmath::vec3 ks;
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