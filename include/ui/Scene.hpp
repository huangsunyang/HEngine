#pragma once
#include "ui/Widget.hpp"
#include "ui/Director.hpp"

using vmath::vec2;

class Scene: public Widget {
public:
    Scene(): Scene({0,0}, {0.5,0.5}) {}
    Scene(vec2 pos, vec2 size): Widget(pos, size, "Scene") {}
    
    void setCurrentScene();
};