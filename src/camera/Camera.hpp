#ifndef __CAMERA__
#define __CAMERA__

#include "glm/glm.hpp"
#include "ui/shape2d.hpp"
#include "GLObject/FrameBuffer.hpp"

class FrameBuffer;

class Camera {
public:
    Camera();
    Camera(glm::vec3 pos);
    ~Camera();

    void draw();
    void drawToScreen();
    void setActive();

    void setCameraPos(float x, float y, float z){ m_cameraPos = glm::vec3(x, y, z);}
    void setCameraPos(glm::vec3 pos) { m_cameraPos = pos;}
    glm::vec3 getCameraPos() { return m_cameraPos; }

    void setDepth(int depth) { m_depth = depth; }
    int getDepth() { return m_depth; }

    void moveCameraBy(glm::vec3);
    void moveCameraBy(float x, float y, float z);
    void rotateCameraBy(float x, float y);

    void lookAt(glm::vec3 pos);
    void lookAt(float x, float y, float z);
    void setCameraFront(glm::vec3);
    void setCameraFront(float x, float y, float z);

    void setCameraYaw(float);
    void setCameraPitch(float);
    void setCameraRotation(float yaw, float pitch);

    float getCameraYaw();
    float getCameraPitch();
    glm::vec3 getCameraFront();
    glm::mat4 getCameraTransform();
    glm::mat4 getProjectionMatrix();

    void setFovy(float);
    void setAspect(float);
    void setNear(float);
    void setFar(float);

    void setViewPos(float x, float y) {m_viewportPos = glm::vec2(x, y);_initScreenDrawer();}
    void setViewSize(float x, float y) {m_viewportSize = glm::vec2(x, y);_initScreenDrawer();}

    void setPerspective(bool val) {m_isPerspective = val;}

    FrameBuffer * getFrameBuffer() {return m_frameBuffer;}
    Texture2D * getTexture() {return m_frameBuffer ? m_frameBuffer->getTexture() : nullptr;}
    Texture2D * getDepthTexture() {return m_frameBuffer ? m_frameBuffer->getDepthTexture() : nullptr;}

protected:
    void _draw();
    void _updateCameraFront();
    void _initFrameBuffer();
    void _initScreenDrawer();

    glm::vec3 m_cameraPos;

    float m_cameraYaw;
    float m_cameraPitch;

    glm::vec3 m_cameraFront;
    glm::vec3 m_cameraUp;
    glm::vec3 m_cameraRight;

    float m_fovy;
    float m_aspect;
    float m_near;
    float m_far;
    float m_isPerspective;

    glm::vec2 m_viewportPos;
    glm::vec2 m_viewportSize;

    int m_depth;

    HPolygon * m_screen;
    FrameBuffer * m_frameBuffer;
};

#endif