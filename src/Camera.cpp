#include "Camera.hpp"
#include <math.h>
#include "sb7/sb7color.h"
#include "base/Director.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "LogManager.hpp"
#include "GLObject/UniformBlock.hpp"
#include "GLObject/FrameBuffer.hpp"

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

void Camera::draw() {
    setActive();
    Director::instance()->draw3D();
}

void Camera::drawToFrameBuffer(FrameBuffer * frameBuffer) {
    frameBuffer->bind(GL_FRAMEBUFFER);
    auto texture = frameBuffer->getTexture();

    glViewport(0, 0, texture->getWidth(), texture->getHeight());
    glClearBufferfv(GL_COLOR, 0, sb7::color::Gray);
    glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);

    draw();

    frameBuffer->unbind(GL_FRAMEBUFFER);
}

void Camera::setActive() {
    auto camera_matrix = getCameraTransform();
    auto proj_matrix = getProjectionMatrix();
    UniformBlock::instance()->setUniformBlockMember("view_matrix", glm::value_ptr(camera_matrix));
    UniformBlock::instance()->setUniformBlockMember("proj_matrix", glm::value_ptr(proj_matrix));
}

void Camera::setCameraPos(float x, float y, float z) {
    m_cameraPos = glm::vec3(x, y, z);
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

void Camera::lookAt(glm::vec3 pos) {
    m_cameraFront = glm::normalize(pos - m_cameraPos);
    m_cameraPitch = asinf(m_cameraFront[1]);
    m_cameraYaw = acosf(m_cameraFront[0] / cosf(m_cameraPitch));
    auto yawSign = asinf(m_cameraFront[2] / cosf(m_cameraPitch));
    if (yawSign < 0) m_cameraYaw = 2 * glm::pi<float>() - m_cameraYaw;
    updateCameraFront();
}

void Camera::lookAt(float x, float y, float z) {
    lookAt(glm::vec3(x, y, z));
}

glm::mat4 Camera::getCameraTransform() {
    return glm::lookAtRH(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

glm::mat4 Camera::getProjectionMatrix() {
    if (m_isPerspective) {
        return glm::perspectiveRH_NO(m_fovy, m_aspect, m_near, m_far);
    } else {
        return glm::orthoRH_NO(-20.0f, 20.0f, -20.0f, 20.0f, m_near, m_far);
    }
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