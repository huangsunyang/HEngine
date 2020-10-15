#pragma once

#include <map>
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "GLObject/Drawable.hpp"

using std::map;
using std::string;
using std::vector;

class Skeleton;

struct Weight {
    Weight(int i, float w): index(i), weight(w) {}
    int index;
    float weight;
};

struct Point {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texcoord;
    vector<Weight> weights;
};

class Skin: public Drawable {
public:
    Skin(Skeleton * m): m_skeleton(m), m_data(), m_points(), m_indices(), Drawable() {}
    virtual ~Skin() {}
    
    virtual void draw();
    void update();
    void loadFromFile(string);
    void loadMorph(vector<string>);
    void loadMorph(string, float blend = 1.0f);

    void setMorphBlend(string name, float f) { m_morphBlend[name] = glm::clamp(f, 0.0f, 1.0f); }
    float getMorphBlend(string name) { return m_morphBlend[name]; }

    // you have to implement these methods to draw something!!
    virtual GLfloat * getPoints() override { return m_data.data(); }
    virtual GLuint * getIndices() override { return m_indices.data(); }
    virtual size_t getPointNum() override { return m_points.size(); }
    virtual size_t getIndiceNum() override { return m_indices.size(); }
    virtual VertexInfo * getVertexInfo() override { return m_vertexInfo; }

protected:
    void pushMatrix();
    glm::vec4 getMorphPosition(int);
    glm::vec4 getMorphNormal(int);

protected:
    Skeleton * m_skeleton;
    vector<float> m_data;
    vector<Point> m_points;
    vector<unsigned int> m_indices;
    vector<glm::mat4> m_bindingMatrixs;
    VertexInfo * m_vertexInfo;

    map<string, map<int, Point>> m_morphs;
    map<string, float> m_morphBlend;

    int m_curRow;
    string m_filePath;
};