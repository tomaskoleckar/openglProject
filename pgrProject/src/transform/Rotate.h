#pragma once
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

class Rotate : public Transform {
public:
    Rotate(float angleDeg, const glm::vec3& axis)
        : m_angle(angleDeg), m_axis(axis) {}
    glm::mat4 getMatrix() const override {
        return glm::rotate(glm::mat4(1.0f),
            glm::radians(m_angle), m_axis);
    }
private:
    float m_angle;
    glm::vec3 m_axis;
};
