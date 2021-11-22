#pragma once
#include "ui/Widget.hpp"
#include "base/Director.hpp"

using glm::vec2;

class Scene: public Widget {
public:
    Scene(): Scene({0,0}, {0.5,0.5}) {}
    Scene(vec2 pos, vec2 size): Widget(pos, size, "Scene") {}
    Scene(string name): Widget(name) {}
    
    void setCurrentScene();
};