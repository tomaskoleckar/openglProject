#pragma once

#include "../Scene/Scene.h"
#include "../Model/Model.h"
#include "../Model/Mesh.h"
#include "../../texture/Texture.h"

class SolarSystemScene {
public:
    explicit SolarSystemScene(Scene& scene, SceneManager& mgr)
        : m_scene(scene), m_mgr(mgr)
    {
        generate();
    }

    struct PlanetDef {
        std::string name;
        float radius;
        float orbitRadius;
        float orbitSpeed;
        float selfRotSpeed;
        float tiltDeg;
        std::string texturePath;
    };

    struct PlanetInstance {
        PlanetDef def;
        std::shared_ptr<Model> model;
    };

    void generate();
    void update(float time);

    std::shared_ptr<Model> getSunModel() const { return m_sun; }

private:
    Scene& m_scene;
    SceneManager& m_mgr;

    std::shared_ptr<Mesh> m_sphereMesh;

    std::shared_ptr<Model> m_sun;
    std::shared_ptr<Model> m_login;
    std::shared_ptr<Model> m_moon;

    std::vector<PlanetInstance> m_planets;

    std::shared_ptr<Texture> m_moonTex;
};
