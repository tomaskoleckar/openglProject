#include "Shader.h"
#include "ShaderProgram.h"
#include "../Camera/Camera.h"
#include "../light/Light.h"

Shader::Shader(std::shared_ptr<ShaderProgram> program)
	: m_program(program)
{
}

void Shader::use() const {
	m_program->use();
}

void Shader::setCamera(const Camera& cam) {
	m_program->setMat4("uView", cam.getView());
	m_program->setMat4("uProj", cam.getProj());
	m_program->setVec3("uViewPos", cam.position());
}

void Shader::setModelMatrix(const glm::mat4& m) {
	m_program->setMat4("uModel", m);
}

void Shader::setSelectedId(int id) {
	m_program->setInt("uSelectedID", id);
}

void Shader::setShadingMode(ShadingMode mode) {
	m_program->setInt("uShadingMode", (int)mode);
}

void Shader::setBugMode(bool enabled) {
	m_program->setInt("uBugMode", enabled ? 1 : 0);
}

void Shader::setLights(const std::vector<Light>& lights) {

	m_program->setInt("uLightCount", (int)lights.size());

	for (int i = 0; i < lights.size(); i++) {

		const auto& L = lights[i];

		std::string base = "uLights[" + std::to_string(i) + "]";

		m_program->setInt(base + ".type", (int)L.type);
		m_program->setVec3(base + ".pos", L.position);
		m_program->setVec3(base + ".dir", L.direction);
		m_program->setVec3(base + ".color", L.color);

		m_program->setFloat(base + ".intensity", L.intensity);
		m_program->setFloat(base + ".kc", L.kc);
		m_program->setFloat(base + ".kl", L.kl);
		m_program->setFloat(base + ".kq", L.kq);

		m_program->setFloat(base + ".cutOff", L.cutOff);
		m_program->setFloat(base + ".outerCutOff", L.outerCutOff);
	}
}


void Shader::setCameraMatrices(const glm::mat4& view,
	const glm::mat4& proj,
	const glm::vec3& viewPos)
{
	m_program->setMat4("uView", view);
	m_program->setMat4("uProj", proj);
	m_program->setVec3("uViewPos", viewPos);
}