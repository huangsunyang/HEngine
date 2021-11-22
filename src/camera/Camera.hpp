#ifndef __CAMERA__
#define __CAMERA__

#include "glm/glm.hpp"

class FrameBuffer;

class Camera {
public:
    Camera();
    Camera(glm::vec3 pos);
    ~Camera();

    void draw();
    void drawToFrameBuffer(FrameBuffer *);
    void setActive();

    void setCameraPos(float, float, float);
    void setCameraPos(glm::vec3);
    glm::vec3 getCameraPos() { return m_cameraPos; }
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

    void setPerspective(bool val) {m_isPerspective = val;}

protected:
    void updateCameraFront();

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
};

#endif