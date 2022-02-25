#pragma once
#include <string>
#include <vector>
#include <functional>

#include "camera/CameraManager.h"

using std::string;
using std::vector;
using std::function;

class Scene;
class Drawable;
class Program;
struct Touch;
enum class TouchEvent;


class Director {
public:
    using ScheduleFunc = function<void(float)>;

    static Director * instance();
    static CameraManager * getCameraManager();

    void setScene(Scene * scene) {m_scene = scene;}
    Scene* getScene() {return m_scene;}

    string getDefaultFont() {return "package/font/arial.ttf";}
    void draw2D();
    void draw3D();
    void preRender();
    void render();
    void onTouchEvent(Touch * e);

    void addSchedule(ScheduleFunc func) {m_scheduleFuncs.push_back(func);}
    void update(float dt) {for (auto &s: m_scheduleFuncs) {s(dt);}}

    void addObject(Drawable * obj) {models.push_back(obj);}
    vector<Drawable*>& getObjects() {return models;}
    
    void setOverrideShader(bool b) {m_overrideShader = b;}
    bool isOverrideShader() const {return m_overrideShader;}

    float getScreenWidth() const {return m_screenSize.x;}
    float getScreenHeight() const {return m_screenSize.y;}
    glm::vec2 getScreenSize() {return m_screenSize;}
    float getAspect() const {return m_aspect;}
    void setScreenSize(int x, int y);

    glm::vec2 getDesignScreenSize() {return glm::vec2(1280, 720);}


protected:
    Director() {}
    void _renderOneCamera(Camera *);

    static Director * m_instance;

    vector<ScheduleFunc> m_scheduleFuncs;

    Scene * m_scene = nullptr;

    vector<Drawable *> models;

    bool m_overrideShader = false;

    glm::vec2 m_screenSize;
    float m_aspect;
};
