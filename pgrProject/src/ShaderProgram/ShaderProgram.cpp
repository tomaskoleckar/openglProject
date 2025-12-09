#include "ShaderProgram.h"
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <stdexcept>

static void checkCompile(GLuint shader, const std::string& type) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, log);
        throw std::runtime_error(type + " shader compile error:\n" + log);
    }
}

ShaderProgram::ShaderProgram(const std::string& vs, const std::string& fs)
{
    std::string vsrc = loadFile(vs);
    std::string fsrc = loadFile(fs);

    GLuint vsId = compileShader(GL_VERTEX_SHADER, vsrc);
    GLuint fsId = compileShader(GL_FRAGMENT_SHADER, fsrc);

    m_id = glCreateProgram();
    glAttachShader(m_id, vsId);
    glAttachShader(m_id, fsId);
    glLinkProgram(m_id);

    glDeleteShader(vsId);
    glDeleteShader(fsId);
}

ShaderProgram::~ShaderProgram()
{
    if (m_id)
        glDeleteProgram(m_id);
}

void ShaderProgram::use() const {
    glUseProgram(m_id);
}

void ShaderProgram::unuse() const {
    glUseProgram(0);
}

void ShaderProgram::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& v) const {
    glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &v[0]);
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& m) const {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &m[0][0]);
}

std::string ShaderProgram::loadFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("Cannot open shader file: " + path);

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

unsigned int ShaderProgram::compileShader(unsigned int type, const std::string& src)
{
    GLuint shader = glCreateShader(type);
    const char* cSrc = src.c_str();
    glShaderSource(shader, 1, &cSrc, nullptr);
    glCompileShader(shader);
    checkCompile(shader, (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT"));
    return shader;
}
