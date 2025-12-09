#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

struct CameraState {
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 position;
    glm::vec3 forward;
};
