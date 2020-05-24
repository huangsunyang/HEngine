#include "GLObject/DrawCommand.hpp"
#include "utils/FileUtils.hpp"
#include "ObjLoader.hpp"
#include "shape2d.hpp"

#define VBO_SIZE 1024 * 1024 * 4


void DrawCommand::setShader(vector<string> shaders) {
    for (const string& shader: shaders) {
        m_program->bindShader(shader);
    }
    m_program->linkProgram();
}

void DrawCommand::initBuffers() {
    m_vertexInfo = m_mesh->getVertexInfo();
    size_t pointNum = getPointNum();
    GLfloat * points = getPoints();

    // init vao and vbos
    m_vao = new VertexArray;
    m_vao->bindVertexArray();

    // calculate all attr size together, as stride
    size_t stride = 0;
    for (auto attr: m_vertexInfo->attrInfos) {
        stride += attr.size;
    }

    // init vbo and sub data
    m_vbo = new VertexBuffer;
    m_vao->setVertexBuffer(0, m_vbo, stride);
    m_vbo->alloc(pointNum * stride, GL_DYNAMIC_STORAGE_BIT);
    m_vbo->subData(points);

    // init attributes
    int attrNum = m_vertexInfo->attrInfos.size();
    int relativeOffset = 0;
    INFO("attribute size %d\n", attrNum);
    for (int i = 0; i < attrNum; i++) {
        VertexAttrInfo attrInfo = m_vertexInfo->attrInfos[i];
        m_vao->setVertexAttrib(i, attrInfo.num, attrInfo.type, relativeOffset);
        m_vao->bindVertexAttrib(i, 0);
        m_vao->enableAttrib(i);
        relativeOffset += attrInfo.size;
    }

    INFO("use index %d\n", m_vertexInfo->useIndice);
    // if use indices, init ebo
    if (m_vertexInfo->useIndice) {
        m_ebo = new VertexBuffer;
        m_ebo->alloc(sizeof(GLuint) * getIndiceNum(), GL_DYNAMIC_STORAGE_BIT);
        m_ebo->subData(sizeof(GLuint) * getIndiceNum(), getIndices());
        m_vao->setElementBuffer(m_ebo);
    }
}


void DrawCommand::loadMesh(string name) {
    string format = Utils::file_format(name);
    if (format == ".obj") {
        m_mesh = new ObjLoader(name);
    } else {
        // other format in future
    }
    initBuffers();
}


void DrawCommand::loadGeometry(vector<GLfloat> points) {
    m_mesh = HPolygon::from_vertex(points);
    initBuffers();
}


void DrawCommand::draw() {
    m_vao->bindVertexArray();
    m_program->useProgram();

    size_t point_num = getPointNum();
    size_t indice_num = getIndiceNum();

    if (!m_vertexInfo->useIndice) {
        glDrawArrays(m_drawMode, 0, point_num);
    } else {
        glDrawElements(m_drawMode, indice_num, GL_UNSIGNED_INT, 0);
    }
}
