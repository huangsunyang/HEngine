#include <math.h>
#include "sb7.h"
#include "sb7color.h"
#include "shape_base.hpp"
#include "logger.hpp"
#include "ObjLoader.hpp"
#include "GLObject/VertexArray.hpp"
#include "GLObject/Program.hpp"


#define VBO_NUM 3
#define VBO_SIZE 1024 * 1024 * 4


class GLBufferMgr {
public:
    GLBufferMgr() {
        vao = new VertexArray;
        vao->bindVertexArray();
        for (int i = 0; i < VBO_NUM; i++) {
            vbo[i] = new VertexBuffer;
        }
        vbo[0]->alloc(VBO_SIZE, GL_DYNAMIC_STORAGE_BIT);
        vao->setVertexBuffer(0, vbo[0], sizeof(GLfloat) * 3);
        vao->setVertexAttrib(0, 3, GL_FLOAT, 0);
        vao->bindVertexAttrib(0, 0);
        vao->enableAttrib(0);

        vbo[1]->alloc(VBO_SIZE, GL_DYNAMIC_STORAGE_BIT);
        vao->setVertexBuffer(1, vbo[1], sizeof(GLfloat) * 2);
        vao->setVertexAttrib(1, 2, GL_FLOAT, 0);
        vao->bindVertexAttrib(1, 1);
        vao->enableAttrib(1);

        vbo[2]->alloc(VBO_SIZE, GL_DYNAMIC_STORAGE_BIT);
        vao->setElementBuffer(vbo[2]);
    }

    static GLBufferMgr * get_instance() {
        static GLBufferMgr * instance = new GLBufferMgr;
        return instance;
    }

    void draw(HShapeBase* shape) {
        if (!shape->is_visible()) return;
        int point_num = shape->get_point_num();
        int indice_num = point_num;
        vbo[0]->subData(0, sizeof(GLfloat) * 3 * point_num, shape->get_points());
        vbo[1]->subData(0, sizeof(GLfloat) * 2 * point_num, shape->getTexcoord());
        vbo[2]->subData(0, sizeof(GLuint) * indice_num, shape->get_indices());
        glDrawElements(GL_TRIANGLE_STRIP, indice_num, GL_UNSIGNED_INT, 0);
    }

    void draw(ObjLoader* loader) {
        int index_num = loader->get_index_num();
        vbo[0]->subData(0, sizeof(GLfloat) * 3 * 2 * index_num, loader->get_vertex_and_normal());
        glDrawArrays(GL_TRIANGLES, 0, index_num);

        // int point_num = loader->get_vertex_num();
        // int indice_num = loader->get_index_num();
        // glNamedBufferSubData(vbo[0], 0, sizeof(GLfloat) * 3 * point_num, loader->get_points());
        // glNamedBufferSubData(vbo[1], 0, sizeof(GLuint) * indice_num, loader->get_indices());
        // glDrawElements(GL_TRIANGLES, indice_num, GL_UNSIGNED_INT, 0);
    }

    VertexBuffer * vbo[VBO_NUM];
    VertexArray * vao;
};

