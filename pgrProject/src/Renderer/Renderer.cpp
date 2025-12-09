#include "Renderer.h"
#include <GL/glew.h>

#include "../ShaderProgram/Shader.h"
#include "../ShaderProgram/ShaderProgram.h"
#include "../Camera/Camera.h"
#include "../Scene/Scene.h"
#include "../Model/Model.h"
#include "../light/Light.h"
#include "../material/Material.h"

Renderer::Renderer(std::shared_ptr<Shader> shader)
    : m_shader(std::move(shader))
{
}

void Renderer::setShader(std::shared_ptr<Shader> shader)
{
    m_shader = std::move(shader);
}

void Renderer::setCamera(const Camera& camera)
{
    m_view = camera.getView();
    m_proj = camera.getProj();
    m_viewPos = camera.position();
}

void Renderer::setShadingMode(ShadingMode mode)
{
    m_shadingMode = mode;
}

void Renderer::setBugMode(bool enabled)
{
    m_bugMode = enabled;
}

void Renderer::setSelectedId(int id)
{
    m_selectedId = id;
}

void Renderer::setLights(const std::vector<Light>& lights)
{
    m_lights = lights;
}

void Renderer::beginFrame(const glm::vec4& clearColor)
{
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::uploadPerFrameUniforms()
{
    if (!m_shader)
        return;

    m_shader->setCameraMatrices(m_view, m_proj, m_viewPos);

    m_shader->setSelectedId(m_selectedId);
    m_shader->setShadingMode(m_shadingMode);
    m_shader->setBugMode(m_bugMode);

    m_shader->setLights(m_lights);
}

void Renderer::applyPerFrameToShader(Shader& shader)
{
    shader.setCameraMatrices(m_view, m_proj, m_viewPos);
    shader.setSelectedId(m_selectedId);
    shader.setShadingMode(m_shadingMode);
    shader.setBugMode(m_bugMode);
    shader.setLights(m_lights);
}

void Renderer::renderSkybox(Scene& scene)
{
    auto skybox = scene.skyboxModel();
    if (!skybox)
        return;

    auto shader = skybox->material().shader();
    auto cubemap = skybox->material().cubeMap();
    if (!shader || !cubemap)
        return;

    GLboolean depthTestEnabled;
    glGetBooleanv(GL_DEPTH_TEST, &depthTestEnabled);

    GLint oldDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunc);

    GLboolean oldDepthMask;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &oldDepthMask);

    GLint oldStencilMask;
    glGetIntegerv(GL_STENCIL_WRITEMASK, &oldStencilMask);

    GLboolean cullFaceEnabled;
    glGetBooleanv(GL_CULL_FACE, &cullFaceEnabled);

    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    glStencilMask(0x00);
    glDisable(GL_CULL_FACE);

    shader->use();

    glm::mat4 viewNoTrans = glm::mat4(glm::mat3(m_view));
    shader->setCameraMatrices(viewNoTrans, m_proj, m_viewPos);
    shader->setModelMatrix(glm::mat4(1.0f));

    shader->program()->setInt("uSkybox", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->id());

    if (auto mesh = skybox->mesh())
        mesh->draw();

    glDepthMask(oldDepthMask);
    glDepthFunc(oldDepthFunc);
    glStencilMask(oldStencilMask);

    if (cullFaceEnabled) glEnable(GL_CULL_FACE);
    else                 glDisable(GL_CULL_FACE);

    if (depthTestEnabled) glEnable(GL_DEPTH_TEST);
    else                  glDisable(GL_DEPTH_TEST);
}




void Renderer::renderScene(Scene& scene)
{
    if (!m_shader)
        return;

    uploadPerFrameUniforms();

    scene.render(*this);
    renderSkybox(scene);
}


void Renderer::renderModel(const Model& model, const glm::mat4& worldMat)
{
    Shader* active = model.material().shader()
        ? model.material().shader().get()
        : m_shader.get();

    active->use();
    applyPerFrameToShader(*active);

    ShaderProgram* prog = active->program();

    glm::mat4 modelMat = worldMat;
    if (model.usesCustomProjection()) {
        modelMat = model.customProjection() * modelMat;
    }
    active->setModelMatrix(modelMat);

    prog->setInt("uIsSun", model.isSun() ? 1 : 0);
    prog->setInt("uIsFirefly", model.isFirefly() ? 1 : 0);
    prog->setVec3("uColor", model.color());
    prog->setInt("uObjectID", model.id());

    glStencilFunc(GL_ALWAYS, model.id(), 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    model.material().upload(*active);

    if (auto mesh = model.mesh())
        mesh->draw();
}



