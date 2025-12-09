#pragma once
#include <glm/glm.hpp>
#include <memory>

class Transform {
public:
    virtual ~Transform() = default;
    virtual glm::mat4 getMatrix() const = 0;
};
using TransformPtr = std::shared_ptr<Transform>;
