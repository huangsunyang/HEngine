#pragma once

#include <string>
#include <3D/Bone.hpp>

using std::string;

class Skeleton {
public:
    Skeleton(): m_boneTree(nullptr) {}
    void loadFromFile(string file);
    void draw();
    void update();

    Bone * getBoneTree() {return m_boneTree;}

protected:
    void pushBone(string name);
    void popBone();

protected:
    Bone * m_boneTree;    
};