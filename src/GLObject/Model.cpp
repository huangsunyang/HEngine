#include "GLObject/Model.hpp"
#include "utils/FileUtils.hpp"
#include "ObjLoader.hpp"
#include "shape2d.hpp"
#include "LogManager.hpp"

#define VBO_SIZE 1024 * 1024 * 4


void Model::setShader(vector<string> shaders) {
    for (const string& shader: shaders) {
        INFO("%s\n", shader.c_str());
        m_program->bindShader(shader);
    }
    m_program->linkProgram();
}

void Model::initBuffers() {
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

    for (size_t i = 0; i < pointNum * stride / sizeof(GLfloat); i++) {
        // INFO("%d %f\n", i, points[i]);
    }
    INFO("\n");

    // init attributes
    int attrNum = m_vertexInfo->attrInfos.size();
    int relativeOffset = 0;
    INFO("attribute size %d\n", attrNum);
    for (int i = 0; i < attrNum; i++) {
        VertexAttrInfo attrInfo = m_vertexInfo->attrInfos[i];
        INFO("attribute %d num: %d\n", i, attrInfo.num);
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


void Model::loadMesh(string name) {
    string format = Utils::file_format(name);
    if (format == ".obj") {
        m_mesh = new ObjLoader(name);
    } else {
        // other format in future
    }
    setTexture({m_mesh->getTextures()});
    initBuffers();
}


void Model::loadVertex(vector<GLfloat> points) {
    m_mesh = HPolygon::from_vertex(points);
    initBuffers();
}


void Model::loadVertexIndice(vector<GLfloat> points, vector<GLuint> index) {
    m_mesh = HPolygon::fromVertexIndice(points, index);
    initBuffers();
}


void Model::loadVertexCoord(vector<GLfloat> points, vector<GLfloat> coords) {
    m_mesh = HPolygon::fromVertexCoord(points, coords);
    initBuffers();
}

/* set texture at binding point
 */
void Model::setTexture(int bindingIndex, const char * textureFile) {
    Texture2D * texture = new Texture2D;
    texture->bindTexture(0);
    texture->loadImage(textureFile);
    
    // replace old texture with new one
    if (m_textures.find(bindingIndex) != m_textures.end()) {
        Texture2D * oldTexture = m_textures[bindingIndex];
        delete oldTexture;
    }
    m_textures[bindingIndex] = texture;
}


void Model::setTexture(vector<string> textureFiles) {
    for (size_t i = 0; i < textureFiles.size(); i++) {
        setTexture(i, textureFiles[i].c_str());
    }
}


void Model::draw() {
    m_vao->bindVertexArray();
    m_program->useProgram();

    /* by default, we use sampler name s0, s1, s2, s3, s4... */
    char sampler_name[5];
    for (auto pair: m_textures) {
        pair.second->bindTexture(pair.first);
        sprintf_s(sampler_name, 5, "s%d", pair.first);
        m_program->setIntUniform(sampler_name, pair.first);
    }

    size_t point_num = getPointNum();
    size_t indice_num = getIndiceNum();

    glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode);
    if (!m_vertexInfo->useIndice) {
        glDrawArrays(m_drawMode, 0, point_num);
    } else {
        glDrawElements(m_drawMode, indice_num, GL_UNSIGNED_INT, 0);
    }
}
