#pragma once
#include <vector>
#include <memory>
#include "../Model/Model.h"
#include "Manager/SceneManager.h"

class Renderer;
class SceneManager;

class Scene {
public:
    void addModel(std::shared_ptr<Model> model, bool withoutId = false);

    void render(Renderer& renderer);

    void select(int id);
    Model* getSelected();
    int nextID();
    void removeSelected();

    void spawnTreeAt(const glm::vec3& worldPos, SceneManager& m_mgr);
    void removeByID(int id);

    void setTreeMesh(std::shared_ptr<Mesh> mesh) { m_treeMesh = mesh; }
    std::shared_ptr<Mesh> getTreeMesh() const { return m_treeMesh; }

    void setTreeTexture(std::shared_ptr<Texture> tex) { m_treeTexture = tex; }
    std::shared_ptr<Model> findModelByID(GLuint id);

    void setSkybox(std::shared_ptr<Model> m) { m_skybox = m; }
    std::shared_ptr<Model> skyboxModel() const { return m_skybox; }
private:
    void renderModelRecursive(Renderer& renderer, const std::shared_ptr<Model>& m, const glm::mat4& parentMat);

    std::shared_ptr<Model> m_skybox;

    std::vector<std::shared_ptr<Model>> m_models;

    std::shared_ptr<Mesh> m_treeMesh;
    std::shared_ptr<Texture> m_treeTexture;


    int m_selectedID = -1;
};
