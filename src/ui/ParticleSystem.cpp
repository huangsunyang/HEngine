#include "ui/ParticleSystem.hpp"
#include "base/Director.hpp"
#include "GLObject/Geometry.hpp"
#include <math.h>
#include <random>

void ParticleSystem::initWithFile(string name) {
    m_maxNum = 2000;
    m_numPerSec = 20;

    m_dir = 135.0f;
    m_dirBias = 30.0f;

    m_speed = 0.3f;
    m_speedBias = 0.1f;

    m_lifeTime = 3.0f;
    m_lifeTimeBias = 1.0f;

    m_size = {0.1f, 0.1f};
    m_sizeBias = {0.02f, 0.02f};

    m_posBias = {0.1f, 0.1f};

    setTexture("package/res/particle-smoke.png");
    Director::instance()->addSchedule([this](float dt){update(dt);});
}

void ParticleSystem::update(float dt) {
    float timePerParticle = 1.0f / float(m_numPerSec);
    m_timeCounter += dt;

    int newParticleNum = 0;
    if (m_timeCounter > 0) { 
        newParticleNum = int(m_timeCounter / timePerParticle);
        m_timeCounter -= newParticleNum * timePerParticle;
        newParticleNum = min(newParticleNum, m_maxNum - m_emittedNum);
        m_emittedNum += newParticleNum;
    }

    auto curParticleNum = m_particles.size();
    for (auto &particle: m_particles) {
        particle.livedTime += dt;
        particle.pos += particle.speed * dt;
    }

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

    auto worldPos = getWorldPosition();
    for (int i = 0; i < newParticleNum; i++) {
        Particle a;
        a.startPos = a.pos = worldPos + vec2{m_posBias[0] * distribution(mt), m_posBias[1] * distribution(mt)};
        a.size = m_size + vec2{m_sizeBias[0] * distribution(mt), m_sizeBias[1] * distribution(mt)};
        auto dir = (m_dir + m_dirBias * distribution(mt)) * 3.1415926f / 180.0f;
        a.speed = vec2{cosf(dir), sinf(dir)} * m_speed + m_speedBias * distribution(mt);
        a.lifeTime = m_lifeTime + m_lifeTimeBias * distribution(mt);
        a.livedTime = 0.0f;
        m_particles.push_back(a);
    }

    // remove dead particles
    curParticleNum = m_particles.size();
    for (auto i = 0; i < curParticleNum; i++) {
        while (i < curParticleNum && m_particles[i].lifeTime < m_particles[i].livedTime) {
            swap(m_particles[i], m_particles[--curParticleNum]);
        }
    }
    m_particles.resize(curParticleNum);
}

void ParticleSystem::drawSelf() {
    m_quads.resize(m_particles.size());
    for (auto i = 0; i < m_quads.size(); i++) {
        m_quads[i].pos = m_particles[i].pos;
        m_quads[i].size = m_particles[i].size;
        m_quads[i].startxp = m_quads[i].startyp = 0.0f;
        m_quads[i].endxp = m_quads[i].endyp = 1.0f;
    }
    if (m_quads.empty()) {
        return;
    }
    UIParticleDrawer drawer;
    drawer.setTexture({m_texture});
    drawer.setQuads(m_quads);
    drawer.draw();
}