#pragma once
#include <glm/glm.hpp>

enum class LightType {
    AMBIENT = 0,
    DIRECTIONAL = 1,
    POINT = 2,
    SPOT = 3
};

class Light {
public:
    LightType type = LightType::POINT;

    glm::vec3 position{ 0.0f };
    glm::vec3 direction{ 0.0f, -1.0f, 0.0f };

    glm::vec3 color{ 1.0f };
    float intensity = 1.0f;

    float kc = 1.0f;
    float kl = 0.09f;
    float kq = 0.032f;

    float cutOff = glm::cos(glm::radians(12.5f));
    float outerCutOff = glm::cos(glm::radians(17.5f));

    Light() = default;

    static Light Ambient(glm::vec3 col, float intensity) {
        Light L;
        L.type = LightType::AMBIENT;
        L.color = col;
        L.intensity = intensity;
        return L;
    }

    static Light Directional(glm::vec3 dir, glm::vec3 col, float intensity) {
        Light L;
        L.type = LightType::DIRECTIONAL;
        L.direction = glm::normalize(dir);
        L.color = col;
        L.intensity = intensity;
        return L;
    }

    static Light Point(glm::vec3 pos, glm::vec3 col, float intensity) {
        Light L;
        L.type = LightType::POINT;
        L.position = pos;
        L.color = col;
        L.intensity = intensity;
        return L;
    }

    static Light Spot(glm::vec3 pos, glm::vec3 dir, glm::vec3 col, float intensity) {
        Light L;
        L.type = LightType::SPOT;
        L.position = pos;
        L.direction = glm::normalize(dir);
        L.color = col;
        L.intensity = intensity;
        return L;
    }
};
