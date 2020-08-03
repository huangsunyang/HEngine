#include "GLObject/Model.hpp"
#include "utils/FileUtils.hpp"
#include "ObjLoader.hpp"
#include "shape2d.hpp"
#include "LogManager.hpp"

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

void Model::loadVertex(const vector<GLfloat> &points) {
    m_mesh = HPolygon::from_vertex(points);
    initBuffers();
}

void Model::loadVertexIndice(const vector<GLfloat> &points, const vector<GLuint> &index) {
    m_mesh = HPolygon::fromVertexIndice(points, index);
    initBuffers();
}

void Model::loadVertexCoord(const vector<GLfloat> &points, const vector<GLfloat> &coords) {
    m_mesh = HPolygon::fromVertexCoord(points, coords);
    initBuffers();
}
