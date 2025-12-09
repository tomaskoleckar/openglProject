#include "Window.h"
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include <stdexcept>

static void glfwErrorCallback(int code, const char* desc) {
	fprintf(stderr, "[GLFW] Error %d: %s\n", code, desc);
}

Window::Window(int width, int height, const std::string& title) : m_width(width), m_height(height)
{
	initGLFW(title);
	initGLEW();

	glViewport(0, 0, m_width, m_height);
	glEnable(GL_DEPTH_TEST);
}

Window::~Window()
{
	if (m_handle) {
		glfwDestroyWindow(m_handle);
		m_handle = nullptr;
	}
	glfwTerminate();
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(m_handle);
}

void Window::pollEvents()
{
	glfwPollEvents();
}

void Window::swapBuffers()
{
	glfwSwapBuffers(m_handle);
}

static void framebuffer_callback(GLFWwindow* win, int w, int h)
{
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
	if (window)
		window->updateSize(w, h);
}

void Window::initGLFW(const std::string& title)
{
	glfwSetErrorCallback(glfwErrorCallback);
	if (!glfwInit()) throw std::runtime_error("Failed to init GLFW");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);

	m_handle = glfwCreateWindow(m_width, m_height, title.c_str(), nullptr, nullptr);
	if (!m_handle) throw std::runtime_error("Failed to create GLFW window");

	glfwMakeContextCurrent(m_handle);

	glfwSetWindowUserPointer(m_handle, this);

	glfwSetFramebufferSizeCallback(m_handle, framebuffer_callback);

	glfwSwapInterval(1);
}

void Window::initGLEW()
{
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		throw std::runtime_error((const char*)glewGetErrorString(err));
	}
}

void Window::updateSize(int w, int h)
{
	m_width = w;
	m_height = h;
	glViewport(0, 0, w, h);
}