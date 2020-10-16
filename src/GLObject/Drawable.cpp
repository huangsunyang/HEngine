#include "GLObject/Drawable.hpp"
#include "utils/FileUtils.hpp"
#include "ObjLoader.hpp"
#include "shape2d.hpp"
#include "LogManager.hpp"
#include "base/Director.hpp"

#define VBO_SIZE 1024 * 1024 * 4


void Drawable::setShader(vector<string> shaders) {
    if (m_program) {
        delete m_program;
        m_program = new Program;
    }
    m_program->bindShader(shaders);
    m_program->linkProgram();
}

size_t Drawable::getPointSize() {
    // calculate all attr size together, as stride
    size_t stride = 0;
    for (auto attr: getVertexInfo()->attrInfos) {
        stride += attr.size;
    }
    return stride;
}

void Drawable::initBuffers() {
    auto vertexInfo = getVertexInfo();
    size_t pointNum = getPointNum();
    GLfloat * points = getPoints();
    auto stride = getPointSize();

    // init vao and vbos
    m_vao = new VertexArray;
    m_vao->bindVertexArray();

    // init vbo and sub data
    m_vbo = new VertexBuffer;
    m_vao->setVertexBuffer(0, m_vbo, stride);
    m_vbo->alloc(pointNum * stride, GL_DYNAMIC_STORAGE_BIT);
    m_vbo->subData(points);

    // init attributes
    int attrNum = vertexInfo->attrInfos.size();
    int relativeOffset = 0;
    INFO("attribute size %d\n", attrNum);
    for (int i = 0; i < attrNum; i++) {
        VertexAttrInfo attrInfo = vertexInfo->attrInfos[i];
        INFO("attribute %d num: %d\n", i, attrInfo.num);
        m_vao->setVertexAttrib(i, attrInfo.num, attrInfo.type, relativeOffset);
        m_vao->bindVertexAttrib(i, 0);
        m_vao->enableAttrib(i);
        relativeOffset += attrInfo.size;
    }

    INFO("use index %d\n", vertexInfo->useIndice);
    // if use indices, init ebo
    if (vertexInfo->useIndice) {
        m_ebo = new VertexBuffer;
        m_ebo->alloc(sizeof(GLuint) * getIndiceNum(), GL_DYNAMIC_STORAGE_BIT);
        m_ebo->subData(sizeof(GLuint) * getIndiceNum(), getIndices());
        m_vao->setElementBuffer(m_ebo);
    }
}

/* set texture at binding point */
void Drawable::setTexture(int bindingIndex, const char * textureFile) {
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

void Drawable::setTexture(vector<Texture2D *> textures) {
    for (size_t i = 0; i < textures.size(); i++) {
        // replace old texture with new one
        // if (m_textures.find(i) != m_textures.end()) {
        //     Texture2D * oldTexture = m_textures[i];
        //     if (oldTexture) {
        //         delete oldTexture;
        //     }
        // }
        m_textures.insert(std::make_pair(i, textures[i]));
    }
}

void Drawable::setTexture(vector<string> textureFiles) {
    for (size_t i = 0; i < textureFiles.size(); i++) {
        setTexture(i, textureFiles[i].c_str());
    }
}

void Drawable::draw() {
    m_vao->bindVertexArray();
    
    if (!Director::instance()->isOverrideShader()) {
        m_program->useProgram();
    }

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
    if (!getVertexInfo()->useIndice) {
        glDrawArraysInstanced(m_drawMode, 0, point_num, m_instanceCount);
    } else {
        glDrawElementsInstanced(m_drawMode, indice_num, GL_UNSIGNED_INT, 0, m_instanceCount);
    }
}