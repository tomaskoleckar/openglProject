#include "TestTriangle.h"
#include <glm/glm.hpp>
#include "../transform/CompositeTransform.h"
#include "../transform/Translate.h"

void TestTriangleScene::generate()
{
    std::vector<float> verts = {
        -0.5f, -0.5f, 0.0f,       0,0,1,
         0.5f, -0.5f, 0.0f,       0,0,1,
         0.0f,  0.5f, 0.0f,       0,0,1
    };

    auto mesh = std::make_shared<Mesh>(verts, true);
    auto model = std::make_shared<Model>(mesh);

    auto transform = std::make_shared<CompositeTransform>();
    transform->add(std::make_shared<Translate>(0.0f, 0.0f, -3.0f));
    model->setTransform(transform);

    model->setColor({ 1.0f, 0.2f, 0.2f });

    m_scene.addModel(model);
}
