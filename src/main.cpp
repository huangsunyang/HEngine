#include <math.h>
#include <string>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <streambuf>
#include "sb7/sb7.h"
#include "sb7/sb7color.h"
#include "shape2d.hpp"
#include "utils/LogManager.hpp"
#include <pybind11/embed.h>
#include "GLObject/Program.hpp"
#include "GLObject/Texture.hpp"
#include "GLObject/Light.hpp"
#include "GLObject/UniformBlock.hpp"
#include "Camera.hpp"
#include "ui/scene.hpp"
#include "ui/Text.hpp"
#include "ui/ParticleSystem.hpp"
#include "base/EventDispatcher.hpp"
#include "3D/Skeleton.hpp"
#include "3D/Skin.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "ObjLoader.hpp"
#include "3D/Terrain.hpp"
#include "GLObject/FrameBuffer.hpp"


#include "DbgHelp.h"
 
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
    MiniDumpWriteDumpT pfnMiniDumpWriteDump = NULL;
    HMODULE hDbgHelp = LoadLibrary("DbgHelp.dll");
    if (NULL == hDbgHelp)
    {
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
 
    if (NULL == pfnMiniDumpWriteDump)
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
    void startup()
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
        m_camera = new Camera;
        m_camera->setCameraPos(10, 40, 40);
        m_camera->lookAt(0, 0, 0);

        m_lightCamera = new Camera;
        m_lightCamera->setPerspective(false);
        m_lightCamera->setCameraPos(5, 5, 5);
        m_lightCamera->lookAt(1, 0, 2);

        // init lights
        m_lightMgr = new LightMgr;
        
        /*
        shader_mgr.bind_shader(GL_TESS_CONTROL_SHADER, "tess_control_shader");
        shader_mgr.bind_shader(GL_TESS_EVALUATION_SHADER, "tess_evaluation_shader");
        shader_mgr.bind_shader(GL_GEOMETRY_SHADER, "geometry_shader");
        */
        init_shape();
        onResize(info.windowWidth, info.windowHeight);
        init_framebuffer();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void initUI() {
        Scene * scene = new Scene("package/ui/scene_test.xml");
        scene->addTouchEventListener([this](Widget * w, Touch * touch) {
            INFO("---------------------\n");
        });
        scene->setCurrentScene();

        Text * text = new Text({0, 0}, {1.5f, 1.5f}, "text", scene);
        //text->setParent(scene);
        text->setText("Hello World!");
        text->setFont("package/font/consolab.ttf");
        text->setColor({0, 0, 1});
        text->setFontSize(100);

        // Widget * widget = new Widget;
        // widget->setParent(scene);
        // widget->setShader({"Package/shader/ui.vs", "Package/shader/ui/ui_blink.fs"});
        // widget->loadTexture("Package/res/sword.png");
        // Director::instance()->addSchedule(
        //     [this, widget](float dt) {
        //         widget->getProgram()->setFloatUniform("sys_time", float(m_gameTime)); 
        //     }
        // );

        // ParticleSystem * p = new ParticleSystem({0, 0}, {1.5f, 1.5f}, "Particle Emitter", scene);
        // p->initWithFile("");
        // p->setParent(scene);
    }

    void initEvent() {
        EventDispatcher::instance()->registerKeyDownEvent(GLFW_KEY_F, [this](){switchFullScreen();});
        EventDispatcher::instance()->registerKeyDownEvent(GLFW_KEY_W, [this](){switchWireframeMode();});
        EventDispatcher::instance()->registerKeyDownEvent(GLFW_KEY_SPACE, [this](){m_pause = !m_pause;});
        EventDispatcher::instance()->registerKeyDownEvent(GLFW_KEY_DOWN, [this](){
            sk->getSkin()->setMorphBlend("package/res/head/head2.morph", sk->getSkin()->getMorphBlend("package/res/head/head2.morph") - 0.05f);
            sk->update(0.03f);
        });

        EventDispatcher::instance()->registerKeyDownEvent(GLFW_KEY_UP, [this](){
            sk->getSkin()->setMorphBlend("package/res/head/head2.morph", sk->getSkin()->getMorphBlend("package/res/head/head2.morph") + 0.05f);
            sk->update(0.03f);
        });
        EventDispatcher::instance()->registerKeyDownEvent(GLFW_KEY_R, [this](){sk->resetAnimation();});
        EventDispatcher::instance()->registerKeyDownEvent(GLFW_KEY_T, [this](){m_controlLightCamera = !m_controlLightCamera;});

    }

    void init_shape() {
        auto director = Director::instance();

        auto terrain = new Terrain;
        director->addObject(terrain);
        EventDispatcher::instance()->registerKeyDownEvent(GLFW_KEY_UP, [terrain]() {
            terrain->addHeight(5.0f);
        });
        EventDispatcher::instance()->registerKeyDownEvent(GLFW_KEY_DOWN, [terrain]() {
            terrain->addHeight(-5.0f);
        });
        EventDispatcher::instance()->registerKeyDownEvent(GLFW_KEY_LEFT, [terrain]() {
            terrain->addDiff(1.5f);
        });
        EventDispatcher::instance()->registerKeyDownEvent(GLFW_KEY_RIGHT, [terrain]() {
            terrain->addDiff(-1.5f);
        });

        // sk = new Skeleton();
        // sk->load("package/res/wasp/wasp.skel", "package/res/wasp/wasp.skin");
        // // sk->getSkin()->loadMorph("package/res/head/head2.morph", 1.0f);
        // sk->playAnimation("package/res/wasp/wasp_walk.anim");
        // sk->update(0.03f);

        HPolygon * triangle = new HPolygon();
        triangle->setShader({"Package/shader/texture.fs", "Package/shader/texture.vs"});
        triangle->setTexture({"Package/res/awesomeface.png", "Package/res/wall.jpg", "Package/res/timg.jpg"});
        triangle->loadVertexCoord({-1, -1, 0, 1, -1, 0, -1, 1, 0}, {0, 0, 1, 0, 0, 1}, {});
        // director.addObject(triangle);

        // auto m_drawer = new UIRectangle({0, 0}, {1, 1});
        // m_drawer->setTexture({"Package/res/awesomeface.png", "Package/res/wall.jpg", "Package/res/timg.jpg"});
        // director->addObject(m_drawer);

        ObjLoader * obj = new ObjLoader("Package/res/capsule.obj");
        obj->setShader({"Package/shader/common_light.vs", "Package/shader/common_light.fs"});
        // director->addObject(obj);

        HPolygon * axis = new HPolygon();
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

        Light * light = m_lightMgr->createLight();
        light->getTransform()->setPosition({0, 5, 0});
        auto dir = light->getTransform()->getForward();
        //DEBUG("light forward: %lf %lf %lf\n", dir[0], dir[1], dir[2]);
        director->addObject(light);
        director->addObject(m_lightMgr->createLight());

        HPolygon * polygon = new HPolygon();
        polygon->setShader({"Package/shader/ui.vs", "package/shader/common.fs"});
        polygon->setDrawMode(GL_TRIANGLE_FAN);
        polygon->loadVertexCoord({
            .25, .0, .0, 
            .75, .0, .0, 
            1.0, .5, .0,
            0.75, 1.0, .0,
            0.25, 1.0, .0,
            .0, .5, .0
        }, {}, {});
        // director->addObject(polygon);

        m_screen = new HPolygon();
        m_screen->setShader({"Package/shader/ui.vs", "package/shader/postprocess/color.fs"});
        m_screen->setDrawMode(GL_TRIANGLE_STRIP);
        m_screen->loadVertexCoord({
            -1, +1, 0,
            -1, -1, 0,
            +1, +1, 0,
            +1, -1, 0,
        }, {
            0, 1,
            0, 0,
            1, 1,
            1, 0,
        }, {});

        m_depth = new HPolygon();
        m_depth->setShader({"Package/shader/ui.vs", "package/shader/postprocess/depth.fs"});
        m_depth->setDrawMode(GL_TRIANGLE_STRIP);
        m_depth->loadVertexCoord({
            -0.9f, +0.9f, 0,
            -0.9f, +0.4f, 0,
            -0.4f, +0.9f, 0,
            -0.4f, +0.4f, 0,
        }, {
            0, 1,
            0, 0,
            1, 1,
            1, 0,
        }, {});

        BoxSolid * box = new BoxSolid(2, 2, 2);
        box->setShader({"Package/shader/common_light.vs", "package/shader/common_light.fs"});
        box->setTexture({"Package/res/wall.jpg"});
        Director::instance()->addObject(box);

        BoxSolid * box1 = new BoxSolid(2, 2, 2);
        box1->getTransform()->setPosition(4, 4, 4);
        box1->setShader({"Package/shader/common_light.vs", "package/shader/common_light.fs"});
        box1->setTexture({"Package/res/wall.jpg"});
        Director::instance()->addObject(box1);

        BoxSolid * box2 = new BoxSolid(50, 0.1, 50);
        box2->getTransform()->setPosition(0, -1, 0);
        box2->setShader({"Package/shader/common_light.vs", "package/shader/common_light.fs"});
        box2->setTexture({"Package/res/wall.jpg"});
        Director::instance()->addObject(box2);

    }

    void init_framebuffer() {
        frameBuffer = new FrameBuffer;
        // must bind framebuffer here!!, don't know why
        frameBuffer->bind(GL_FRAMEBUFFER);

        Texture2D * texture = new Texture2D;
        texture->alloc(1, GL_RGBA8, info.windowWidth, info.windowHeight);
        m_screen->setTexture({texture});

        Texture2D * depth_texture = new Texture2D;
        depth_texture->alloc(1, GL_DEPTH_COMPONENT32F, info.windowWidth, info.windowHeight);
        depth_texture->bindTexture(0);

        frameBuffer->bindTexture(GL_COLOR_ATTACHMENT0, texture, 0);
        frameBuffer->bindTexture(GL_DEPTH_ATTACHMENT, depth_texture, 0);
        frameBuffer->drawBuffer(GL_COLOR_ATTACHMENT0);

        Texture2D * shadow_map_depth = new Texture2D;
        shadow_map_depth->alloc(1, GL_DEPTH_COMPONENT32F, 1024, 1024);
        m_depth->setTexture({shadow_map_depth});

        shadowMap = new FrameBuffer;
        shadowMap->bind(GL_FRAMEBUFFER);
        shadowMap->bindTexture(GL_DEPTH_ATTACHMENT, shadow_map_depth, 0);
        shadowMap->drawBuffer(GL_DEPTH_ATTACHMENT);
    }

    void onMouseButton(int button, int action) {
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

    void onMouseMove(int x, int y) {
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

    void onKey(int key, int action) {
        if (action == GLFW_PRESS) {
            EventDispatcher::instance()->dispatchKeyDownEvent(key);
        } else if (action == GLFW_RELEASE) {
            EventDispatcher::instance()->dispatchKeyUpEvent(key);
        }
    }

    Camera * getControlCamera() {
        return m_controlLightCamera ? m_lightCamera : m_camera;
    }

    void onMouseWheel(int pos) {
        auto camera = getControlCamera();
        camera->moveCameraBy(0, 0, pos * 0.5f);
    }

    void onResize(int w, int h) {
        sb7::application::onResize(w, h);
        glViewport(0, 0, w, h);
        aspect = (float)info.windowWidth / (float)info.windowHeight;
        m_camera->setAspect(aspect);
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
        if (sk) {
            sk->getSkin()->setPolygonMode(m_polygonMode);
        }
    }

    void shutdown()
    {
    }

    virtual void run(application * app) {
        pybind11::scoped_interpreter guard{};
        pybind11::module::import("sys").attr("path").cast<pybind11::list>().append("./package/script");
        this->application::run(this);
    }

    void logic() {
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
        m_lightMgr->getLight(0)->getTransform()->setPosition(translate);
        m_lightMgr->getLight(1)->getTransform()->setPosition(translate1);
    }

    // Our rendering function
    void render(double currentTime)
    {
        if (!gl3wIsSupported(4, 3)) return;
        if (!m_pause) {
            m_gameTime += currentTime - m_lastTickTime;
            if(sk) sk->update(0.03f);
        }
        
        // logic stuff
        Director::instance()->update(float(currentTime - m_lastTickTime));
        auto python_module = pybind11::module::import("python");
        python_module.attr("logic")(currentTime);
        logic();
        m_lastTickTime = currentTime;

        // light info
        vmath::uvec4 light_num = m_lightMgr->getLightNum();
        UniformBlock::instance()->setUniformBlockMember("light_num_info", &light_num);
        UniformBlock::instance()->setUniformBlockMember("light_info", m_lightMgr->getLightInfo().data());

        // shadowmap
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // draw shadowmap
        m_lightCamera->drawToFrameBuffer(shadowMap);
        auto camera_matrix = m_lightCamera->getCameraTransform();
        auto proj_matrix = m_lightCamera->getProjectionMatrix();
        UniformBlock::instance()->setUniformBlockMember("light_view_matrix", glm::value_ptr(camera_matrix));
        UniformBlock::instance()->setUniformBlockMember("light_proj_matrix", glm::value_ptr(proj_matrix));

        auto texture = shadowMap->getTexture();
        texture->bindTexture(TEXTURE_SHADOW_MAP);

        // draw regular buffers
        m_camera->drawToFrameBuffer(frameBuffer);
        
        // texture to screen
        glViewport(0, 0, info.windowWidth, info.windowHeight);
        glDisable(GL_DEPTH_TEST);
        
        glClearBufferfv(GL_COLOR, 0, sb7::color::White);
        glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);
        m_screen->getProgram()->setMatrix4fvUniform("m_matrix", glm::value_ptr(glm::mat4(1.0f)));
        m_screen->draw();

        // depth view port
        m_depth->getProgram()->setMatrix4fvUniform("m_matrix", glm::value_ptr(glm::mat4(1.0f)));
        m_depth->draw();
        
        // ui
        Director::instance()->draw();
    }

private:
    float aspect;
    LightMgr * m_lightMgr;
    Texture2D * texture;
    bool left_mouse_down = false;
    bool middle_mouse_down = false;
    int _wndPos[2];
    int _wndSize[2];
    int mouse_pos_x = -1;
    int mouse_pos_y = -1;
    Camera * m_camera;
    Camera * m_lightCamera;
    bool m_controlLightCamera = false;

    GLenum m_polygonMode;

    bool m_pause;
    double m_gameTime;
    double m_lastTickTime;
    Skeleton * sk = nullptr;
    FrameBuffer * frameBuffer;
    FrameBuffer * shadowMap;
    HPolygon * m_screen;
    HPolygon * m_depth;
};
// Our one and only instance of DECLARE_MAIN
DECLARE_MAIN(my_application);
