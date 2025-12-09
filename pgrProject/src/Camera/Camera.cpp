#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fovDeg, float aspect, float nearZ, float farZ)
    : m_fov(fovDeg), m_aspect(aspect), m_nearZ(nearZ), m_farZ(farZ)
{
}

void Camera::move(const glm::vec3& delta) {
    m_position += delta;
    notifyState();
}

void Camera::look(float yawDelta, float pitchDelta) {
    m_yaw += yawDelta;
     m_pitch = glm::clamp(m_pitch + pitchDelta, -89.0f, 89.0f);
    notifyState();
}

void Camera::setPosition(const glm::vec3& pos) {
    m_position = pos;
    notifyState();
}

void Camera::setYawPitch(float yawDeg, float pitchDeg) {
    m_yaw = yawDeg;
    m_pitch = pitchDeg;
    notifyState();
}

void Camera::setFOV(float fovDeg) {
    m_fov = fovDeg;
    notifyState();
}

glm::vec3 Camera::forward() const {
    return glm::normalize(glm::vec3(
        cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
        sin(glm::radians(m_pitch)),
        sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
    ));
}

glm::mat4 Camera::getView() const {
    return glm::lookAt(m_position, m_position + forward(), { 0.0f, 1.0f, 0.0f });
}

glm::mat4 Camera::getProj() const {
    return glm::perspective(glm::radians(m_fov), m_aspect, m_nearZ, m_farZ);
}

void Camera::notifyState() {
    CameraState state{
        getView(),
        getProj(),
        m_position,
        forward()
    };

    notify(state);
}
