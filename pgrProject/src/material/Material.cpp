#include "Material.h"
#include "../ShaderProgram/Shader.h"
#include "../ShaderProgram/ShaderProgram.h"
#include <GL/glew.h>

void Material::setShader(std::shared_ptr<Shader> shader)
{
    m_shader = shader;
}

std::shared_ptr<Shader> Material::shader() const
{
    return m_shader;
}

void Material::setTexture(std::shared_ptr<Texture> tex)
{
    m_texture = tex;
}

void Material::upload(Shader& shader) const
{
    ShaderProgram* prog = shader.program();

    prog->setVec3("material.ra", ra);
    prog->setVec3("material.rd", rd);
    prog->setVec3("material.rs", rs);
    prog->setFloat("material.h", h);

    if (m_texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture->id());
        prog->setInt("uHasTexture", 1);
        prog->setInt("uTexture", 0);
    }
    else {
        prog->setInt("uHasTexture", 0);
    }
}
