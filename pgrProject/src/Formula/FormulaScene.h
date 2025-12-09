#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "../Scene/Scene.h"
#include "../Scene/Manager/SceneManager.h"
#include "../light/Light.h"
#include "../BezierSpline/BezierSpline.h"

class Model;
class Mesh;

class FormulaScene {
public:
    FormulaScene(Scene& scene, SceneManager& mgr);

    void onClickPoint(const glm::vec3& worldPos);
    void update(float dt);

    void spawnFormula();
    void drawBezierSegments();
    void spawnControlPointMarker(const glm::vec3& p);

    std::vector<Light> getLights() const;

private:
    Scene& m_scene;
    SceneManager& m_mgr;

    std::shared_ptr<Mesh> m_controlSphereMesh;

    std::vector<int> m_curvePointIDs;
    std::vector<int> m_controlPointIDs;

    std::shared_ptr<Model> m_formula;
    BezierSpline m_spline;

    float m_t = 0.0f;
    float m_speed = 0.3f;

    bool m_animStarted = false;
};
