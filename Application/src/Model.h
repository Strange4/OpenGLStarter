#pragma once
#include "pch.h"

#include <optional>
#include <assimp/scene.h>
#include "Mesh.h"

class Model
{
public:
    Model(std::string directory_path);

    void draw(ShaderProgram& shader_program) const;

private:
    std::vector <std::unique_ptr<Mesh>> m_meshes;
    std::vector<Texture> m_loaded_textures;

    void loadModel(std::string directory_path);
    void processNode(aiNode* node, const aiScene* scene);
    std::unique_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);

    std::optional<unsigned int> getLoadedTextureIndex(const std::string& file_path) const;
    std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type);
};