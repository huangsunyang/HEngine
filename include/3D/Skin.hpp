#pragma once

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "GLObject/Drawable.hpp"

using std::string;
using std::vector;

class SkModel;

struct Weight {
    Weight(int i, float w): index(i), weight(w) {}
    int index;
    float weight;
};

class Skin: public Drawable {
public:
    Skin(SkModel * m): m_model(m), m_positions(), Drawable() {}
    virtual ~Skin() {}
    
    virtual void draw();
    void update();
    void loadFromFile(string);

    // you have to implement these methods to draw something!!
    virtual GLfloat * getPoints() override { return m_positions.data(); }
    virtual GLuint * getIndices() override { return m_triangles.data(); }
    virtual size_t getPointNum() override { return m_positions.size() / 6; }
    virtual size_t getIndiceNum() override { return m_triangles.size(); }
    virtual VertexInfo * getVertexInfo() override { return m_vertexInfo; }

    void setModelMatrix(glm::mat4 m) {m_matrix = m;}

protected:
    void pushMatrix();

protected:
    SkModel * m_model;
    vector<float> m_positions;
    vector<unsigned int> m_triangles;
    vector<vector<Weight>> m_weights;
    vector<glm::mat4> m_bindingMatrixs;
    VertexInfo * m_vertexInfo;
    glm::mat4 m_matrix;

    int m_curRow;
};