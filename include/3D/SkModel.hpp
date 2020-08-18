#pragma once
#include "3D/Skeleton.hpp"
#include "GLObject/Drawable.hpp"
#include "3D/Skin.hpp"


class SkModel {
public:
    SkModel(): m_skin(nullptr), m_skeleton(nullptr) {}
    void load(string skeletonFile, string skinFile); 
    void draw();
    void update();

    Skin * getSkin() {return m_skin;}
    Skeleton * getSkeleton() {return m_skeleton;}

protected:
    Skeleton * m_skeleton;
    Skin * m_skin;
};