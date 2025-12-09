#pragma once

#include "../Scene/Scene.h"
#include "../transform/Transform.h"
#include "../material/Material.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <random>
#include "../Scene/Manager/SceneManager.h"

class ArcadeScene
{
public:
    explicit ArcadeScene(Scene& scene, SceneManager& mgr);

    void update(float dt);

    void onHit(int objectID);

    int score() const { return m_score; }

private:
    Scene& m_scene;

    struct Goblin {
        std::shared_ptr<Model> model;
        std::vector<glm::vec3> path;
        float t = 0.0f;
        int   segment = 0;
        float speed = 1.0f;
        float life = 5.0f;
        float animTime = 0.0f;
        int scoreValue = 5;     
        glm::vec3 color = { 0.3f, 0.9f, 0.3f };
    };

    std::vector<Goblin> m_goblins;

    SceneManager& m_mgr;

    std::mt19937 m_rng;
    std::uniform_real_distribution<float> m_posDist;
    std::uniform_real_distribution<float> m_speedDist;
    std::uniform_real_distribution<float> m_lifeDist;
    std::uniform_real_distribution<float> m_heightDist;

    float m_spawnTimer = 0.0f;
    float m_spawnInterval = 1.5f;
    int   m_score = 0;

    std::shared_ptr<Mesh> m_goblinMesh;
    std::shared_ptr<Mesh> m_treeMesh;

    void ensureMeshes();
    void spawnGoblinInternal();
    void removeGoblinById(int id);
};
