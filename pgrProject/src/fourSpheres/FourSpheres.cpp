#include "FourSpheres.h"
#include "Sphere.h"
#include "../transform/CompositeTransform.h"
#include "../transform/Translate.h"
#include "../transform/Scale.h"

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

    return std::make_shared<Mesh>(data, true);
}

void FourSpheresScene::generate()
{
    auto sphereMesh = makeSphereMesh();

    glm::vec3 positions[] = {
        { -2.0f,  0.0f, 0.0f },
        {  2.0f,  0.0f, 0.0f },
        { 0.0f, -2.0f, 0.0f },
        {  0.0f, 2.0f, 0.0f }
    };


    for (int i = 0; i < 4; ++i) {
        auto model = std::make_shared<Model>(sphereMesh);

        auto transform = std::make_shared<CompositeTransform>();
        transform->add(std::make_shared<Translate>(
            positions[i].x, positions[i].y, positions[i].z
        ));

        transform->add(std::make_shared<Scale>(1.0f));

        model->setTransform(transform);

        model->setColor({ 0.8f, 0.8f, 0.8f });
        m_scene.addModel(model);
    }
}
