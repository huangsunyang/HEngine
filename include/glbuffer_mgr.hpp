#include <math.h>
#include "sb7.h"
#include "sb7color.h"
#include "shader_mgr.h"
#include "shape_base.hpp"
#include "logger.hpp"

#define VBO_NUM 2
#define VBO_SIZE 1024 * 1024

class GLBufferMgr {
public:
    GLBufferMgr() {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glCreateBuffers(VBO_NUM, &vbo[0]);
        glNamedBufferStorage(vbo[0], VBO_SIZE, NULL, GL_DYNAMIC_STORAGE_BIT);
        glNamedBufferStorage(vbo[1], VBO_SIZE, NULL, GL_DYNAMIC_STORAGE_BIT);
        glVertexArrayVertexBuffer(vao, 0, vbo[0], 0, sizeof(GLfloat) * 3);
        glVertexArrayElementBuffer(vao, vbo[1]);
        glEnableVertexArrayAttrib(vao, 0);
        glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(vao, 0, 0);
    }

    static GLBufferMgr * get_instance() {
        static GLBufferMgr * instance = new GLBufferMgr;
        return instance;
    }

    void draw(HShapeBase* shape) {
        if (!shape->is_visible()) return;
        int point_num = shape->get_point_num();
        int triangle_num = point_num - 2;
        int indice_num = point_num;
        glNamedBufferSubData(vbo[0], 0, sizeof(GLfloat) * 3 * point_num, shape->get_points());
        glNamedBufferSubData(vbo[1], 0, sizeof(GLuint) * indice_num, shape->get_indices());
        glDrawElements(GL_TRIANGLE_STRIP, indice_num, GL_UNSIGNED_INT, 0);
    }

    GLuint vao;
    GLuint vbo[VBO_NUM];
};

