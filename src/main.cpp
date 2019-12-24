#include <math.h>
#include <string>
#include <fstream>
#include <streambuf>
#include "sb7.h"
#include "sb7color.h"
#include "shader_mgr.h"
#include "shape2d.hpp"
#include "glbuffer_mgr.hpp"


// Derive my_application from sb7::application
class my_application : public sb7::application
{
public:
    void startup()
    {
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
        aspect = (float)info.windowWidth / (float)info.windowHeight;
        proj_matrix = vmath::perspective(
            50.0f, aspect, 0.1f, 1000.0f
        );
    }

    void init_shape() {
        triangle = HTriangle::from_vertex(
            0.25f, 0.25f, 0.25f,
            0.25f, -0.25f, 0.25f,
            0.25f, 0.25f, 0.25f
        );
    }

    void bind_buffer() {
        GLBufferMgr::get_instance();
    }

    void bind_texture() {

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
        if (!gl3wIsSupported(4, 3)) return;

        glClearBufferfv(GL_COLOR, 0, sb7::color::White);
        glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);
        shader_mgr->use_program();
        glUniformMatrix4fv(1, 1, GL_FALSE, proj_matrix);
        for (int i = 0; i < 24; i++) {
            float f = (float)currentTime * 0.3f + (float)i;
            vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -4.0f)
            * vmath::translate(
                sinf(2.1f * f) * 0.5f,
                cosf(1.7f * f) * 0.5f,
                sinf(1.3f * f) * cosf(1.5f * f) * 2.0f
            ) * vmath::rotate(
                (float)currentTime * 45.0f, 0.0f, 1.0f, 0.0f
            ) * vmath::rotate(
                (float)currentTime * 81.0f, 1.0f, 0.0f, 0.0f
            );
            glUniformMatrix4fv(0, 1, GL_FALSE, mv_matrix);
            triangle->visible = sinf(f) > 0.5;
            GLBufferMgr::get_instance()->draw(triangle);
        }
    }

private:
    ShaderMgr * shader_mgr;
    GLuint vao;
    float aspect;
    vmath::mat4 proj_matrix;
    HTriangle * triangle;
};
// Our one and only instance of DECLARE_MAIN
DECLARE_MAIN(my_application);
