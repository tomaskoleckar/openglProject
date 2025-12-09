#include "ObjLoader.h"
#include "tinyObjectLoader/tiny_obj_loader.h"
#include <glm/glm.hpp>
#include <limits>
#include <stdexcept>


std::vector<MeshWithTexture> loadOBJWithMaterials(const std::string& path, const std::string& baseDir)
{
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig config;
    config.mtl_search_path = baseDir;
    config.triangulate = true;

    if (!reader.ParseFromFile(path, config)) {
        if (!reader.Error().empty())
            throw std::runtime_error("OBJ load error: " + reader.Error());
        throw std::runtime_error("OBJ load failed (unknown error).");
    }

    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();
    const auto& materials = reader.GetMaterials();

    std::vector<MeshWithTexture> result;

    for (const auto& shape : shapes)
    {
        std::vector<float> interleaved;
        interleaved.reserve(shape.mesh.indices.size() * 8);

        for (size_t i = 0; i < shape.mesh.indices.size(); i++)
        {
            tinyobj::index_t idx = shape.mesh.indices[i];
            glm::vec3 pos(
                attrib.vertices[3 * idx.vertex_index + 0],
                attrib.vertices[3 * idx.vertex_index + 1],
                attrib.vertices[3 * idx.vertex_index + 2]);

            glm::vec3 nrm(0, 1, 0);
            if (idx.normal_index >= 0 && idx.normal_index * 3 + 2 < attrib.normals.size()) {
                nrm = glm::vec3(
                    attrib.normals[3 * idx.normal_index + 0],
                    attrib.normals[3 * idx.normal_index + 1],
                    attrib.normals[3 * idx.normal_index + 2]);
            }

            glm::vec2 uv(0.0f);
            if (idx.texcoord_index >= 0 && (size_t)(2 * idx.texcoord_index + 1) < attrib.texcoords.size()) {
                uv = glm::vec2(
                    attrib.texcoords[2 * idx.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]); 
            }

            interleaved.insert(interleaved.end(),
                { pos.x, pos.y, pos.z, nrm.x, nrm.y, nrm.z, uv.x, uv.y });
        }

        int mat_id = (shape.mesh.material_ids.empty() ? -1 : shape.mesh.material_ids[0]);
        std::string texPath;
        if (mat_id >= 0 && mat_id < materials.size()) {
            texPath = materials[mat_id].diffuse_texname;
        }

        auto mesh = std::make_shared<Mesh>(interleaved, true, true, std::vector<unsigned int>());
        result.push_back({ mesh, texPath });
    }

    return result;
}