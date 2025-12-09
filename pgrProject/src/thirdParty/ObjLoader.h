#pragma once
#include <memory>
#include <string>
#include <vector>
#include "../Model/Mesh.h"

struct MeshWithTexture {
    std::shared_ptr<Mesh> mesh;
    std::string texturePath;
};

std::vector<MeshWithTexture> loadOBJWithMaterials(const std::string& path, const std::string& baseDir);
