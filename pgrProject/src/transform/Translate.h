#pragma once
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

class Translate : public Transform {
public:
    Translate(float x, float y, float z) : m_vec(x, y, z) {}
    glm::mat4 getMatrix() const override {
        return glm::translate(glm::mat4(1.0f), m_vec);
    }

    void setPosition(const glm::vec3& position) {
        m_vec = position;
    }
private:
    glm::vec3 m_vec;
};
