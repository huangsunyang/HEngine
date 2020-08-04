#pragma once
#include "ui/widget.hpp"
#include "ui/Common.hpp"
#include "sb7/vmath.h"
#include "GLObject/Texture.hpp"
#include <string>
#include <vector>

using vmath::vec2;
using vmath::vec3;
using std::string;
using std::vector;

struct Particle {
    vec2 pos;
    vec2 startPos;
    vec2 speed;
    vec2 size;
    vec3 color;

    float lifeTime;
    float livedTime;
};


class ParticleSystem: public Widget {
public:
    ParticleSystem(): Widget() {}
    ParticleSystem(vec2 pos, vec2 size): Widget(pos, size) {}
    ParticleSystem(vec2 pos, vec2 size, string name): Widget(pos, size, name) {}
    ParticleSystem(vec2 pos, vec2 size, string name, Widget * parent): Widget(pos, size, name, parent), m_particles(), m_quads() {
    }

    void initWithFile(string fileName);
    // void reset();

    void setTexture(Texture2D* tex) {m_texture = tex;};
    void setTexture(const string& name) {m_texture = new Texture2D; m_texture->loadImage(name.c_str());};

protected:
    void update(float dt);
    virtual void drawSelf() override;

protected:
    Texture2D * m_texture;
    int m_emittedNum = 0;
    int m_maxNum;                   // max num of particles of this emitter
    int m_numPerSec;                // num of particles emitted every second
    
    float m_timeCounter = 0.0;     // use to determine when to emit a particle

    float m_lifeTime;              // life time of each particle
    float m_lifeTimeBias;          // add a little random 

    float m_dir;                   // init dir of the particle
    float m_dirBias;

    float m_speed;
    float m_speedBias;     

    vec2 m_size;
    vec2 m_sizeBias;
    vec2 m_posBias;

    vector<Particle> m_particles;
    vector<Quad> m_quads;
};