#pragma once
#include <memory>
#include <unordered_map>
#include <string>

#include "../../ShaderProgram/Shader.h"
#include "../../ShaderProgram/ShaderProgram.h"
#include "../../Model/Factory/ModelParams.h"
#include "../Scene.h"
#include "SceneModelBuilder.h"

class Shader;
class ShaderProgram;
class Scene;
class Model;

class SceneManager {
public:
    SceneManager() = default;
    explicit SceneManager(Scene& scene) : m_scene(&scene) { initDefaultShaders(); }

    void setScene(Scene& scene) { m_scene = &scene; }
    Scene& scene() { return *m_scene; }

    SceneModelBuilder builder();
    void initDefaultShaders();

    std::unordered_map<std::string, std::shared_ptr<Shader>> GetDefaultShaders() { return m_shaders; };

    void updateModelPosition(GLuint id, const glm::vec3& newPosition);

    std::shared_ptr<Shader> getShader(const std::string& name) const;
    std::shared_ptr<Model> spawnModel(const ModelParams& p);

private:
    Scene* m_scene = nullptr;

    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
};
