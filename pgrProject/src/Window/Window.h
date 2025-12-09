#pragma once
#include <string>
#include <glm/glm.hpp>

struct GLFWwindow;

class Window
{
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool shouldClose() const;
    void pollEvents();
    void swapBuffers();

    glm::ivec2 getResolution() const { return { m_width, m_height }; }

    void updateSize(int w, int h);

    GLFWwindow* handle() const { return m_handle; }

    int width() const { return m_width; }
    int height() const { return m_height; }

private:
    void initGLFW(const std::string& title);
    void initGLEW();

    GLFWwindow* m_handle = nullptr;
    int m_width = 0;
    int m_height = 0;
};
