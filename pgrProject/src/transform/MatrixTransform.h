#pragma once
#include "Transform.h"
#include <glm/glm.hpp>

class MatrixTransform : public Transform {
public:
    explicit MatrixTransform(const glm::mat4& m) : m_mat(m) {}
    glm::mat4 getMatrix() const override { return m_mat; }

    void setMatrix(const glm::mat4& m) { m_mat = m; }

private:
    glm::mat4 m_mat;
};
