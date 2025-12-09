#pragma once
#include <memory>
#include <string>
#include <glm/glm.hpp>

class SceneManager;
class Shader;
class Texture;
class Material;
class Model;
class Mesh;

class SceneModelBuilder {
public:
    SceneModelBuilder(SceneManager& mgr);

    SceneModelBuilder& path(const std::string& p);
    SceneModelBuilder& textureDir(const std::string& d);
    SceneModelBuilder& shader(std::shared_ptr<Shader> s);
    SceneModelBuilder& shader(const std::string& shaderName);

    SceneModelBuilder& position(const glm::vec3& p);
    SceneModelBuilder& rotation(const glm::vec3& r);
    SceneModelBuilder& scale(float s);
    SceneModelBuilder& scale(const glm::vec3& s);

    SceneModelBuilder& color(const glm::vec3& c);
    SceneModelBuilder& texture(std::shared_ptr<Texture> t);

    SceneModelBuilder& material(const Material& m);
    SceneModelBuilder& mesh(std::shared_ptr<Mesh> m);


    SceneModelBuilder& projection(const glm::mat4& P);
    SceneModelBuilder& assignID(bool enabled);

    std::shared_ptr<Model> build();
    std::shared_ptr<Model> add();

private:
    SceneManager& m_mgr;

    std::string m_path;
    std::string m_texDir;

    std::shared_ptr<Shader> m_shader;

    glm::vec3 m_pos{ 0,0,0 };
    glm::vec3 m_rot{ 0,0,0 };
    glm::vec3 m_scale{ 1,1,1 };
    glm::vec3 m_color{ 1,1,1 };

    std::shared_ptr<Texture> m_texture;

    bool m_hasMaterial = false;
    Material* m_materialPtr = nullptr;

    std::shared_ptr<Mesh> m_mesh;
    bool m_fromMesh = false;

    bool m_hasProjection = false;
    glm::mat4 m_projection = glm::mat4(1.0f);

    bool m_assignID = true;
};
