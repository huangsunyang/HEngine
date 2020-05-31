#include "Camera.hpp"
#include <math.h>

Camera::Camera(vmath::vec3 pos) {
    m_cameraPos = pos;
    m_cameraUp = vmath::vec3(0, 1, 0);
    setCameraRotation(0, 0);

    m_near = 0.1;
    m_far = 100.0;
    m_fovy = 30.0;
    m_aspect = 1;
}

Camera::Camera(): Camera(vmath::vec3(-5, 0, 0)) {}

Camera::~Camera() {

}

void Camera::setCameraPos(vmath::vec3 pos) {
    m_cameraPos = pos;
}

void Camera::moveCameraBy(float x, float y, float z) {
    vmath::vec3 up = normalize(cross(m_cameraRight, m_cameraFront));
    m_cameraPos += x * m_cameraRight;
    m_cameraPos += y * up;
    m_cameraPos += z * m_cameraFront;
}

void Camera::rotateCameraBy(float yaw, float pitch) {
    setCameraRotation(m_cameraYaw + yaw, m_cameraPitch + pitch);
}

void Camera::setCameraYaw(float yaw) {
    setCameraRotation(yaw, m_cameraPitch);
}

void Camera::setCameraPitch(float pitch) {
    setCameraRotation(m_cameraYaw, pitch);
}

void Camera::setCameraRotation(float yaw, float pitch) {
    m_cameraYaw = yaw;
    if (pitch > M_PI_2 - 0.3) {
        pitch = M_PI_2 - 0.3;
    } else if (pitch < 0.3 - M_PI_2) {
        pitch = 0.3 - M_PI_2;
    }
    m_cameraPitch = pitch;
    updateCameraFront();
}

void Camera::updateCameraFront() {
    m_cameraFront[0] = cosf(m_cameraPitch) * cosf(m_cameraYaw);
    m_cameraFront[1] = sinf(m_cameraPitch);
    m_cameraFront[2] = cosf(m_cameraPitch) * sinf(m_cameraYaw);
    m_cameraRight = normalize(cross(m_cameraFront, m_cameraUp));
}

float Camera::getCameraYaw() {
    return m_cameraYaw;
}

float Camera::getCameraPitch() {
    return m_cameraPitch;
}

void Camera::moveCameraBy(vmath::vec3 diff) {
    moveCameraBy(diff[0], diff[1], diff[2]);
}

vmath::mat4 Camera::getCameraTransform() {
    return vmath::lookat(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

vmath::mat4 Camera::getProjectionMatrix() {
    return vmath::perspective(m_fovy, m_aspect, m_near, m_far);
}

void Camera::setAspect(float x) {
    m_aspect = x;
}

void Camera::setFovy(float x) {
    m_fovy = x;
}

void Camera::setNear(float x) {
    m_near = x;
}

void Camera::setFar(float y) {
    m_far = y;
}