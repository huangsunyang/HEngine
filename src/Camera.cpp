#include "Camera.hpp"
#include <math.h>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "LogManager.hpp"

Camera::Camera(glm::vec3 pos) {
    m_cameraPos = pos;
    m_cameraUp = glm::vec3(0, 1, 0);
    setCameraRotation(0, 0);

    m_near = 0.1f;
    m_far = 100.0f;
    m_fovy = glm::radians(30.0f);
    m_aspect = 1.f;
}

Camera::Camera(): Camera(glm::vec3(-5, 0, 0)) {}

Camera::~Camera() {

}

void Camera::setCameraPos(glm::vec3 pos) {
    m_cameraPos = pos;
}

void Camera::moveCameraBy(float x, float y, float z) {
    glm::vec3 up = normalize(cross(m_cameraRight, m_cameraFront));
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
    auto M_PI_2 = glm::half_pi<float>();
    if (pitch > M_PI_2 - 0.3f) {
        pitch = float(M_PI_2 - 0.3f);
    } else if (pitch < 0.3f - M_PI_2) {
        pitch = float(0.3f - M_PI_2);
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

glm::vec3 Camera::getCameraFront() {
    return m_cameraFront;
}

void Camera::moveCameraBy(glm::vec3 diff) {
    moveCameraBy(diff[0], diff[1], diff[2]);
}

glm::mat4 Camera::getCameraTransform() {
    return glm::lookAtRH(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

glm::mat4 Camera::getProjectionMatrix() {
    auto a = glm::perspectiveRH_NO(m_fovy, m_aspect, m_near, m_far);
    return a;
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