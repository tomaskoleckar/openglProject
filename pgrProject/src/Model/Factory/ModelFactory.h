#pragma once
#include <memory>
#include <string>
#include <vector>
#include "../Model.h"

class Shader;
class Texture;

class ModelFactory {
public:
    static std::shared_ptr<Model> loadOBJWithShader(
        const std::string& objPath,
        const std::string& baseDir,
        std::shared_ptr<Shader> shader);
};
