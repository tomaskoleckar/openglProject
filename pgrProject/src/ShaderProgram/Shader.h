#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>

class ShaderProgram;
class Light;
class Camera;
enum class ShadingMode;
struct Material;

class Shader {
public:
    Shader(std::shared_ptr<ShaderProgram> program);

    void use() const;

    void setCamera(const Camera& cam);
    void setLights(const std::vector<Light>& lights);
    void setShadingMode(ShadingMode mode);
    void setBugMode(bool enabled);
    void setSelectedId(int id);

    void setCameraMatrices(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos);

    void setModelMatrix(const glm::mat4& m);

    ShaderProgram* program() const { return m_program.get(); }

private:
    std::shared_ptr<ShaderProgram> m_program;
};
