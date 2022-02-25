#include <cmath>
#include <string>
#include "sb7/sb7.h"
#include "sb7/sb7color.h"
#include "ui/shape2d.hpp"
#include <pybind11/embed.h>
#include "GLObject/Light.hpp"
#include "GLObject/UniformBlock.hpp"
#include "camera/Camera.hpp"
#include "ui/Scene.hpp"
#include "ui/Text.hpp"
#include "base/EventDispatcher.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "3D/ObjLoader.hpp"
#include "3D/Terrain.hpp"
#include "DbgHelp.h"
#include "ui/imgui.h"
 
LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo);

// dump
int GenerateMiniDump(PEXCEPTION_POINTERS pExceptionPointers)
{
    // 定义函数指针
    typedef BOOL(WINAPI * MiniDumpWriteDumpT)(
        HANDLE,
        DWORD,
        HANDLE,
        MINIDUMP_TYPE,
        PMINIDUMP_EXCEPTION_INFORMATION,
        PMINIDUMP_USER_STREAM_INFORMATION,
        PMINIDUMP_CALLBACK_INFORMATION
        );
    // 从 "DbgHelp.dll" 库中获取 "MiniDumpWriteDump" 函数
    MiniDumpWriteDumpT pfnMiniDumpWriteDump = nullptr;
    HMODULE hDbgHelp = LoadLibrary("DbgHelp.dll");
    if (nullptr == hDbgHelp)
    {
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
 
    if (nullptr == pfnMiniDumpWriteDump)
    {
        FreeLibrary(hDbgHelp);
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    // 创建 dmp 文件件
    TCHAR szFileName[MAX_PATH] = {0};
    auto szVersion = L"Dump";
    SYSTEMTIME stLocalTime;
    GetLocalTime(&stLocalTime);
    wsprintf(szFileName, "%s-%04d%02d%02d-%02d%02d%02d.dmp",
        szVersion, stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
        stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond);
    HANDLE hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE, 
        FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
    if (INVALID_HANDLE_VALUE == hDumpFile)
    {
        FreeLibrary(hDbgHelp);
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    // 写入 dmp 文件
    MINIDUMP_EXCEPTION_INFORMATION expParam;
    expParam.ThreadId = GetCurrentThreadId();
    expParam.ExceptionPointers = pExceptionPointers;
    expParam.ClientPointers = FALSE;
    pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), 
        hDumpFile, MiniDumpWithDataSegs, (pExceptionPointers ? &expParam : NULL), NULL, NULL);
    // 释放文件
    CloseHandle(hDumpFile);
    FreeLibrary(hDbgHelp);
    return EXCEPTION_EXECUTE_HANDLER;
}
 
LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
{
    // 这里做一些异常的过滤或提示
    if (IsDebuggerPresent())
    {
        return EXCEPTION_CONTINUE_SEARCH;
    }
    return GenerateMiniDump(lpExceptionInfo);
}

// Derive my_application from sb7::application
class my_application : public sb7::application
{
public:
    void startup() override
    {   
        SetUnhandledExceptionFilter(ExceptionFilter);
        left_mouse_down = false;
        middle_mouse_down = false;
        mouse_pos_x = -1;
        mouse_pos_y = -1;
        m_polygonMode = GL_FILL;
        m_pause = false;
        m_gameTime = 0;
        m_lastTickTime = 0;

        // redirect unbuffered STDOUT to the console
        LOG::LogManager::init();
        LOG::LogManager::showAllLogger();

        initUI();
        initEvent();

        // init camera
        auto camera = new Camera;
        camera->setCameraPos(10, 40, 40);
        camera->lookAt(0, 0, 0);

        auto c = new Camera;
        c->setCameraPos(50, 50, 50);
        c->lookAt(0, 0, 0);
        c->setViewPos(0.8, 0.8);
        c->setViewSize(.2, .2);

        m_lightCamera = new Camera;
        m_lightCamera->setPerspective(false);
        m_lightCamera->setCameraPos(5, 5, 5);
        m_lightCamera->lookAt(1, 0, 2);
        m_lightCamera->setViewSize(0.2, 0.2);
        m_lightCamera->setViewPos(0.1, 0.1);

        init_shape();
        onResize(info.windowWidth, info.windowHeight);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void initUI() {
        auto * scene = new Scene("package/ui/scene_test.xml");
        scene->addTouchEventListener([this](Widget * w, Touch * touch) {
            INFO("---------------------\n");
        });
        scene->setCurrentScene();

        Text * text = new Text({0, 0}, {1.5f, 1.5f}, "text", scene);
        text->setParent(scene);
        text->setText("He");
        text->setFont("package/font/consolab.ttf");
        text->setColor({0, 0, 1});
        text->setFontSize(100);

        EventDispatcher::instance()->registerKeyDownEvent(GLFW_KEY_A, [text](){
           text->setText(text->getText() + "A");
        });
    }

    void initEvent() {
        EventDispatcher::instance()->registerKeyDownEvent(GLFW_KEY_F, [this](){switchFullScreen();});
        EventDispatcher::instance()->registerKeyDownEvent(GLFW_KEY_W, [this](){switchWireframeMode();});
        EventDispatcher::instance()->registerKeyDownEvent(GLFW_KEY_SPACE, [this](){m_pause = !m_pause;});
        EventDispatcher::instance()->registerKeyDownEvent(GLFW_KEY_T, [this](){
            auto cameras = CameraManager::getInstance()->getCameras();
            m_controlingCamera = (m_controlingCamera + 1) % cameras.size();
        });
    }

    void init_shape() {
        auto director = Director::instance();

        auto * obj = new ObjLoader("Package/res/capsule.obj");
        obj->setShader({"Package/shader/common_light.vs", "Package/shader/common_light.fs"});
        director->addObject(obj);

        auto * axis = new HPolygon();
        axis->setShader({"Package/shader/axis.vs", "Package/shader/common.fs"});
        axis->setDrawMode(GL_LINES);
        axis->loadVertexCoord({
            10., .0, .0, 
            .0, .0, .0, 
            .0, .0, 10.0,
            .0, .0, .0,
            .0, 10.0, 0.,
            .0, .0, .0
        }, {}, {});
        director->addObject(axis);

        Light * light = LightMgr::instance()->createLight();
        light->getTransform()->setPosition({0, 5, 0});
        auto dir = light->getTransform()->getForward();
        director->addObject(light);
        director->addObject(LightMgr::instance()->createLight());

        auto * box = new BoxSolid(2, 2, 2);
        box->getTransform()->setPosition(0, 0, 4);
        box->setShader({"Package/shader/common_light.vs", "package/shader/common_light.fs"});
        box->setTexture({"Package/res/wall.jpg"});
        Director::instance()->addObject(box);

        auto * box1 = new BoxSolid(2, 2, 2);
        box1->getTransform()->setPosition(4, 4, 4);
        box1->setShader({"Package/shader/common_light.vs", "package/shader/common_light.fs"});
        box1->setTexture({"Package/res/wall.jpg"});
        Director::instance()->addObject(box1);

        auto * box2 = new BoxSolid(50, 0.1, 50);
        box2->getTransform()->setPosition(0, -1, 0);
        box2->setShader({"Package/shader/common_light.vs", "package/shader/common_light.fs"});
        box2->setTexture({"Package/res/wall.jpg"});
        Director::instance()->addObject(box2);

    }

    void onMouseButton(int button, int action) override {
        auto touch = new Touch;
        auto width = info.windowWidth, height = info.windowHeight;
        if (button == GLFW_MOUSE_BUTTON_1) {
            touch->pos = {float(mouse_pos_x) / info.windowWidth * 2 - 1, - float(mouse_pos_y) / info.windowHeight * 2 + 1};
            if (action == GLFW_PRESS) {
                left_mouse_down = true;
                touch->event = TouchEvent::BEGAN;
                Director::instance()->onTouchEvent(touch);
            } else if (action == GLFW_RELEASE) {
                touch->event = TouchEvent::END;
                left_mouse_down = false;
                Director::instance()->onTouchEvent(touch);
            }
        } else if (button == GLFW_MOUSE_BUTTON_3) {
            if (action == GLFW_PRESS) {
                middle_mouse_down = true;
            } else if (action == GLFW_RELEASE) {
                middle_mouse_down = false;
                mouse_pos_x = mouse_pos_y = -1;
            }
        }
    }

    void onMouseMove(int x, int y) override {
        auto camera = getControlCamera();
        if (left_mouse_down) {
            if (!(mouse_pos_x < 0 && mouse_pos_y < 0)) {
                float diff_x = float(x - mouse_pos_x) / 1000.0f; 
                float diff_y = float(y - mouse_pos_y) / -1000.0f;
                camera->rotateCameraBy(diff_x, diff_y);
            }
        }
        if (middle_mouse_down) {
            if (!(mouse_pos_x < 0 && mouse_pos_y < 0)) {
                float diff_x = float(x - mouse_pos_x) / -50.0f;
                float diff_y = float(y - mouse_pos_y) / 50.0f;
                camera->moveCameraBy(diff_x, diff_y, 0);
            }
        }
        mouse_pos_x = x, mouse_pos_y = y;
    }

    void onKey(int key, int action) override {
        if (action == GLFW_PRESS) {
            EventDispatcher::instance()->dispatchKeyDownEvent(key);
        } else if (action == GLFW_RELEASE) {
            EventDispatcher::instance()->dispatchKeyUpEvent(key);
        }
    }

    Camera * getControlCamera() const {
        auto cameras = CameraManager::getInstance()->getCameras();
        return cameras[m_controlingCamera % cameras.size()];
    }

    void onMouseWheel(int pos) override {
        auto camera = getControlCamera();
        camera->moveCameraBy(0, 0, pos * 0.5f);
    }

    void onResize(int w, int h) override {
        sb7::application::onResize(w, h);
        glViewport(0, 0, w, h);
        Director::instance()->setScreenSize(w, h);
    }

    bool isFullscreen() {
        return glfwGetWindowMonitor(window) != nullptr;
    }

    void switchFullScreen() {
        if (!isFullscreen()) {
            // backup window position and window size
            glfwGetWindowPos(window, &_wndPos[0], &_wndPos[1]);
            glfwGetWindowSize(window, &_wndSize[0], &_wndSize[1]);

            // get reolution of monitor
            const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

            // switch to full screen
            glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, 0);
            glViewport(0, 0, mode->width, mode->height);
        } else {
            // restore last window size and position
            glfwSetWindowMonitor(window, nullptr,  _wndPos[0], _wndPos[1], _wndSize[0], _wndSize[1], 0 );
            glViewport(0, 0, _wndSize[0], _wndSize[1]);
        }
    }

    void switchWireframeMode() {
        if (m_polygonMode == GL_LINE) {
            m_polygonMode = GL_FILL;
        } else {
            m_polygonMode = GL_LINE;
        }
        for (auto model: Director::instance()->getObjects()) {
            model->setPolygonMode(m_polygonMode);
        }
    }

    void shutdown() override
    {
    }

    void run(application * app) override {
        pybind11::scoped_interpreter guard{};
        pybind11::module::import("sys").attr("path").cast<pybind11::list>().append("./package/script");
        this->application::run(this);
    }

    void logic() const {
        float f = (float)m_gameTime * 0.3f;
        glm::vec3 translate = glm::vec3(
            sinf(2.1f * f) * 5,
            cosf(2.7f * f) * 5,
            sinf(2.3f * f) * 5
        );
        glm::vec3 translate1 = glm::vec3(
            cosf(2.9f * f) * 3,
            sinf(1.0f * f) * 3,
            sinf(1.5f * f) * 3
        );
        glm::vec3 rotate = translate * 50.0f;
        LightMgr::instance()->getLight(0)->getTransform()->setPosition(translate);
        LightMgr::instance()->getLight(1)->getTransform()->setPosition(translate1);
    }

    // Our rendering function
    void render(double currentTime) override
    {
        if (!gl3wIsSupported(4, 3)) return;
        if (!m_pause) {
            m_gameTime += currentTime - m_lastTickTime;
        }
        
        // logic stuff
        Director::instance()->update(float(currentTime - m_lastTickTime));
        auto python_module = pybind11::module::import("python");
        python_module.attr("logic")(currentTime);
        logic();
        m_lastTickTime = currentTime;

        // shadowmap
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // draw shadowmap
        auto camera_matrix = m_lightCamera->getCameraTransform();
        auto proj_matrix = m_lightCamera->getProjectionMatrix();
        UniformBlock::instance()->setUniformBlockMember("light_view_matrix", glm::value_ptr(camera_matrix));
        UniformBlock::instance()->setUniformBlockMember("light_proj_matrix", glm::value_ptr(proj_matrix));

        auto texture = m_lightCamera->getDepthTexture();
        texture->bindTexture(TEXTURE_SHADOW_MAP);

        Director::instance()->preRender();
//        Director::instance()->render();

        // ui
//        Director::instance()->draw2D();

        ImGui::Button("imgui text");
        ImGui::Text("imgui text");
    }

private:
    float aspect;
    bool left_mouse_down = false;
    bool middle_mouse_down = false;
    int _wndPos[2];
    int _wndSize[2];
    int mouse_pos_x = -1;
    int mouse_pos_y = -1;
    Camera * m_lightCamera;
    int m_controlingCamera = 0;

    GLenum m_polygonMode;

    bool m_pause;
    double m_gameTime;
    double m_lastTickTime;
};

// Our one and only instance of DECLARE_MAIN
DECLARE_MAIN(my_application);
