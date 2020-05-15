#include <math.h>
#include <string>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <streambuf>
#include "sb7.h"
#include "sb7color.h"
#include "shader_mgr.h"
#include "shape2d.hpp"
#include "glbuffer_mgr.hpp"
#include "LogManager.hpp"

void test_binding_python();

// Derive my_application from sb7::application
class my_application : public sb7::application
{
public:
    void startup()
    {
        // redirect unbuffered STDOUT to the console
        LOG::LogManager::init();
        LOG::LogManager::showAllLogger();
        test_binding_python();

        shader_mgr = new ShaderMgr;
        shader_mgr->bind_shader(GL_VERTEX_SHADER, "./shader/vertex");
        shader_mgr->bind_shader(GL_FRAGMENT_SHADER, "./shader/fragment");
        shader_mgr->link_program();
        /*
        shader_mgr.bind_shader(GL_TESS_CONTROL_SHADER, "tess_control_shader");
        shader_mgr.bind_shader(GL_TESS_EVALUATION_SHADER, "tess_evaluation_shader");
        shader_mgr.bind_shader(GL_GEOMETRY_SHADER, "geometry_shader");
        */
        init_shape();
        bind_buffer();
        onResize(info.windowWidth, info.windowHeight);
        camera_matrix = vmath::lookat(eye_pos, lookat_pos, vmath::vec3(0, 1, 0));
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    void init_shape() {
        shape = new ObjLoader("suzanne.obj");
    }

    void bind_buffer() {
        GLBufferMgr::get_instance();
    }

    void bind_texture() {

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
        }
        if (middle_mouse_down) {
            if (!(mouse_pos_x < 0 && mouse_pos_y < 0)) {
                diff_x_scale += float(x - mouse_pos_x) / -100.0;
                diff_z_scale += float(y - mouse_pos_y) / 100.0;
            }
            mouse_pos_x = x, mouse_pos_y = y;
        }
    }

    void onMouseWheel(int pos) {
        vmath::vec3 lookat_dir = normalize(lookat_pos - eye_pos);
        eye_pos -= lookat_dir * pos * 0.1;
    }

    void tick_camera() {
        vmath::vec3 lookat_dir = lookat_pos - eye_pos;
        vmath::vec3 f = normalize(lookat_pos - eye_pos);
        vmath::vec3 upN = vmath::vec3(0, 1, 0);
        vmath::vec3 x = cross(f, upN);
        vmath::vec3 y = cross(x, f);

        vmath::vec3 diff = x * diff_x_scale + y * diff_z_scale;
        diff_x_scale = diff_z_scale = 0;
        lookat_pos += diff;
        eye_pos += diff;
        camera_matrix = vmath::lookat(eye_pos, lookat_pos, vmath::vec3(0, 1, 0));
    }

    void onResize(int w, int h) {
        sb7::application::onResize(w, h);
        aspect = (float)info.windowWidth / (float)info.windowHeight;
        proj_matrix = vmath::perspective(
            50.0f, aspect, 0.1f, 1000.0f
        );
    }

    void shutdown()
    {
        shader_mgr->delete_program();
    }

    // Our rendering function
    void render(double currentTime)
    {
        INFO("%lf\n", currentTime);
        if (!gl3wIsSupported(4, 3)) return;
        tick_camera();
        glClearBufferfv(GL_COLOR, 0, sb7::color::LightPink);
        glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);
        shader_mgr->use_program();
        // glUniformMatrix4fv(2, 1, GL_FALSE, proj_matrix);
        // glUniformMatrix4fv(1, 1, GL_FALSE, camera_matrix);
        for (int i = 0; i < 1; i++) {
            float f = (float)currentTime * 0.3f + (float)i;
            vmath::mat4 mv_matrix = vmath::rotate(
                sinf(2.1f * f) * 0,
                cosf(1.7f * f) * 0,
                sinf(1.3f * f) * cosf(1.5f * f) * 200.0f
            );
            vmath::mat4 mvp_matrix = proj_matrix * camera_matrix * mv_matrix;
            vmath::mat4 mvp_matrix_transpose = mv_matrix.transpose();
            glUniformMatrix4fv(shader_mgr->get_location("mvp_matrix"), 1, GL_FALSE, mvp_matrix);
            glUniformMatrix4fv(shader_mgr->get_location("mvp_matrix_transpose"), 1, GL_FALSE, mv_matrix);
            GLBufferMgr::get_instance()->draw(shape);
        }
    }

private:
    ShaderMgr * shader_mgr;
    GLuint vao;
    float aspect;
    vmath::mat4 proj_matrix;
    vmath::mat4 camera_matrix;
    ObjLoader * shape;
    bool left_mouse_down = false;
    bool middle_mouse_down = false;
    int mouse_pos_x = -1;
    int mouse_pos_y = -1;
    vmath::vec3 eye_pos = {1, 1, 0};
    vmath::vec3 lookat_pos = {0, 0, 0};
    float diff_x_scale = 0;
    float diff_z_scale = 0;
};
// Our one and only instance of DECLARE_MAIN
DECLARE_MAIN(my_application);
