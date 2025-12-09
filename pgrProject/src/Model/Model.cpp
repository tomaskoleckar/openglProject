#include "Model.h"

Model::Model(std::shared_ptr<Mesh> mesh)
    : m_mesh(std::move(mesh))
{
}

void Model::setTransform(TransformPtr t)
{
    m_transform = std::move(t);
}

glm::mat4 Model::transform() const
{
    return m_transform ? m_transform->getMatrix() : glm::mat4(1.0f);
}

glm::vec3 Model::worldPosition() const
{
    glm::mat4 m = transform();
    return glm::vec3(m[3]);
}

void Model::addChild(std::shared_ptr<Model> child)
{
    m_children.push_back(std::move(child));
}

const std::vector<std::shared_ptr<Model>>& Model::children() const
{
    return m_children;
}

void Model::setModelPosition(const glm::vec3& position)
{
    if (m_positionTransform)
    {
        m_positionTransform->setPosition(position);
    }
}
