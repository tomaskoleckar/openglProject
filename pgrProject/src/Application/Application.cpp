#include "Application.h"
#include "../Window/Window.h"
#include "../Scene/Scene.h"
#include "../ShaderProgram/Shader.h"
#include "../ShaderProgram/ShaderProgram.h"
#include "../input/Input.h"
#include "../SolarSystem/SolarSystemScene.h"
#include "../ArcadeScene/ArcadeScene.h"
#include "../forest/ForestGenerator.h"
#include "../testTriangle/TestTriangle.h"
#include "../fourSpheres/FourSpheres.h"
#include "../thirdParty/ObjLoader.h"
#include "../Formula/FormulaScene.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../transform/Translate.h"
#include "../transform/CompositeTransform.h"
#include "../transform/Scale.h"

Application::Application(int width, int height)
{
	m_window = std::make_unique<Window>(width, height, "PGRProject");

	m_shaderProgram = std::make_shared<ShaderProgram>(
		"shaders/basic.vert",
		"shaders/basic.frag"
	);

	m_shader = std::make_shared<Shader>(m_shaderProgram);

	m_camera = std::make_unique<Camera>(60.0f, (float)width / height, 0.1f, 200.0f);
	m_camera->setPosition({ 0.0f, 2.0f, 8.0f });

	m_renderer.setShader(m_shader);

	m_sceneType = SceneType::FOREST;
	changeScene();
}

Application::~Application() = default;

void Application::run()
{
	float lastTime = (float)glfwGetTime();
	SceneType lastScene = m_sceneType;

	bool bugMode = false;
	bool flashlight = false;

	while (!m_window->shouldClose())
	{
		float now = (float)glfwGetTime();
		float dt = now - lastTime;
		lastTime = now;

		m_window->pollEvents();
		Input::update(*m_window, *m_camera, m_shading, m_sceneType, bugMode, flashlight, dt);

		if (m_sceneType == SceneType::ARCADE && m_arcadeScene)
			m_arcadeScene->update(dt);

		if (m_sceneType == SceneType::FORMULA_SPLINE && m_formulaScene)
			m_formulaScene->update(dt);

		if (Input::requestPickEvent)
		{
			auto result = pickAt(Input::pickedPixelX, Input::pickedPixelY);

			if (m_sceneType == SceneType::ARCADE && m_arcadeScene) {
				if (result.id > 0)
					m_arcadeScene->onHit(result.id);
			}
			else if (m_sceneType == SceneType::FORMULA_SPLINE && m_formulaScene) {
				if (result.id == 0 && result.depth < 0.999f) {
					m_formulaScene->onClickPoint(result.worldPos);
				}
			}
			else {
				if (result.id > 0) {
					m_scene->select(result.id);
				}
				else if (result.depth < 0.999f) {
					m_scene->spawnTreeAt(result.worldPos, m_sceneManager);
				}
			}

			Input::requestPickEvent = false;
		}

		if (Input::requestDeleteEvent)
		{
			if (auto* sel = m_scene->getSelected()) {
				printf("Removed ID %u\n", sel->id());
				m_scene->removeSelected();
			}
			Input::requestDeleteEvent = false;
		}

		if (lastScene != m_sceneType) {
			changeScene();
			lastScene = m_sceneType;
		}

		m_renderer.beginFrame({ 0.00f, 0.00f, 0.00f, 1.0f });
		m_renderer.setCamera(*m_camera);

		int selId = -1;
		if (auto* sel = m_scene->getSelected())
			selId = sel->id();
		m_renderer.setSelectedId(selId);

		m_renderer.setShadingMode(m_shading);
		m_renderer.setBugMode(bugMode);

		std::vector<Light> lights;

		if (m_sceneType == SceneType::SOLAR_SYSTEM && m_solarSystemScene)
		{
			m_solarSystemScene->update(now);
			lights.push_back(Light::Point(
				m_solarSystemScene->getSunModel()->worldPosition(),
				{ 1.0f, 0.9f, 0.4f },
				100.0f
			));
		}
		else {
			lights = m_lights;
		}

		if (m_sceneType == SceneType::FOREST && m_forestScene)
		{
			m_forestScene->UpdateFireflies(now);

			lights.clear();
			lights.push_back(Light::Ambient(glm::vec3(0.05f), 1.0f));

			for (const auto& f : m_forestScene->getFireflies())
				lights.push_back(f);
		}

		if (flashlight) {
			lights.push_back(
				Light::Spot(
					m_camera->position(),
					m_camera->forward(),
					{ 1.0f, 1.0f, 0.9f },
					8.0f
				)
			);
		}

		m_renderer.setLights(lights);

		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		m_renderer.renderScene(*m_scene);

		glDisable(GL_STENCIL_TEST);

		m_window->swapBuffers();
	}
}

void Application::changeScene()
{
	m_scene = std::make_unique<Scene>();
	m_sceneManager = SceneManager(*m_scene);
	m_lights.clear();

	if (m_sceneType == SceneType::TEST_TRIANGLE) {
		TestTriangleScene gen(*m_scene);
		gen.generate();

		m_lights.push_back(Light::Point(
			glm::vec3(0.0f, 10.0f, 10.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			1.0f
		));
	}
	else if (m_sceneType == SceneType::LIGHT_TEST) {
		FourSpheresScene gen(*m_scene,m_sceneManager);
		gen.generate();

		auto t = std::make_shared<CompositeTransform>();
		t->add(std::make_shared<Translate>(0.0f, -0.5f, 0.0f));
		t->add(std::make_shared<Scale>(1.5f));

		m_camera->setPosition({ 0.0f, 0.0f, 8.0f });
		m_camera->setYawPitch(-90.0f, 0.0f);

		m_lights.push_back(Light::Point(
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			3.0f
		));
	}
	else if (m_sceneType == SceneType::FOREST) {
		m_forestScene = std::make_unique<ForestGenerator>(*m_scene, m_sceneManager);

		auto fireflies = m_forestScene->CreateFireFlies();

		m_renderer.setCamera(*m_camera);

		m_lights.clear();
		m_lights.push_back(Light::Ambient(glm::vec3(0.05f), 1.0f));
		for (auto& f : fireflies)
			m_lights.push_back(f);

		m_forestScene->generate(50, 50);
	}
	else if (m_sceneType == SceneType::SOLAR_SYSTEM) {
		m_solarSystemScene = std::make_unique<SolarSystemScene>(*m_scene,m_sceneManager);

		m_camera->setPosition({ 0.0f, 5.0f, 20.0f });
		m_camera->setYawPitch(-90.0f, -10.0f);

		m_lights.clear();
	}
	else if (m_sceneType == SceneType::ARCADE) {
		m_arcadeScene = std::make_unique<ArcadeScene>(*m_scene, m_sceneManager);

		m_camera->setPosition({ 0.0f, 20.0f, 00.0f });
		m_camera->setYawPitch(-90.0f, 0.0f);

		m_lights.clear();
	}
	else if (m_sceneType == SceneType::FORMULA_SPLINE) {
		m_formulaScene = std::make_unique<FormulaScene>(*m_scene, m_sceneManager);

		m_lights = m_formulaScene->getLights();

		m_camera->setPosition({ 0.0f, 5.0f, 5.0f });
		m_camera->setYawPitch(-90.0f, 0.0f);
	}
}

PickResult Application::pickAt(int mouseX, int mouseY)
{
	PickResult result;

	glm::ivec2 res = m_window->getResolution();
	int winWidth = res.x;
	int winHeight = res.y;

	int newY = winHeight - mouseY;

	GLubyte color[4];
	GLfloat depth;
	GLuint stencilID = 0;

	glReadPixels(mouseX, newY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);

	glReadPixels(mouseX, newY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

	glReadPixels(mouseX, newY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &stencilID);

	result.id = stencilID;
	result.depth = depth;

	glm::vec3 screen(mouseX, newY, depth);
	glm::vec3 pos = glm::unProject(
		screen,
		m_camera->getView(),
		m_camera->getProj(),
		glm::vec4(0, 0, winWidth, winHeight)
	);

	result.worldPos = pos;

	printf("\n[Pick] Mouse (%d,%d) -> ID=%u  depth=%f  world=[%.3f, %.3f, %.3f]\n",
		mouseX, mouseY,
		stencilID,
		depth,
		pos.x, pos.y, pos.z
	);

	return result;
}
