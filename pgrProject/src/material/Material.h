#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "../../texture/Texture.h"
#include "../SkyBox/CubeMapTexture.h"

class Shader;
class Texture;

class Material {
public:
    Material() = default;

    void setShader(std::shared_ptr<Shader> shader);
    std::shared_ptr<Shader> shader() const;

    void setAmbient(const glm::vec3& v) { ra = v; }
    void setDiffuse(const glm::vec3& v) { rd = v; }
    void setSpecular(const glm::vec3& v) { rs = v; }
    void setShininess(float value) { h = value; }

    void setTexture(std::shared_ptr<Texture> tex);
    std::shared_ptr<Texture> texture() const { return m_texture; }
    bool hasTexture() const { return m_texture != nullptr; }


    void setCubeMap(std::shared_ptr<CubeMapTexture> tex) { m_cubemap = tex; }
    std::shared_ptr<CubeMapTexture> cubeMap() const { return m_cubemap; }
    bool hasCubeMap() const { return m_cubemap != nullptr; }


    void upload(Shader& shader) const;

public:
    glm::vec3 ra{ 0.1f };
    glm::vec3 rd{ 0.8f };
    glm::vec3 rs{ 0.2f };
    float h{ 32.0f };

private:
    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Texture> m_texture;
    std::shared_ptr<CubeMapTexture> m_cubemap;
};
