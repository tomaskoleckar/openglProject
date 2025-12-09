#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>

class Shader;
class Camera;
class Scene;
class Model;
class Light;
enum class ShadingMode;

class Renderer {
public:
    Renderer() = default;
    explicit Renderer(std::shared_ptr<Shader> shader);

    void setShader(std::shared_ptr<Shader> shader);

    void setCamera(const Camera& camera);
    void setShadingMode(ShadingMode mode);
    void setBugMode(bool enabled);
    void setSelectedId(int id);
    void setLights(const std::vector<Light>& lights);

    void renderSkybox(Scene& scene);

    void beginFrame(const glm::vec4& clearColor);

    void renderScene(Scene& scene);

    void renderModel(const Model& model, const glm::mat4& worldMat);

private:
    void uploadPerFrameUniforms();
    void applyPerFrameToShader(Shader& shader);

private:

    std::shared_ptr<Shader> m_shader;

    glm::mat4 m_view{ 1.0f };
    glm::mat4 m_proj{ 1.0f };
    glm::vec3 m_viewPos{ 0.0f };

    ShadingMode m_shadingMode{};
    bool m_bugMode = false;
    int m_selectedId = -1;

    std::vector<Light> m_lights;
};
