#pragma once
#include <random>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "../Scene/Scene.h"
#include "../Scene/Manager/SceneManager.h"
#include "../light/Light.h"

class ForestGenerator {
public:
    ForestGenerator(Scene& scene, SceneManager& mgr, unsigned int seed = std::random_device{}());

    void generate(int treeCount, int bushCount);
    std::vector<Light> CreateFireFlies();

    std::vector<Light>& getFireflies() { return m_fireflies; }

    void UpdateFireflies(float time);

private:
    void makeShrek();
    void makeFionna();
    void makeToilet();

    std::vector<float> m_fireflyPhase;
    std::vector<glm::vec3> m_fireflyCenter;
    std::vector<GLuint> m_fireflyModelIDs; 

    Scene& m_scene;
    SceneManager& m_mgr;

    std::mt19937 m_rng;
    std::vector<Light> m_fireflies;

    std::shared_ptr<Shader> m_phong;
    std::shared_ptr<Shader> m_blinn;
    std::shared_ptr<Shader> m_lambert;
    std::shared_ptr<Shader> m_constant;
};