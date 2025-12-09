#include "Input.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


int Input::pickedPixelX = 0;
int Input::pickedPixelY = 0;
bool Input::requestPickEvent = false;
bool Input::requestDeleteEvent = false;


void Input::update(Window& window, Camera& camera,ShadingMode& shading,SceneType& sceneType, bool& bugMode,bool& flashlight, float dt)
{
    GLFWwindow* win = window.handle();
    const float speed = 3.0f * dt;
    static double lastX = 0.0;
    static double lastY = 0.0;
    static bool firstMouse = true;

    glm::vec3 forward{
        cos(glm::radians(camera.yaw())) * cos(glm::radians(camera.pitch())),
        0,
        sin(glm::radians(camera.yaw())) * cos(glm::radians(camera.pitch()))
    };
    glm::vec3 right = glm::normalize(glm::cross(forward, { 0,1,0 }));

    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) camera.move(forward * speed);
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) camera.move(-forward * speed);
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) camera.move(-right * speed);
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) camera.move(right * speed);

    if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.move(glm::vec3(0, 1, 0) * speed);

    if (glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS ||
        glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.move(glm::vec3(0, -1, 0) * speed);

    if (glfwGetKey(win, GLFW_KEY_1) == GLFW_PRESS) shading = ShadingMode::CONSTANT;
    if (glfwGetKey(win, GLFW_KEY_2) == GLFW_PRESS) shading = ShadingMode::LAMBERT;
    if (glfwGetKey(win, GLFW_KEY_3) == GLFW_PRESS) shading = ShadingMode::PHONG;
    if (glfwGetKey(win, GLFW_KEY_4) == GLFW_PRESS) shading = ShadingMode::BLINN;

    if (glfwGetKey(win, GLFW_KEY_F1) == GLFW_PRESS) sceneType = SceneType::ARCADE;
    if (glfwGetKey(win, GLFW_KEY_F2) == GLFW_PRESS) sceneType = SceneType::FORMULA_SPLINE;
    if (glfwGetKey(win, GLFW_KEY_F3) == GLFW_PRESS) sceneType = SceneType::FOREST;
    if (glfwGetKey(win, GLFW_KEY_F4) == GLFW_PRESS) sceneType = SceneType::SOLAR_SYSTEM;
    if (glfwGetKey(win, GLFW_KEY_F9) == GLFW_PRESS) sceneType = SceneType::LIGHT_TEST;

    static bool fPressed = false;
    if (glfwGetKey(win, GLFW_KEY_F) == GLFW_PRESS && !fPressed) {
        fPressed = true;
        flashlight = !flashlight;
    }
    if (glfwGetKey(win, GLFW_KEY_F) == GLFW_RELEASE) {
        fPressed = false;
    }

    if (glfwGetKey(win, GLFW_KEY_F6) == GLFW_PRESS) camera.setFOV(45.0f);
    if (glfwGetKey(win, GLFW_KEY_F7) == GLFW_PRESS) camera.setFOV(90.0f);
    if (glfwGetKey(win, GLFW_KEY_F8) == GLFW_PRESS) camera.setFOV(130.0f);


    static bool bugPressed = false;
    if (glfwGetKey(win, GLFW_KEY_F5) == GLFW_PRESS && !bugPressed) {
        bugPressed = true;
        bugMode = !bugMode;
    }
    if (glfwGetKey(win, GLFW_KEY_F5) == GLFW_RELEASE) {
        bugPressed = false;
    }

    if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(win, &xpos, &ypos);

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = static_cast<float>(xpos - lastX);
        float yoffset = static_cast<float>(lastY - ypos);
        lastX = xpos;
        lastY = ypos;

        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        camera.look(xoffset, yoffset);
    }
    else {
        firstMouse = true;
    }


    static bool leftPressed = false;

    if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !leftPressed)
    {
        leftPressed = true;

        double xpos, ypos;
        glfwGetCursorPos(win, &xpos, &ypos);

        pickedPixelX = (int)xpos;
        pickedPixelY = (int)ypos;

        requestPickEvent = true;
    }

    if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        leftPressed = false;
    }


    static bool delPressed = false;
    if (glfwGetKey(win, GLFW_KEY_DELETE) == GLFW_PRESS && !delPressed)
    {
        delPressed = true;
        requestDeleteEvent = true;
    }
    if (glfwGetKey(win, GLFW_KEY_DELETE) == GLFW_RELEASE)
    {
        delPressed = false;
    }
}
