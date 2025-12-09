#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <memory>

class Mesh
{
public:
    Mesh(const std::vector<float>& interleavedData, bool hasNormals = false, bool hasTexCoords = false, const std::vector<unsigned int>& indices = {});
    ~Mesh();

    static std::shared_ptr<Mesh> makeQuad();
    void draw() const;

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;
    bool m_hasIndices = false;
    bool m_hasTexCoords = false;
    GLsizei m_indexCount = 0;
    GLsizei m_vertexCount = 0;
};
