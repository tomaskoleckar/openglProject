#pragma once
#include "Transform.h"
#include <vector>

class CompositeTransform : public Transform {
public:
    void add(TransformPtr t) { m_children.push_back(std::move(t)); }

    glm::mat4 getMatrix() const override {
        glm::mat4 result(1.0f);
        for (const auto& child : m_children)
            result *= child->getMatrix();
        return result;
    }

private:
    std::vector<TransformPtr> m_children;
};
