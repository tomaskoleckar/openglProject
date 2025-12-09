#pragma once
#include "../Model/Mesh.h"
#include <memory>

class SkyboxMesh {
public:
    static std::shared_ptr<Mesh> create()
    {
        std::vector<float> vertices = {
            -1,  1, -1,   -1, -1, -1,    1, -1, -1,
             1, -1, -1,    1,  1, -1,   -1,  1, -1,

            -1, -1,  1,   -1, -1, -1,   -1,  1, -1,
            -1,  1, -1,   -1,  1,  1,   -1, -1,  1,

             1, -1, -1,    1, -1,  1,    1,  1,  1,
             1,  1,  1,    1,  1, -1,    1, -1, -1,

            -1, -1,  1,   -1,  1,  1,    1,  1,  1,
             1,  1,  1,    1, -1,  1,   -1, -1,  1,

            -1,  1, -1,    1,  1, -1,    1,  1,  1,
             1,  1,  1,   -1,  1,  1,   -1,  1, -1,

            -1, -1, -1,   -1, -1,  1,    1, -1, -1,
             1, -1, -1,   -1, -1,  1,    1, -1,  1
        };

        return std::make_shared<Mesh>(vertices, false, false);
    }
};
