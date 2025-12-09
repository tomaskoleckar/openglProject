#include "SceneModelBuilder.h"
#include "SceneManager.h"

#include "../../transform/Rotate.h"
#include "../../transform/Translate.h"
#include "../../transform/Scale.h"
#include "../../Model/Factory/ModelFactory.h"
#include "../../Scene/Scene.h"
#include "../../transform/CompositeTransform.h"
#include "../../Model/Model.h"
#include "../../Model/Mesh.h"


SceneModelBuilder::SceneModelBuilder(SceneManager& mgr)
    : m_mgr(mgr)
{
}

SceneModelBuilder& SceneModelBuilder::path(const std::string& p) {
    m_path = p;
    return *this;
}
SceneModelBuilder& SceneModelBuilder::textureDir(const std::string& d) {
    m_texDir = d;
    return *this;
}
SceneModelBuilder& SceneModelBuilder::shader(std::shared_ptr<Shader> s) {
    m_shader = s;
    return *this;
}
SceneModelBuilder& SceneModelBuilder::shader(const std::string& name) {
    m_shader = m_mgr.getShader(name);
    return *this;
}

SceneModelBuilder& SceneModelBuilder::position(const glm::vec3& p) {
    m_pos = p;
    return *this;
}
SceneModelBuilder& SceneModelBuilder::rotation(const glm::vec3& r) {
    m_rot = r;
    return *this;
}
SceneModelBuilder& SceneModelBuilder::scale(float s) {
    m_scale = glm::vec3(s);
    return *this;
}
SceneModelBuilder& SceneModelBuilder::scale(const glm::vec3& s) {
    m_scale = s;
    return *this;
}

SceneModelBuilder& SceneModelBuilder::color(const glm::vec3& c) {
    m_color = c;
    return *this;
}

SceneModelBuilder& SceneModelBuilder::texture(std::shared_ptr<Texture> t) {
    m_texture = t;
    return *this;
}

SceneModelBuilder& SceneModelBuilder::material(const Material& m) {
    m_hasMaterial = true;
    m_materialPtr = new Material(m);
    return *this;
}

SceneModelBuilder& SceneModelBuilder::projection(const glm::mat4& P) {
    m_hasProjection = true;
    m_projection = P;
    return *this;
}

SceneModelBuilder& SceneModelBuilder::assignID(bool enabled) {
    m_assignID = enabled;
    return *this;
}

SceneModelBuilder& SceneModelBuilder::mesh(std::shared_ptr<Mesh> m) {
    m_mesh = m;
    m_fromMesh = true;
    return *this;
}


std::shared_ptr<Model> SceneModelBuilder::build() {
    std::shared_ptr<Model> root;

    if (m_mesh) {
        root = std::make_shared<Model>(m_mesh);
    }
    else {
        root = ModelFactory::loadOBJWithShader(m_path, m_texDir, m_shader);
    }

    if (m_assignID)
    {
        int id = m_mgr.scene().nextID();
        root->setID(id);

        for (auto& child : root->children())
            child->setID(id);
    }

    if (m_shader) {
        root->material().setShader(m_shader);
        for (auto& c : root->children())
            c->material().setShader(m_shader);
    }

    if (m_hasMaterial) {
        root->setMaterial(*m_materialPtr);
        for (auto& c : root->children())
            c->setMaterial(*m_materialPtr);
    }

    if (m_texture) {
        root->material().setTexture(m_texture);
        for (auto& c : root->children())
            c->material().setTexture(m_texture);
    }

    root->setColor(m_color);

    auto t = std::make_shared<CompositeTransform>();
    t->add(std::make_shared<Translate>(m_pos.x, m_pos.y, m_pos.z));
    t->add(std::make_shared<Rotate>(m_rot.y, glm::vec3(0, 1, 0)));
    t->add(std::make_shared<Scale>(m_scale.x, m_scale.y, m_scale.z));

    root->setTransform(t);

    for (auto& c : root->children())
        c->setTransform(t);

    if (m_hasProjection)
        root->setCustomProjection(m_projection);

    return root;
}

std::shared_ptr<Model> SceneModelBuilder::add() {
    auto obj = build();
    m_mgr.scene().addModel(obj, !m_assignID);
    return obj;
}
