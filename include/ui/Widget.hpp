#pragma once
#include "sb7/vmath.h"
#include "ui/Touch.hpp"
#include <vector>
#include <string>

using std::string;
using std::vector;
using vmath::vec2;
using vmath::vec3;
using vmath::vec4;



class Widget {
public:
    using ClickCallback = void (*)(Widget *, Touch *);
    friend class Director;

    Widget(): Widget({0, 0}, {0.5, 0.5}) {}
    Widget(vec2 pos, vec2 size): Widget(pos, size, "Widget") {}
    Widget(vec2 pos, vec2 size, string name): Widget(pos, size, name, nullptr) {}
    Widget(vec2 pos, vec2 size, string name, Widget * parent): m_pos(pos), m_size(size), m_name(name), m_parent(m_parent) {}
    
    void setPosition(vec2 pos) {m_pos = pos;}
    void setSize(vec2 size) {m_size = size;}
    void setName(string name) {m_name = name;}
    void setVisible(bool visible) {m_visible = visible;}
    void setParent(Widget * parent) {m_parent = parent;}
    void setColor(vec3 color) {m_color = vec4{color, 1.0};}
    void setAlpha(float alpha) {m_color[3] = alpha;}
    void setTouchEnabled(bool enabled) {m_touchEnabled = enabled;}
    void setSwallowTouch(bool swallow) {m_swallowTouch = swallow;}

    vec2 getPosition() {return m_pos;}
    vec2 getWorldPosition() {_refreshWorldPosition(); return m_worldPos;}
    vec2 getSize() {return m_size;}
    string getName() {return m_name;}
    bool isVisible() {return m_visible;}
    Widget * setParent() {return m_parent;}
    vec3 getColor() {return {m_color[0], m_color[1], m_color[2]};}
    float getAlpha() {return m_color[3];}
    bool isTouchEnabled() {return m_touchEnabled;}
    bool isSwallowTouch() {return m_swallowTouch;}

    void addChildren(Widget * child) {m_children.push_back(child);}
    void addTouchEventListener(ClickCallback cb) {m_callback = cb;}
    
    virtual void draw();
    void onTouchEvent(Touch *);

protected:
    void _refreshWorldPosition();
    bool _inTouchArea(vec2 pos);

protected:
    vec2 m_pos;
    vec2 m_worldPos;
    vec2 m_size;
    vec4 m_color = {1.f, 1.f, 1.f, 1.f};
    string m_name = "Widget";
    bool m_visible = true;

    bool m_touchEnabled = true;
    bool m_swallowTouch = false;
    ClickCallback m_callback = nullptr;

    Widget * m_parent = nullptr;
    vector<Widget *> m_children;
};
