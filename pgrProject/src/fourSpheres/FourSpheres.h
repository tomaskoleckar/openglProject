#pragma once
#include "../Scene/Scene.h"

class FourSpheresScene {
public:
    FourSpheresScene(Scene& scene, SceneManager& mgr) : m_scene(scene), m_mgr(mgr) {}
    void generate();

private:
    Scene& m_scene;
    SceneManager& m_mgr;
};
