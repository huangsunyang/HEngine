#pragma once

class Scene;
struct Touch;


class Director {
public:
    static Director * instance() {
        if (!Director::m_instance) {
            Director::m_instance = new Director();
        }
        return m_instance;
    }

    void setScene(Scene * scene) {m_scene = scene;}
    Scene* getScene() {return m_scene;}

    void draw();
    void onTouchEvent(Touch * e);

protected:
    Director() {}

    static Director * m_instance;
    Scene * m_scene;
};
