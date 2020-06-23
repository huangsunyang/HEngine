#include <math.h>
#include <string>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <streambuf>
#include "sb7.h"
#include "sb7color.h"
#include "shape2d.hpp"
#include "LogManager.hpp"
#include <pybind11/embed.h>
#include "GLObject/Program.hpp"
#include "GLObject/Texture.hpp"
#include "GLObject/Model.hpp"
#include "GLObject/Light.hpp"
#include "Camera.hpp"


// Derive my_application from sb7::application
class my_application : public sb7::application
{
public:
    void startup()
    {   
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

        // init camera
        m_camera = new Camera;
        
        /*
        shader_mgr.bind_shader(GL_TESS_CONTROL_SHADER, "tess_control_shader");
        shader_mgr.bind_shader(GL_TESS_EVALUATION_SHADER, "tess_evaluation_shader");
        shader_mgr.bind_shader(GL_GEOMETRY_SHADER, "geometry_shader");
        */
        init_shape();
        onResize(info.windowWidth, info.windowHeight);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    void init_shape() {
        models = vector<Model *>();

        Model * triangle = new Model();
        triangle->setShader({"shader/texture.vs", "shader/texture.fs"});
        triangle->setTexture({"Package/res/awesomeface.png", "Package/res/wall.jpg", "Package/res/timg.jpg"});
        triangle->loadVertexCoord({-1, -1, 0, 1, -1, 0, -1, 1, 0}, {0, 0, 1, 0, 0, 1});
        // drawCommands.push_back(triangle);

        Model * obj = new Model();
        obj->setShader({"shader/common_light.vs", "shader/common_light.fs"});
        obj->loadMesh("Package/res/capsule.obj");
        models.push_back(obj);

        Model * axis = new Model();
        axis->setShader({"shader/axis.vs", "shader/common.fs"});
        axis->setDrawMode(GL_LINES);
        axis->loadVertex({
            10., .0, .0, 
            .0, .0, .0, 
            .0, .0, 10.0,
            .0, .0, .0,
            .0, 10.0, 0.,
            .0, .0, .0
        });
        models.push_back(axis);

        light = new Light();
        light->getTransform()->setPosition({0, 5, 0});
        models.push_back(light);

        Model * polygon = new Model();
        polygon->setShader({"shader/ui.vs", "shader/common.fs"});
        polygon->setDrawMode(GL_TRIANGLE_FAN);
        polygon->loadVertex({
            .25, .0, .0, 
            .75, .0, .0, 
            1.0, .5, .0,
            0.75, 1.0, .0,
            0.25, 1.0, .0,
            .0, .5, .0
        });
        // models.push_back(polygon);
    }

    void onMouseButton(int button, int action) {
        if (button == GLFW_MOUSE_BUTTON_1) {
            if (action == GLFW_PRESS) {
                left_mouse_down = true;
            } else if (action == GLFW_RELEASE) {
                left_mouse_down = false;
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
        if (left_mouse_down) {
            if (!(mouse_pos_x < 0 && mouse_pos_y < 0)) {
                float diff_x = float(x - mouse_pos_x) / 1000.0; 
                float diff_y = float(y - mouse_pos_y) / -1000.0;
                m_camera->rotateCameraBy(diff_x, diff_y);
            }
        }
        if (middle_mouse_down) {
            if (!(mouse_pos_x < 0 && mouse_pos_y < 0)) {
                float diff_x = float(x - mouse_pos_x) / -100.0;
                float diff_y = float(y - mouse_pos_y) / 100.0;
                m_camera->moveCameraBy(diff_x, diff_y, 0);
            }
        }
        mouse_pos_x = x, mouse_pos_y = y;
    }

    void onKey(int key, int action) {
        if (key == GLFW_KEY_F && action == GLFW_PRESS) {
            onFullScreen();
        } else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
            if (m_polygonMode == GL_LINE) {
                m_polygonMode = GL_FILL;
            } else {
                m_polygonMode = GL_LINE;
            }
            for (auto command: models) {
                command->setPolygonMode(m_polygonMode);
            }
        } else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
            m_pause = !m_pause;
        }
    }

    void onMouseWheel(int pos) {
        m_camera->moveCameraBy(0, 0, pos * 0.1);
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

    void onFullScreen() {
        if (!isFullscreen()) {
            // backup windwo position and window size
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

    void shutdown()
    {
    }

    virtual void run(application * app) {
        pybind11::scoped_interpreter guard{};
        pybind11::module::import("sys").attr("path").cast<pybind11::list>().append("./script");
        this->application::run(this);
    }

    // Our rendering function
    void render(double currentTime)
    {
        if (!gl3wIsSupported(4, 3)) return;
        if (!m_pause) {
            m_gameTime += currentTime - m_lastTickTime;
        }
        m_lastTickTime = currentTime;
        auto python_module = pybind11::module::import("python");
        python_module.attr("logic")(currentTime);
        glClearBufferfv(GL_COLOR, 0, sb7::color::Black);
        glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);
        vmath::mat4 camera_matrix = m_camera->getCameraTransform();
        vmath::mat4 proj_matrix = m_camera->getProjectionMatrix();

        float f = (float)m_gameTime * 0.3f;
        vmath::vec3 translate = vmath::vec3(
            sinf(2.1f * f) * 5,
            cosf(1.7f * f) * 5,
            sinf(1.3f * f) * 5
        );
        vmath::vec3 rotate = translate * 50;
        light->getTransform()->setPosition(translate);
        for (auto model: models) {
            vmath::mat4 m_matrix = model->getTransformMatrix();
            vmath::mat4 m_matrix_t = m_matrix.transpose();
            vmath::mat4 mvp_matrix = proj_matrix * camera_matrix * m_matrix;
            model->getProgram()->setMatrix4fvUniform("m_matrix", m_matrix);
            model->getProgram()->setMatrix4fvUniform("v_matrix", camera_matrix);
            model->getProgram()->setMatrix4fvUniform("p_matrix", proj_matrix);
            model->getProgram()->setMatrix4fvUniform("mvp_matrix", mvp_matrix);
            model->getProgram()->setMatrix4fvUniform("m_matrix_it", m_matrix);
            model->getProgram()->setMatrix4fvUniform("m_matrix_it", m_matrix);
            model->getProgram()->setVec3Uniform("light_dir", light->getTransform()->getPosition() - model->getTransform()->getPosition());
            model->draw();
        }
    }

private:
    GLuint vao;
    float aspect;
    vector<Model *> models;
    Light * light;
    Texture2D * texture;
    Texture2D * texture1;
    bool left_mouse_down = false;
    bool middle_mouse_down = false;
    int _wndPos[2];
    int _wndSize[2];
    int mouse_pos_x = -1;
    int mouse_pos_y = -1;
    Camera * m_camera;
    GLenum m_polygonMode;

    bool m_pause;
    float m_gameTime;
    float m_lastTickTime;
};
// Our one and only instance of DECLARE_MAIN
DECLARE_MAIN(my_application);
