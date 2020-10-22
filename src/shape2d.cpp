#include "shape2d.hpp"

HPolygon::~HPolygon() {
    m_verteces.clear();
    m_indices.clear();
    delete m_vertexInfo;
};


HPolygon* HPolygon::loadVertexCoord(const vector<float> &positions, const vector<float> &coords, const vector<GLuint> &index) {
    size_t N = positions.size() / 3;
    int vertex_size = 3;
    if (!coords.empty()) vertex_size += 2;

    m_verteces.clear();
    m_indices.clear();
    if (index.empty()) {
        for (size_t i = 0; i < N; i++) {
            m_indices.push_back(i);
        }
    } else {
        m_indices.insert(m_indices.end(), index.begin(), index.end());
    }

    m_vertexInfo = new VertexInfo;
    m_vertexInfo->useIndice = true;
    auto attrInfos = vector<VertexAttrInfo> {
        {3, GL_FLOAT, 12},
    };
    if (!coords.empty()) attrInfos.push_back({2, GL_FLOAT, 8});
    m_vertexInfo->attrInfos = attrInfos;

    for (size_t i = 0; i < N; i++) {
        m_verteces.push_back(positions[3 * i + 0]);
        m_verteces.push_back(positions[3 * i + 1]);
        m_verteces.push_back(positions[3 * i + 2]);
        if (!coords.empty()) {
            m_verteces.push_back(coords[2 * i + 0]);
            m_verteces.push_back(coords[2 * i + 1]);
        }
    }

    this->initBuffers();
    return this;
}
