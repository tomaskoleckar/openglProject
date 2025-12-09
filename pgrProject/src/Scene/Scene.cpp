#include "Scene.h"
#include "../Renderer/Renderer.h"
#include "../transform/Translate.h"
#include "../transform/CompositeTransform.h"
#include "../transform/Scale.h"
#include "../transform/Rotate.h"
#include "../forest/Tree.h"

void Scene::addModel(std::shared_ptr<Model> model, bool withoutId)
{
    if (!withoutId)
        model->setID(nextID());

    m_models.push_back(std::move(model));
}

void Scene::render(Renderer& renderer)
{
    for (auto& m : m_models) {
        renderModelRecursive(renderer, m, glm::mat4(1.0f));
    }
}

std::shared_ptr<Model> Scene::findModelByID(GLuint id)
{
    if (id <= 0)
        return nullptr;

    for (auto& m : m_models) {
        if (m->id() == id)
            return m;
    }
    return nullptr;
}

void Scene::renderModelRecursive(Renderer& renderer,
    const std::shared_ptr<Model>& m,
    const glm::mat4& parentMat)
{
    glm::mat4 world = parentMat * m->transform();
    renderer.renderModel(*m, world);
    for (auto& child : m->children()) {
        renderModelRecursive(renderer, child, world);
    }
}

void Scene::select(int id)
{
    m_selectedID = id;

    for (auto& m : m_models) {
        if (m->id() == id)
            m->setColor(glm::vec3(1.0f, 0.2f, 0.2f));
        else
            m->setColor(glm::vec3(1.0f));
    }
}

Model* Scene::getSelected()
{
    if (m_selectedID < 0)
        return nullptr;

    for (auto& m : m_models) {
        if (m->id() == m_selectedID)
            return m.get();
    }
    return nullptr;
}

int Scene::nextID()
{
    return m_models.size() + 1;
}

void Scene::removeSelected()
{
    if (m_selectedID < 0) return;

    m_models.erase(
        std::remove_if(m_models.begin(), m_models.end(),
            [&](const std::shared_ptr<Model>& m) {
                return m->id() == m_selectedID;
            }),
        m_models.end()
    );

    m_selectedID = -1;
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

void Scene::spawnTreeAt(const glm::vec3& pos, SceneManager& m_mgr)
{
    if (!m_treeMesh)
        m_treeMesh = makeTreeMeshFromHeader();

    auto model = m_mgr.builder()
        .mesh(m_treeMesh)
        .texture(m_treeTexture)
        .color({ 0.4f, 0.8f, 0.4f }) 
        .scale(1.0f)
        .rotation(glm::vec3(0, 1, 0))
        .position(glm::vec3(pos.x, 0.0f, pos.z))
        .assignID(true)
        .add();

    Material wood;
    wood.ra = { 0.1f, 0.08f, 0.05f };
    wood.rd = { 0.4f, 0.3f, 0.2f };
    wood.rs = { 0.05f, 0.05f, 0.05f };
    wood.h = 8.0f;
    model->setMaterial(wood);
}

void Scene::removeByID(int id)
{
    m_models.erase(
        std::remove_if(m_models.begin(), m_models.end(),
            [id](const std::shared_ptr<Model>& m) {
                return m->id() == id;
            }),
        m_models.end()
    );

    if (m_selectedID == id)
        m_selectedID = -1;
}
