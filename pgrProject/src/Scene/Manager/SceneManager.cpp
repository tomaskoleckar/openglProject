#include "SceneManager.h"
#include "../../transform/CompositeTransform.h"
#include "../../transform/Translate.h"
#include "../../transform/Rotate.h"
#include "../../transform/Scale.h"
#include "../../Model/Model.h"
#include "../../Model/Factory/ModelFactory.h"
#include "../../Model/Mesh.h"

std::shared_ptr<Shader> SceneManager::getShader(const std::string& name) const
{
    auto it = m_shaders.find(name);
    if (it == m_shaders.end())
        return nullptr;
    return it->second;
}

void SceneManager::initDefaultShaders()
{
    m_shaders["basic"] = std::make_shared<Shader>(
        std::make_shared<ShaderProgram>("shaders/basic.vert", "shaders/basic.frag"));

    m_shaders["phong"] = std::make_shared<Shader>(
        std::make_shared<ShaderProgram>("shaders/Phong/phong.vert", "shaders/Phong/phong.frag"));

    m_shaders["blinn"] = std::make_shared<Shader>(
        std::make_shared<ShaderProgram>("shaders/Blinn/blinn.vert", "shaders/Blinn/blinn.frag"));

    m_shaders["lambert"] = std::make_shared<Shader>(
        std::make_shared<ShaderProgram>("shaders/Lambert/lambert.vert", "shaders/Lambert/lambert.frag"));

    m_shaders["constant"] = std::make_shared<Shader>(
        std::make_shared<ShaderProgram>("shaders/Constant/constant.vert", "shaders/Constant/constant.frag"));

    m_shaders["skybox"] = std::make_shared<Shader>(
        std::make_shared<ShaderProgram>("shaders/SkyBox/skybox.vert", "shaders/SkyBox/skybox.frag"));
}

void SceneManager::updateModelPosition(GLuint id, const glm::vec3& newPosition)
{
    if (!m_scene)
        return;

    if (auto model = m_scene->findModelByID(id))
    {
        model->setModelPosition(newPosition);
    }
}



std::shared_ptr<Model> SceneManager::spawnModel(const ModelParams& p)
{
    auto root = ModelFactory::loadOBJWithShader(
        p.objPath,
        p.textureDir,
        p.shader
    );

    if (p.shader) {
        root->material().setShader(p.shader);
        for (auto& c : root->children())
            c->material().setShader(p.shader);
    }

    auto t = std::make_shared<CompositeTransform>();
    t->add(std::make_shared<Translate>(p.position.x,
        p.position.y,
        p.position.z));
    t->add(std::make_shared<Rotate>(p.rotation.y,
        glm::vec3(0, 1, 0)));
    t->add(std::make_shared<Scale>(p.scale.x,
        p.scale.y,
        p.scale.z));

    root->setTransform(t);

    if (p.assignID)
        root->setID(m_scene->nextID());

    return root;
}

SceneModelBuilder SceneManager::builder()
{
    return SceneModelBuilder(*this);
}