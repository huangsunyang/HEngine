#include <math.h>
#include "sb7.h"
#include "sb7color.h"
#include "shader_mgr.h"
#include "shape_base.hpp"
#include "logger.hpp"
#include "ObjLoader.hpp"


#define VBO_NUM 2
#define VBO_SIZE 1024 * 1024 * 1024


class GLBufferMgr {
public:
    GLBufferMgr() {
        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glCreateBuffers(VBO_NUM, &vbo[0]);
        glNamedBufferStorage(vbo[0], VBO_SIZE, NULL, GL_DYNAMIC_STORAGE_BIT);
        
        glVertexArrayAttribBinding(vao, 0, 0); // attribute index/binding index
        glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);    // attribute index/size/relative offset
        glEnableVertexArrayAttrib(vao, 0);  //attribute index
        
        glVertexArrayAttribBinding(vao, 1, 0);
        glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3);
        glEnableVertexArrayAttrib(vao, 1);

        glNamedBufferStorage(vbo[1], VBO_SIZE, NULL, GL_DYNAMIC_STORAGE_BIT);
        glVertexArrayElementBuffer(vao, vbo[1]);
        
        glVertexArrayVertexBuffer(vao, 0, vbo[0], 0, sizeof(GLfloat) * 6); // binding index/offset/stride
    }

    static GLBufferMgr * get_instance() {
        static GLBufferMgr * instance = new GLBufferMgr;
        return instance;
    }

    void draw(HShapeBase* shape) {
        if (!shape->is_visible()) return;
        int point_num = shape->get_point_num();
        int indice_num = point_num;
        glNamedBufferSubData(vbo[0], 0, sizeof(GLfloat) * 3 * point_num, shape->get_points());
        glNamedBufferSubData(vbo[1], 0, sizeof(GLuint) * indice_num, shape->get_indices());
        glDrawElements(GL_TRIANGLE_STRIP, indice_num, GL_UNSIGNED_INT, 0);
    }

    void draw(ObjLoader* loader) {
        int surface_num = loader->get_surface_num();
        glNamedBufferSubData(vbo[0], 0, sizeof(GLfloat) * 18 * surface_num, loader->get_vertex_and_normal());
        glDrawArrays(GL_TRIANGLES, 0, surface_num * 3);

        // int point_num = loader->get_vertex_num();
        // int indice_num = loader->get_index_num();
        // glNamedBufferSubData(vbo[0], 0, sizeof(GLfloat) * 3 * point_num, loader->get_points());
        // glNamedBufferSubData(vbo[1], 0, sizeof(GLuint) * indice_num, loader->get_indices());
        // glDrawElements(GL_TRIANGLES, indice_num, GL_UNSIGNED_INT, 0);
    }

    GLuint vao;
    GLuint vbo[VBO_NUM];
};

