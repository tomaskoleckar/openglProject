#include "FormulaScene.h"
#include "../Model/Model.h"
#include "../material/Material.h"
#include "../transform/MatrixTransform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../fourSpheres/Sphere.h"
#include <iostream>

FormulaScene::FormulaScene(Scene& scene, SceneManager& mgr)
    : m_scene(scene), m_mgr(mgr)
{

    m_mgr.builder()
        .mesh(Mesh::makeQuad())
        .shader("constant")
        .color({ 0,0,0 })
        .scale(200.0f)
        .assignID(false)
        .add();

    m_mgr.builder()
        .path("assets/models/zed/zed.obj")
        .textureDir("assets/models/zed/")
        .assignID(false)
        .position({ 0.0f, 0.0f, 0.0f })
        .add();

    m_controlSphereMesh = std::make_shared<Mesh>(
        std::vector<float>(sphere, sphere + sizeof(sphere) / sizeof(float)),
        true,
        false 
    );
}

void FormulaScene::onClickPoint(const glm::vec3& worldPos)
{
    spawnControlPointMarker(worldPos);
    m_spline.addPoint(worldPos);

    drawBezierSegments();

    if (m_spline.segmentCount() > 0) {
        m_animStarted = true;
        m_t = 0.0f;

        spawnFormula();
    }
}

void FormulaScene::update(float dt)
{
    if (!m_animStarted || !m_spline.hasSegments() || !m_formula)
        return;

    int segCount = m_spline.segmentCount();
    if (segCount == 0)
        return;

    m_t += m_speed * dt;

    if (m_t >= segCount)
        m_t -= segCount;

    int segIndex = (int)m_t;
    float localT = m_t - segIndex;

    if (segIndex < 0) segIndex = 0;
    if (segIndex >= segCount) segIndex = segCount - 1;

    glm::vec3 tangent;
    glm::vec3 pos = m_spline.evaluateSegment(segIndex, localT, &tangent);

    if (glm::length(tangent) < 1e-6f)
        tangent = glm::vec3(0, 0, -1);

    glm::vec3 forward = glm::normalize(tangent);
    glm::vec3 up(0, 1, 0);

    if (fabs(glm::dot(forward, up)) > 0.95f)
        up = glm::vec3(1, 0, 0);

    glm::vec3 right = glm::normalize(glm::cross(forward, up));
    up = glm::normalize(glm::cross(right, forward));

    glm::mat4 M(1.0f);
    M[0] = glm::vec4(right, 0.0f);
    M[1] = glm::vec4(up, 0.0f);
    M[2] = glm::vec4(forward, 0.0f);
    M[3] = glm::vec4(pos, 1.0f);

    glm::mat4 carLocalRot = glm::rotate(
        glm::mat4(1.0f),
        glm::radians(90.0f),
        glm::vec3(0, 1, 0)
    );

    M = M * carLocalRot;

    m_formula->setTransform(std::make_shared<MatrixTransform>(M));

    std::cout << "T = " << m_t << "\n";
}



void FormulaScene::spawnFormula()
{
    if (m_formula)
        return;

    m_formula = m_mgr.builder()
        .path("assets/models/formula/formula.obj")
        .textureDir("assets/models/formula/")
        .shader("phong")
        .scale(0.04f)
        .position({ 0,0,0 })
        .assignID(false)
        .add();

    Material mat;
    mat.ra = glm::vec3(0.1f, 0.1f, 0.1f);
    mat.rd = glm::vec3(0.8f, 0.1f, 0.1f);
    mat.rs = glm::vec3(0.8f, 0.8f, 0.8f);
    mat.h = 64.0f;
    m_formula->setMaterial(mat);
}


std::vector<Light> FormulaScene::getLights() const
{
    std::vector<Light> lights;
    lights.push_back(Light::Ambient(glm::vec3(0.1f), 1.0f));
    lights.push_back(Light::Point(
        glm::vec3(0.0f, 10.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        4.0f
    ));
    return lights;
}


void FormulaScene::spawnControlPointMarker(const glm::vec3& p)
{
    m_mgr.builder()
        .mesh(m_controlSphereMesh)
        .color({ 1.0f, 0.2f, 0.2f })
        .scale(0.2f)
        .position(p)
        .assignID(false)
        .add();
}

void FormulaScene::drawBezierSegments()
{
    for (int id : m_curvePointIDs)
        m_scene.removeByID(id);

    m_curvePointIDs.clear();

    if (!m_spline.hasSegments())
        return;

    const int samplesPerSegment = 50;

    for (int s = 0; s < m_spline.segmentCount(); ++s)
    {
        for (int i = 0; i < samplesPerSegment; ++i)
        {
            float t = (float)i / (samplesPerSegment - 1);

            glm::vec3 p = m_spline.evaluateSegment(s, t);

            auto model = m_mgr.builder()
                .mesh(m_controlSphereMesh)
                .color({ 1.0f, 1.0f, 1.0f })
                .scale(0.08f)
                .position(p)
                .assignID(true)
                .add();

            m_curvePointIDs.push_back(model->id());
        }
    }
}

