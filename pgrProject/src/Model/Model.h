#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "../material/Material.h"
#include "../transform/Transform.h"
#include "../../texture/Texture.h"
#include "Mesh.h"
#include "../transform/Translate.h"

class Texture;

class Model {
public:
    explicit Model(std::shared_ptr<Mesh> mesh);

    void setTransform(TransformPtr transform);
    glm::mat4 transform() const;
    glm::vec3 worldPosition() const;

    void setIsSun(bool v) { m_isSun = v; }
    bool isSun() const { return m_isSun; }

    void setIsFirefly(bool v) { m_isFirefly = v; }
    bool isFirefly() const { return m_isFirefly; }

    Material& material() { return m_material; }
    const Material& material() const { return m_material; }
    void setMaterial(const Material& m) { m_material = m; }

    void setTexture(std::shared_ptr<Texture> tex) { m_material.setTexture(tex);}

    std::shared_ptr<Mesh> mesh() const { return m_mesh; }

    void setColor(const glm::vec3& c) { m_color = c; }
    const glm::vec3& color() const { return m_color; }

    unsigned int id() const { return m_id; }
    void setID(unsigned int id) { m_id = id; }

    void addChild(std::shared_ptr<Model> child);
    const std::vector<std::shared_ptr<Model>>& children() const;

    void setCustomProjection(const glm::mat4& m) {
        m_customProj = m;
        m_useCustomProj = true;
    }
    bool usesCustomProjection() const { return m_useCustomProj; }
    glm::mat4 customProjection() const { return m_customProj; }

    void setModelPosition(const glm::vec3& position);
    void setPositionTransform(const std::shared_ptr<Translate>& t) { m_positionTransform = t; }

private:
    unsigned int m_id = 0;

    bool m_isSun = false;
    bool m_isFirefly = false;

    glm::vec3 m_color{ 1,1,1 };

    std::shared_ptr<Mesh> m_mesh;

    Material m_material;
    TransformPtr m_transform;

    std::vector<std::shared_ptr<Model>> m_children;

    glm::mat4 m_customProj = glm::mat4(1.0f);
    bool m_useCustomProj = false;

    std::shared_ptr<Translate> m_positionTransform;
};
