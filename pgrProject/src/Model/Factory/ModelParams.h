#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <string>

class Shader;
class Texture;
class Transform;
class Mesh;

struct ModelParams {
    std::string objPath;
    std::string textureDir;

    std::shared_ptr<Mesh> mesh = nullptr;
    bool fromMesh = false;

    std::shared_ptr<Shader> shader = nullptr;
    glm::vec3 position = { 0,0,0 };
    glm::vec3 rotation = { 0,0,0 };
    glm::vec3 scale = { 1,1,1 };

    bool assignID = true;
};
