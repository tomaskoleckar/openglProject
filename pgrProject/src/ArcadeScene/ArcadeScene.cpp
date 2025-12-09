#include "ArcadeScene.h"
#include "../fourSpheres/sphere.h"
#include "../forest/Tree.h"
#include "../Model/Mesh.h"
#include "../transform/CompositeTransform.h"
#include "../transform/Translate.h"
#include "../transform/Scale.h"
#include <algorithm>
#include "../SkyBox/SkyBoxMesh.h"

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

ArcadeScene::ArcadeScene(Scene& scene, SceneManager& mgr)
    : m_scene(scene), m_mgr(mgr) ,
    m_rng(std::random_device{}()),
    m_posDist(-15.0f, 15.0f),
    m_speedDist(0.5f, 1.5f),
    m_lifeDist(4.0f, 8.0f),
    m_heightDist(0.0f, 0.5f)
{
    ensureMeshes();

    std::vector<std::string> faces = {
    "assets/sky/right.jpg",
    "assets/sky/left.jpg",
    "assets/sky/top.jpg",
    "assets/sky/bottom.jpg",
    "assets/sky/front.jpg",
    "assets/sky/back.jpg"
    };

    auto cubemap = std::make_shared<CubeMapTexture>(faces);
    auto skyboxShader = m_mgr.getShader("skybox");

    auto skyboxMesh = SkyboxMesh::create();

    auto skyboxModel = m_mgr.builder()
        .mesh(skyboxMesh)
        .shader("skybox")
        .assignID(false)
        .add();

    skyboxModel->material().setShader(skyboxShader);
    skyboxModel->material().setCubeMap(cubemap);

    m_scene.setSkybox(skyboxModel);

    for (int i = 0; i < 5; ++i)
        spawnGoblinInternal();

}

void ArcadeScene::ensureMeshes()
{
    if (!m_goblinMesh) {
        m_goblinMesh = makeSphereMesh();
    }
    if (!m_treeMesh) {
        m_treeMesh = makeTreeMeshFromHeader();
    }
}

void ArcadeScene::spawnGoblinInternal()
{
    ensureMeshes();

    glm::vec3 p0(m_posDist(m_rng), 0.0f, m_posDist(m_rng));
    glm::vec3 p1(m_posDist(m_rng), 0.0f, m_posDist(m_rng));
    glm::vec3 p2(m_posDist(m_rng), 0.0f, m_posDist(m_rng));

    float speedMultiplier = 0.8f;

    Goblin g;
    g.path = { p0, p1, p2 };
    g.t = 0.0f;
    g.segment = 0;
    g.speed = m_speedDist(m_rng) * speedMultiplier;
    g.life = m_lifeDist(m_rng);
    g.animTime = 0.0f;

    int type = rand() % 3;

    if (type == 0) {
        g.scoreValue = 5;
        g.color = glm::vec3(0.2f, 0.8f, 0.2f);
        g.life = m_lifeDist(m_rng);
    }
    else if (type == 1) {
        g.scoreValue = 10;
        g.color = glm::vec3(0.8f, 0.2f, 0.2f);
        g.life = m_lifeDist(m_rng) * 0.7f;
        g.speed *= 1.4f;
    }
    else {
        g.scoreValue = 20;
        g.color = glm::vec3(1.0f, 0.85f, 0.2f);
        g.life = m_lifeDist(m_rng) * 0.5f;
        g.speed *= 1.8f;
    }

    auto model = std::make_shared<Model>(m_goblinMesh);

    Material mat;
    mat.ra = glm::vec3(0.05f);
    mat.rd = glm::vec3(0.4f, 0.9f, 0.2f);
    mat.rs = glm::vec3(0.2f);
    mat.h = 16.0f;
    mat.setShader(m_mgr.GetDefaultShaders()["constant"]);
    model->setMaterial(mat);

    auto t = std::make_shared<CompositeTransform>();
    t->add(std::make_shared<Translate>(p0.x, p0.y, p0.z));
    t->add(std::make_shared<Scale>(0.4f));
    model->setTransform(t);

    model->setColor(g.color);

    m_scene.addModel(model);

    g.model = model;
    m_goblins.push_back(std::move(g));
}

void ArcadeScene::update(float dt)
{
    ensureMeshes();

    m_spawnTimer += dt;
    if (m_spawnTimer >= m_spawnInterval) {
        m_spawnTimer = 0.0f;
        m_spawnInterval = 1.0f + static_cast<float>(rand()) / RAND_MAX * 1.5f;

        if (m_goblins.size() < 20) {
            spawnGoblinInternal();
        }
    }

    std::vector<int> toRemove;

    for (auto& g : m_goblins) {
        g.life -= dt;
        g.animTime += dt;

        if (g.life <= 0.0f) {
            if (g.model)
                toRemove.push_back(static_cast<int>(g.model->id()));
            continue;
        }

        if (g.path.size() < 2 || !g.model)
            continue;

        g.t += dt * g.speed;
        while (g.t > 1.0f) {
            g.t -= 1.0f;
            g.segment++;
            if (g.segment >= static_cast<int>(g.path.size()) - 1) {
                g.segment = 0;
            }
        }

        int i = g.segment;
        glm::vec3 P0 = g.path[i];
        glm::vec3 P1 = g.path[i + 1];

        glm::vec3 pos = glm::mix(P0, P1, g.t);

        float jump = std::max(0.0f, sin(g.animTime * 5.0f)) * 1.0f;
        pos.y += jump + m_heightDist(m_rng) * 0.2f;

        auto t = std::make_shared<CompositeTransform>();
        t->add(std::make_shared<Translate>(pos.x, pos.y, pos.z));
        t->add(std::make_shared<Scale>(0.4f));
        g.model->setTransform(t);
    }

    for (int id : toRemove) {
        removeGoblinById(id);
    }

    m_goblins.erase(
        std::remove_if(m_goblins.begin(), m_goblins.end(),
            [](const Goblin& g) { return g.life <= 0.0f || !g.model; }),
        m_goblins.end()
    );
}

void ArcadeScene::onHit(int objectID)
{
    if (objectID <= 0)
        return;

    for (auto& g : m_goblins) {
        if (g.model && g.model->id() == objectID) {
            m_score += g.scoreValue;
            printf("Hit goblin ID=%d => +%d points!\n", objectID, g.scoreValue);
            removeGoblinById(objectID);
            printf("Current score is %d!\n", m_score);
            return;
        }
    }
}

void ArcadeScene::removeGoblinById(int id)
{
    m_scene.removeByID(id);

    for (auto& g : m_goblins) {
        if (g.model && static_cast<int>(g.model->id()) == id) {
            g.life = -1.0f;
            g.model.reset();
        }
    }
}