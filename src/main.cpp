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
#include "GLObject/DrawCommand.hpp"


// Derive my_application from sb7::application
class my_application : public sb7::application
{
public:
    void startup()
    {   
        left_mouse_down = false;
        middle_mouse_down = false;
        m_isFullScreen = false;
        mouse_pos_x = -1;
        mouse_pos_y = -1;
        eye_pos = {0, 0, 2};
        lookat_pos = {0, 0, 0};
        diff_x_scale = 0;
        diff_z_scale = 0;
        x_rotate = 0;
        y_rotate = 0;

        // redirect unbuffered STDOUT to the console
        LOG::LogManager::init();
        LOG::LogManager::showAllLogger();
        
        /*
        shader_mgr.bind_shader(GL_TESS_CONTROL_SHADER, "tess_control_shader");
        shader_mgr.bind_shader(GL_TESS_EVALUATION_SHADER, "tess_evaluation_shader");
        shader_mgr.bind_shader(GL_GEOMETRY_SHADER, "geometry_shader");
        */
        init_shape();
        onResize(info.windowWidth, info.windowHeight);
        camera_matrix = vmath::lookat(eye_pos, lookat_pos, vmath::vec3(0, 1, 0));
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    void init_shape() {
        drawCommands = vector<DrawCommand *>();

        DrawCommand * triangle = new DrawCommand();
        triangle->setShader({"shader/texture.vs", "shader/texture.fs"});
        triangle->setTexture({"Package/res/awesomeface.png", "Package/res/wall.jpg"});
        triangle->loadVertexCoord({-1, -1, 0, 1, -1, 0, -1, 1, 0}, {0, 0, 1, 0, 0, 1});
        drawCommands.push_back(triangle);

        DrawCommand * obj = new DrawCommand();
        obj->setShader({"shader/common_light.vs", "shader/common.fs"});
        obj->loadMesh("./suzanne.obj");
        drawCommands.push_back(obj);

        DrawCommand * axis = new DrawCommand();
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
        drawCommands.push_back(axis);
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
                x_rotate += float(x - mouse_pos_x); 
                y_rotate += float(y - mouse_pos_y);
            }
        }
        if (middle_mouse_down) {
            if (!(mouse_pos_x < 0 && mouse_pos_y < 0)) {
                diff_x_scale += float(x - mouse_pos_x) / -100.0;
                diff_z_scale += float(y - mouse_pos_y) / 100.0;
            }
        }
        mouse_pos_x = x, mouse_pos_y = y;
    }

    void onKey(int key, int action) {
        if (key == GLFW_KEY_F && action == GLFW_PRESS) {
            onFullScreen();
        }
    }

    void onMouseWheel(int pos) {
        vmath::vec3 lookat_dir = normalize(lookat_pos - eye_pos);
        eye_pos -= lookat_dir * pos * 0.1;
    }

    void tick_camera() {
        vmath::vec3 lookat_dir = normalize(lookat_pos - eye_pos);
        vmath::vec3 upN = vmath::vec3(0, 1, 0);
        vmath::vec3 x = cross(lookat_dir, upN);
        vmath::vec3 y = cross(x, lookat_dir);

        vmath::vec3 diff = x * diff_x_scale + y * diff_z_scale;
        diff_x_scale = diff_z_scale = 0;
        lookat_pos += diff;
        eye_pos += diff;

        lookat_dir = lookat_dir.rotate(vmath::vec3(0, 1, 0), -x_rotate / 300.0);
        lookat_dir = lookat_dir.rotate(x, -y_rotate / 300.0);
        lookat_pos = eye_pos + lookat_dir;
        // INFO("%f %f %f %f %f %f %f %f\n", x_rotate, y_rotate, lookat_pos[0], lookat_pos[1], lookat_pos[2], eye_pos[0], eye_pos[1], eye_pos[2]);
        x_rotate = y_rotate = 0;
        camera_matrix = vmath::lookat(eye_pos, lookat_pos, vmath::vec3(0, 1, 0));
    }

    void onResize(int w, int h) {
        sb7::application::onResize(w, h);
        glViewport(0, 0, w, h);
        aspect = (float)info.windowWidth / (float)info.windowHeight;
        proj_matrix = vmath::perspective(
            30.0f, aspect, 0.1f, 100.0f
        );
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

            // swithc to full screen
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
        auto python_module = pybind11::module::import("python");
        python_module.attr("logic")(currentTime);
        tick_camera();
        glClearBufferfv(GL_COLOR, 0, sb7::color::LightPink);
        glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);
        for (int i = 0; i < 1; i++) {
            float f = (float)currentTime * 0.3f + (float)i;
            vmath::mat4 m_matrix = vmath::rotate(
                sinf(2.1f * f) * 0,
                cosf(1.7f * f) * 0,
                sinf(1.3f * f) * cosf(1.5f * f) * 200.0f
            );
            vmath::mat4 mvp_matrix = proj_matrix * camera_matrix * m_matrix;
            vmath::mat4 m_matrix_t = m_matrix.transpose();
            for (auto triangle: drawCommands) {
                triangle->getProgram()->setMatrix4fvUniform("m_matrix", m_matrix);
                triangle->getProgram()->setMatrix4fvUniform("v_matrix", camera_matrix);
                triangle->getProgram()->setMatrix4fvUniform("p_matrix", proj_matrix);
                triangle->getProgram()->setMatrix4fvUniform("mvp_matrix", mvp_matrix);
                triangle->getProgram()->setMatrix4fvUniform("m_matrix_it", m_matrix);
                triangle->getProgram()->setIntUniform("s", 0);
                triangle->getProgram()->setIntUniform("s1", 1);
                triangle->draw();
            }
        }
    }

private:
    GLuint vao;
    float aspect;
    vmath::mat4 proj_matrix;
    vmath::mat4 camera_matrix;
    vector<DrawCommand *> drawCommands;
    Texture2D * texture;
    Texture2D * texture1;
    bool left_mouse_down = false;
    bool middle_mouse_down = false;
    bool m_isFullScreen = false;
    int _wndPos[2];
    int _wndSize[2];
    int mouse_pos_x = -1;
    int mouse_pos_y = -1;
    vmath::vec3 eye_pos;
    vmath::vec3 lookat_pos;
    float diff_x_scale = 0;
    float diff_z_scale = 0;
    float x_rotate = 0;
    float y_rotate = 0;
};
// Our one and only instance of DECLARE_MAIN
DECLARE_MAIN(my_application);
