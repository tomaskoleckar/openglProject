#pragma once
#include "../Manager/SceneManager.h"
#include "../../Application/Application.h"

class SceneGenerator {
public:
    SceneGenerator(Scene& scene, SceneManager& manager)
        : m_scene(scene), m_mgr(manager) {
    }

    virtual ~SceneGenerator() = default;

    virtual void generate() = 0;

protected:
    Scene& m_scene;
    SceneManager& m_mgr;
};
