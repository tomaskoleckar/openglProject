#include "ForestGenerator.h"

#include "../Model/Factory/ModelFactory.h"
#include "../../texture/Texture.h"
#include "../transform/CompositeTransform.h"
#include "../transform/Translate.h"
#include "../transform/Rotate.h"
#include "../transform/Scale.h"
#include "../fourSpheres/sphere.h"
#include "Tree.h"
#include "Bush.h"

ForestGenerator::ForestGenerator(
    Scene& scene,
    SceneManager& mgr,
    unsigned int seed)
    : m_scene(scene),
    m_mgr(mgr),
    m_rng(seed)
{
    m_phong = m_mgr.getShader("phong");
    m_blinn = m_mgr.getShader("blinn");
    m_lambert = m_mgr.getShader("lambert");
    m_constant = m_mgr.getShader("constant");
}

std::vector<Light> ForestGenerator::CreateFireFlies() {
    m_fireflies.clear();
    m_fireflyPhase.clear();
    m_fireflyCenter.clear();

    std::uniform_real_distribution<float> pos(-20.0f, 20.0f);
    std::uniform_real_distribution<float> ypos(1.5f, 3.0f);
    std::uniform_real_distribution<float> col(0.7f, 1.0f);
    std::uniform_real_distribution<float> phase(0.0f, 2.0f * 3.14159f);

    for (int i = 0; i < 8; ++i) {
        glm::vec3 center = { pos(m_rng), ypos(m_rng), pos(m_rng) };

        Light f = Light::Point(
            center,
            { col(m_rng), col(m_rng), 0.3f },
            15.0f
        );

        f.kc = 1.0f;
        f.kl = 0.7f;
        f.kq = 1.2f;

        m_fireflies.push_back(f);
        m_fireflyPhase.push_back(phase(m_rng));
        m_fireflyCenter.push_back(center);
    }
    return m_fireflies;
}

void ForestGenerator::makeShrek()
{
    m_mgr.builder()
        .path("assets/models/shrek/shrek.obj")
        .textureDir("assets/models/shrek/")
        .shader(m_phong)
        .scale(0.2f)
        .assignID(true)
        .position({ 0.0f, 0.0f, 5.0f })
        .add();

    m_mgr.builder()
        .path("assets/models/shrek2/shrek.obj")
        .textureDir("assets/models/shrek2/")
        .shader(m_lambert)
        .scale(1.0f)
        .assignID(true)
        .position({ 1.0f, 0.0f, 5.0f })
        .add();
}


void ForestGenerator::makeFionna()
{
    m_mgr.builder()
        .path("assets/models/fiona/fionna.obj")
        .textureDir("assets/models/fiona/")
        .shader(m_lambert)
        .assignID(true)
        .position({ 3.0f, 0.0f, 5.0f })
        .add();
}

void ForestGenerator::makeToilet() {
    m_mgr.builder()
        .path("assets/models/toilet/toiled.obj")
        .textureDir("assets/models/toilet/")
        .shader(m_blinn)
        .assignID(true)
        .position({ 2.0f, 0.0f, 5.0f })
        .add();
}

void ForestGenerator::UpdateFireflies(float time)
{
    float speed = 1.5f;
    float radius = 0.5f;

    for (size_t i = 0; i < m_fireflies.size(); ++i)
    {
        if (i >= m_fireflyCenter.size() ||
            i >= m_fireflyPhase.size() ||
            i >= m_fireflyModelIDs.size())
            continue;

        float phaseX = m_fireflyPhase[i];

        glm::vec3 offset;
        offset.x = radius * std::sin(time * speed + phaseX);
        offset.y = radius * 0.5f * std::cos(time * speed * 0.5f + phaseX * 0.7f);
        offset.z = radius * std::sin(time * speed * 1.2f + phaseX * 1.3f);

        glm::vec3 newPosition = m_fireflyCenter[i] + offset;

        m_fireflies[i].position = newPosition;

        m_mgr.updateModelPosition(m_fireflyModelIDs[i], newPosition);
    }
}

static std::shared_ptr<Mesh> makeSphereMesh()
{
    std::vector<float> data;
    int vertexCount = sizeof(sphere) / sizeof(float) / 6;
    data.reserve(vertexCount * 6);

    for (int i = 0; i < vertexCount; ++i) {
        data.push_back(sphere[i * 6 + 0]);
        data.push_back(sphere[i * 6 + 1]);
        data.push_back(sphere[i * 6 + 2]);
        data.push_back(sphere[i * 6 + 3]);
        data.push_back(sphere[i * 6 + 4]);
        data.push_back(sphere[i * 6 + 5]);
    }

    return std::make_shared<Mesh>(data, true, false);
}

static std::shared_ptr<Mesh> makeTreeMeshFromHeader()
{
    std::vector<float> data;
    data.reserve(92814 * 6);

    for (int i = 0; i < 92814; ++i) {
        data.push_back(tree[i * 6 + 0]);
        data.push_back(tree[i * 6 + 1]);
        data.push_back(tree[i * 6 + 2]);
        data.push_back(tree[i * 6 + 3]);
        data.push_back(tree[i * 6 + 4]);
        data.push_back(tree[i * 6 + 5]);
    }

    return std::make_shared<Mesh>(data, true, false);
}

static std::shared_ptr<Mesh> makeBushMesh()
{
    std::vector<float> data;
    data.reserve(8730 * 6);

    for (int i = 0; i < 8730; ++i) {
        data.push_back(bushes[i * 6 + 0]);
        data.push_back(bushes[i * 6 + 1]);
        data.push_back(bushes[i * 6 + 2]);
        data.push_back(bushes[i * 6 + 3]);
        data.push_back(bushes[i * 6 + 4]);
        data.push_back(bushes[i * 6 + 5]);
    }

    return std::make_shared<Mesh>(data, true, false);
}


void ForestGenerator::generate(int treeCount, int bushCount)
{
    makeShrek();
    makeFionna();
    makeToilet();

    auto texGrass = std::make_shared<Texture>("assets/textures/grass.jpg", true);
    auto texTree = std::make_shared<Texture>("assets/textures/tree_bark.jpg", true);
    auto texBush = std::make_shared<Texture>("assets/textures/leaves.jpg", true);

    m_scene.setTreeTexture(texTree);

    m_mgr.builder()
        .mesh(Mesh::makeQuad())
        .texture(texGrass)
        .color({ 0.2f, 0.5f, 0.2f })
        .scale(100.0f)
        .assignID(false)
        .add();

    std::uniform_real_distribution<float> pos(-20.f, 20.f);
    std::uniform_real_distribution<float> rot(0.f, 360.f);
    std::uniform_real_distribution<float> treeScale(0.6f, 1.2f);
    std::uniform_real_distribution<float> bushScale(1.2f, 3.0f);
    std::uniform_real_distribution<float> greenShade(0.5f, 0.8f);
    std::uniform_real_distribution<float> bushGreen(0.3f, 0.6f);

    auto treeMesh = makeTreeMeshFromHeader();
    auto bushMesh = makeBushMesh();
    auto sphereMesh = makeSphereMesh();

    for (int i = 0; i < treeCount; ++i)
    {
        float g = greenShade(m_rng);

        m_mgr.builder()
            .mesh(treeMesh)
            .color({ g * 0.4f, g, g * 0.3f })
            .position({ pos(m_rng), 0.f, pos(m_rng) })
            .rotation({ 0.f, rot(m_rng), 0.f })
            .scale(treeScale(m_rng))
            .texture(texTree)
            .add();
    }

    for (int i = 0; i < bushCount; ++i)
    {
        float g = bushGreen(m_rng);

        m_mgr.builder()
            .mesh(bushMesh)
            .color({ 0.15f, g, 0.15f })
            .position({ pos(m_rng), 0.f, pos(m_rng) })
            .rotation({ 0.f, rot(m_rng), 0.f })
            .scale(bushScale(m_rng))
            .texture(texBush)
            .add();
    }

    m_fireflyModelIDs.clear();

    for (size_t i = 0; i < m_fireflies.size(); ++i)
    {
        auto translate = std::make_shared<Translate>(m_fireflies[i].position.x, m_fireflies[i].position.y, m_fireflies[i].position.z);
        auto scale = std::make_shared<Scale>(0.01f);

        auto composite = std::make_shared<CompositeTransform>();
        composite->add(translate);
        composite->add(scale);

        auto model = m_mgr.builder()
            .mesh(sphereMesh)
            .color({ 1,1,1 })
            .assignID(true)
            .add();

        model->setTransform(composite);

        model->setPositionTransform(translate);

        model->setIsFirefly(true);
        m_fireflyModelIDs.push_back(model->id());
    }
}
