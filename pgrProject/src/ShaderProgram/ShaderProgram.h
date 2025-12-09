#pragma once
#include <string>
#include <glm/glm.hpp>

class ShaderProgram {
public:
    ShaderProgram(const std::string& vsPath, const std::string& fsPath);
    ~ShaderProgram();

    void use() const;
    void unuse() const;

    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& v) const;
    void setMat4(const std::string& name, const glm::mat4& m) const;

private:
    unsigned int m_id = 0;

    static std::string loadFile(const std::string& path);
    static unsigned int compileShader(unsigned int type, const std::string& source);
};
