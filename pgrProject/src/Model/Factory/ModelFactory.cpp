#include "ModelFactory.h"
#include "../../texture/Texture.h"
#include "../../thirdParty/ObjLoader.h"

std::shared_ptr<Model> ModelFactory::loadOBJWithShader(
    const std::string& objPath,
    const std::string& baseDir,
    std::shared_ptr<Shader> shader)
{
    auto loaded = loadOBJWithMaterials(objPath, baseDir);

    auto root = std::make_shared<Model>(nullptr);

    for (auto& m : loaded)
    {
        auto part = std::make_shared<Model>(m.mesh);

        if (!m.texturePath.empty()) {
            auto tex = std::make_shared<Texture>(baseDir + m.texturePath, false);
            part->material().setTexture(tex);
        }

        if (shader)
            part->material().setShader(shader);

        root->addChild(part);
    }

    return root;
}
