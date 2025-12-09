#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "../Camera/Camera.h"
#include "../light/Light.h"
#include "../light/ShadingMode.h"
#include "../Renderer/Renderer.h"
#include "../Scene/Manager/SceneManager.h"
#include "../forest/ForestGenerator.h"

class Window;
class ShaderProgram;
class Shader;
class Scene;
class SolarSystemScene;
class ArcadeScene;
class SceneManager;
class FormulaScene;

enum class SceneType {
    TEST_TRIANGLE,
    LIGHT_TEST,
    FOREST,
    SOLAR_SYSTEM,
    ARCADE,
    FORMULA_SPLINE
};

struct PickResult {
    unsigned int id = 0;
    glm::vec3 worldPos;
    float depth = 1.0f;
};

class Application {
public:
    Application(int width, int height);
    ~Application();

    void run();
    void changeScene();
    PickResult pickAt(int mouseX, int mouseY);

private:
    std::unique_ptr<Window> m_window;

    std::shared_ptr<ShaderProgram> m_shaderProgram;
    std::shared_ptr<Shader> m_shader;

    std::unique_ptr<Scene> m_scene;
    SceneManager m_sceneManager;
    std::unique_ptr<Camera> m_camera;
    Renderer m_renderer;

    bool m_flashlightEnabled = false;
    std::vector<Light> m_lights;
    ShadingMode m_shading = ShadingMode::BLINN;
    SceneType m_sceneType = SceneType::FOREST;
    std::unique_ptr<SolarSystemScene> m_solarSystemScene;
    std::unique_ptr<ArcadeScene> m_arcadeScene;
    std::unique_ptr<FormulaScene> m_formulaScene;
    std::unique_ptr<ForestGenerator> m_forestScene;


    std::shared_ptr<Shader> m_phongShader;
    std::shared_ptr<Shader> m_blinnShader;
    std::shared_ptr<Shader> m_lambertShader;
    std::shared_ptr<Shader> m_constantShader;
};
