#include "../Scene/Scene.h"

class TestTriangleScene {
public:
    TestTriangleScene(Scene& scene) : m_scene(scene) {}
    void generate();

private:
    Scene& m_scene;
};
