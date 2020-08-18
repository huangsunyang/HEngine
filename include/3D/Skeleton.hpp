#pragma once

#include <string>
#include <vector>
#include <3D/Bone.hpp>

using std::string;
using std::vector;

class Skeleton {
public:
    Skeleton(): m_boneTree(nullptr), m_bones() {}
    void loadFromFile(string file);
    void draw();
    void update();

    Bone * getBoneTree() {return m_boneTree;}
    Bone * getBone(int n) {return m_bones[n];}

protected:
    void pushBone(string name);
    void popBone();

protected:
    Bone * m_boneTree;
    vector<Bone *> m_bones;
};