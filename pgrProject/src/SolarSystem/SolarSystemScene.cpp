#include "SolarSystemScene.h"
#include "../transform/CompositeTransform.h"
#include "../transform/Translate.h"
#include "../transform/Scale.h"
#include "../transform/Rotate.h"
#include "../thirdParty/ObjLoader.h"
#include <glm/gtc/constants.hpp>
#include <cmath>

std::shared_ptr<Mesh> makeUVSphere(int stacks = 64, int slices = 64)
{
    std::vector<float> data;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= stacks; ++i)
    {
        float V = (float)i / stacks;
        float phi = V * glm::pi<float>();

        for (int j = 0; j <= slices; ++j)
        {
            float U = (float)j / slices;
            float theta = U * glm::two_pi<float>();

            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);

            data.push_back(x); data.push_back(y); data.push_back(z);
            data.push_back(x); data.push_back(y); data.push_back(z);
            data.push_back(U); data.push_back(V);
        }
    }

    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < slices; ++j)
        {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    return std::make_shared<Mesh>(data, true, true, indices);
}

void SolarSystemScene::generate()
{
    m_sphereMesh = makeUVSphere(64, 64);

    std::vector<PlanetDef> defs = {
        { "Mercury", 0.38f,  8.0f, 4.15f, 10.0f,   0.0f,   "assets/textures/mercury.jpg" },
        { "Venus",   0.95f,  12.5f, 1.62f,-2.0f,  177.0f,  "assets/textures/venus.jpg" },
        { "Earth",   1.00f,  15.0f, 1.00f,30.0f,  23.5f,   "assets/textures/earth.jpg" },
        { "Mars",    0.53f,  20.0f, 0.53f,24.0f,  25.0f,   "assets/textures/mars.jpg" },
        { "Jupiter", 3.60f, 30.0f,0.08f,45.0f,   3.0f,   "assets/textures/jupiter.jpg" },
        { "Saturn",  2.73f, 40.0f,0.03f,37.0f,  27.0f,   "assets/textures/saturn.jpg" },
        { "Uranus",  2.00f, 50.0f,0.02f,-20.0f, 98.0f,   "assets/textures/uranus.jpg" },
        { "Neptune", 1.90f, 60.0f,0.01f,28.0f,  30.0f,   "assets/textures/neptune.jpg" }
    };

    {
        m_sun = std::make_shared<Model>(m_sphereMesh);

        auto texSun = std::make_shared<Texture>("assets/textures/sun.jpg", true);
        m_sun->setTexture(texSun);
        m_sun->setIsSun(true);

        auto tSun = std::make_shared<CompositeTransform>();
        tSun->add(std::make_shared<Scale>(1.0f));
        m_sun->setTransform(tSun);

        m_scene.addModel(m_sun);

        
        m_login = m_mgr.builder()
            .path("assets/models/login/login.obj")
            .textureDir("assets/models/login/")
            .scale(1.0f)
            .assignID(true)
            .add();

        m_login->setTexture(texSun);

    }

    for (auto& def : defs)
    {
        PlanetInstance inst;
        inst.def = def;
        inst.model = std::make_shared<Model>(m_sphereMesh);

        auto tex = std::make_shared<Texture>(def.texturePath, true);
        inst.model->setTexture(tex);

        auto t = std::make_shared<CompositeTransform>();
        t->add(std::make_shared<Translate>(def.orbitRadius, 0, 0));
        t->add(std::make_shared<Scale>(def.radius));
        inst.model->setTransform(t);

        m_scene.addModel(inst.model);
        m_planets.push_back(inst);
    }

    m_moon = std::make_shared<Model>(m_sphereMesh);
    m_moonTex = std::make_shared<Texture>("assets/textures/moon.jpg", true);
    m_moon->setTexture(m_moonTex);

    auto tm = std::make_shared<CompositeTransform>();
    tm->add(std::make_shared<Translate>(15.0f, 0.0f, 0.0f));
    tm->add(std::make_shared<Scale>(0.27f));
    m_moon->setTransform(tm);

    m_scene.addModel(m_moon);
}

void SolarSystemScene::update(float time)
{
    auto tSun = std::make_shared<CompositeTransform>();
    tSun->add(std::make_shared<Rotate>(time * 5.0f, glm::vec3(0, 1, 0)));
    tSun->add(std::make_shared<Scale>(5.0f));
    m_sun->setTransform(tSun);

    for (auto& p : m_planets)
    {
        auto t = std::make_shared<CompositeTransform>();

        float x = cos(time * p.def.orbitSpeed) * p.def.orbitRadius;
        float z = sin(time * p.def.orbitSpeed) * p.def.orbitRadius;

        t->add(std::make_shared<Translate>(x, 0, z));
        t->add(std::make_shared<Rotate>(p.def.tiltDeg, glm::vec3(0, 0, 1)));
        t->add(std::make_shared<Rotate>(time * p.def.selfRotSpeed, glm::vec3(0, 1, 0)));
        t->add(std::make_shared<Scale>(p.def.radius));

        p.model->setTransform(t);
    }

    PlanetInstance* earth = nullptr;
    for (auto& p : m_planets)
    {
        if (p.def.name == "Earth") {
            earth = &p;
            break;
        }
    }

    if (earth)
    {
        float ex = cos(time * earth->def.orbitSpeed) * earth->def.orbitRadius;
        float ez = sin(time * earth->def.orbitSpeed) * earth->def.orbitRadius;

        float moonDist = 1.5f;
        float moonSpeed = 5.0f;

        float mx = ex + cos(time * moonSpeed) * moonDist;
        float mz = ez + sin(time * moonSpeed) * moonDist;

        auto tm = std::make_shared<CompositeTransform>();
        tm->add(std::make_shared<Translate>(mx, 0, mz));
        tm->add(std::make_shared<Rotate>(time * 10.0f, glm::vec3(0, 1, 0)));
        tm->add(std::make_shared<Scale>(0.27f));

        m_moon->setTransform(tm);
    }

    float orbitRadius = 25.0f;
    float orbitSpeed = 0.5f;

    float x = cos(time * orbitSpeed) * orbitRadius;
    float z = sin(time * orbitSpeed) * orbitRadius;

    auto tl = std::make_shared<CompositeTransform>();

    tl->add(std::make_shared<Translate>(x, 0.0f, z));
    tl->add(std::make_shared<Rotate>(time * 2.0f, glm::vec3(0, 1, 0)));

    m_login->setTransform(tl);
}
