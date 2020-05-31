#ifndef __CAMERA__
#define __CAMERA__

#include "sb7/vmath.h"


class Camera {
public:
    Camera();
    Camera(vmath::vec3 pos);
    ~Camera();

    void setCameraPos(vmath::vec3);
    void moveCameraBy(vmath::vec3);
    void moveCameraBy(float x, float y, float z);
    void rotateCameraBy(float x, float y);

    void lookAt(vmath::vec3 pos);
    void lookAt(float x, float y, float z);
    void setCameraFront(vmath::vec3);
    void setCameraFront(float x, float y, float z);

    void setCameraYaw(float);
    void setCameraPitch(float);
    void setCameraRotation(float yaw, float pitch);

    float getCameraYaw();
    float getCameraPitch();
    vmath::mat4 getCameraTransform();
    vmath::mat4 getProjectionMatrix();

    void setFovy(float);
    void setAspect(float);
    void setNear(float);
    void setFar(float);

protected:
    void updateCameraFront();

    vmath::vec3 m_cameraPos;

    float m_cameraYaw;
    float m_cameraPitch;

    vmath::vec3 m_cameraFront;
    vmath::vec3 m_cameraUp;
    vmath::vec3 m_cameraRight;

    float m_fovy;
    float m_aspect;
    float m_near;
    float m_far;
};

#endif