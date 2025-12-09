#pragma once
#include <string>
#include <GL/glew.h>

class Texture {
public:
    Texture() = default;
    explicit Texture(const std::string& path, bool flip = true);
    ~Texture();

    void bind(GLuint unit = 0) const;
    void unbind() const;

    GLuint id() const { return m_id; }

private:
    GLuint m_id = 0;
};
