#pragma once
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

class Scale : public Transform {
public:
    explicit Scale(float s) : m_vec(s, s, s) {}
    Scale(float x, float y, float z) : m_vec(x, y, z) {}
    glm::mat4 getMatrix() const override {
        return glm::scale(glm::mat4(1.0f), m_vec);
    }
private:
    glm::vec3 m_vec;
};
