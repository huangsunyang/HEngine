#pragma once
#include <string>
#include <vector>
#include <functional>

using std::string;
using std::vector;
using std::function;

class Scene;
struct Touch;
enum class TouchEvent;


class Director {
public:
    using ScheduleFunc = function<void(float)>;

    static Director * instance() {
        if (!Director::m_instance) {
            Director::m_instance = new Director();
        }
        return m_instance;
    }

    void setScene(Scene * scene) {m_scene = scene;}
    Scene* getScene() {return m_scene;}

    string getDefaultFont() {return "package/font/arial.ttf";}
    void draw();
    void onTouchEvent(Touch * e);

    void addSchedule(ScheduleFunc func) {m_scheduleFuncs.push_back(func);}
    void update(float dt) {for (auto &s: m_scheduleFuncs) {s(dt);}}

protected:
    Director() {}

    vector<ScheduleFunc> m_scheduleFuncs;
    static Director * m_instance;
    Scene * m_scene;
};
