#include "Mesh.h"

Mesh::Mesh(const std::vector<float>& data, bool hasNormals, bool hasTexCoords, const std::vector<unsigned int>& indices)
{
    int stride = 3;
    if (hasNormals) stride += 3;
    if (hasTexCoords) stride += 2;

    m_vertexCount = data.size() / stride;
    m_hasIndices = !indices.empty();
    m_indexCount = static_cast<GLsizei>(indices.size());
    m_hasTexCoords = hasTexCoords;

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);

    if (hasNormals) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
    }

    if (hasTexCoords) {
        size_t offset = (hasNormals ? 6 : 3) * sizeof(float);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)offset);
    }

    if (m_hasIndices) {
        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_vbo);
    if (m_hasIndices) glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
}

void Mesh::draw() const
{
    glBindVertexArray(m_vao);
    if (m_hasIndices)
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    glBindVertexArray(0);
}

std::shared_ptr<Mesh> Mesh::makeQuad()
{
    float repeat = 10.0f;

    std::vector<float> verts = {
        -1.0f, 0.0f, -1.0f,   0, 1, 0,   0.0f,      0.0f,
         1.0f, 0.0f, -1.0f,   0, 1, 0,   repeat,    0.0f,
         1.0f, 0.0f,  1.0f,   0, 1, 0,   repeat,    repeat,

        -1.0f, 0.0f, -1.0f,   0, 1, 0,   0.0f,      0.0f,
         1.0f, 0.0f,  1.0f,   0, 1, 0,   repeat,    repeat,
        -1.0f, 0.0f,  1.0f,   0, 1, 0,   0.0f,      repeat
    };

    return std::make_shared<Mesh>(verts, true, true);
}
