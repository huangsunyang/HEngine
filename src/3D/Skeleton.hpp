#pragma once

#include <string>
#include <vector>
#include <3D/Bone.hpp>
#include "3D/Animation.hpp"

using std::string;
using std::vector;
using std::map;

class Skin;

class Skeleton {
public:
    Skeleton(): m_boneTree(nullptr), m_bones(), m_skin(nullptr) {}
    void load(string skeleton_file, string skin_file);
    void loadFromFile(string file);
    void playAnimation(string file);
    void resetAnimation();
    void draw();
    void update(float dt);
    void updatePose(std::shared_ptr<Pose> p);

    Bone * getBoneTree() {return m_boneTree;}
    Bone * getBone(int n) {return m_bones[n];}
    Skin * getSkin() {return m_skin;}

protected:
    void loadAnimation(string file);
    void updateAnimation(float dt);
    void pushBone(string name);
    void popBone();

protected:
    Bone * m_boneTree;
    vector<Bone *> m_bones;
    map<string, Animation *> m_animations;
    float m_curAnimationTime;
    string m_curAnimation;

    Skin * m_skin;
};