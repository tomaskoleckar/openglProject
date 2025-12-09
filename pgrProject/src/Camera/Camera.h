#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "../interfaces/ICameraObserver.h"
#include "CameraState.h"
#include "../interfaces/Subject.h"

class Camera : public Subject<CameraState> {
public:
    Camera(float fovDeg, float aspect, float nearZ, float farZ);

    void move(const glm::vec3& delta);
    void look(float yawDelta, float pitchDelta);

    void setPosition(const glm::vec3& pos);
    void setYawPitch(float yawDeg, float pitchDeg);
    void setFOV(float fovDeg);

    glm::mat4 getView() const;
    glm::mat4 getProj() const;

    glm::vec3 forward() const;


    float yaw() { return m_yaw; };
    float pitch() { return m_pitch; };
    const glm::vec3& position() const { return m_position; }

private:
    void notifyState();

    glm::vec3 m_position{ 0.0f, 1.6f, 3.0f };
    float m_yaw{ -90.0f };
    float m_pitch{ 0.0f };
    float m_fov;
    float m_aspect;
    float m_nearZ;
    float m_farZ;
};