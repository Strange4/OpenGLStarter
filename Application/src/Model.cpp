#include "pch.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Model.h"

Model::Model(std::string directory_path)
{
	this->loadModel(directory_path);
}

void Model::draw(ShaderProgram& shader_program) const
{
	for (const std::unique_ptr<Mesh>& mesh : this->m_meshes)
	{
		mesh.get()->draw(shader_program);
	}
}

void Model::loadModel(std::string directory_path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(directory_path, aiProcess_Triangulate
		| aiProcess_FlipUVs | aiProcess_OptimizeMeshes);

	if (!scene || scene->mFlags * AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->m_meshes.push_back(std::move(processMesh(mesh, scene)));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}

std::unique_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	

	// process vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		aiVector3D position = mesh->mVertices[i];
		aiVector3D normal = mesh->mNormals[i];
		glm::vec2 tex_coordinates(0.0f);

		// we only allow 1 texture coodinate
		if (mesh->mTextureCoords[0])
		{
			tex_coordinates.x = mesh->mTextureCoords[0][i].x;
			tex_coordinates.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(
			Vertex{
				glm::vec3{position.x, position.y, position.z}, // position
				glm::vec3{normal.x, normal.y, normal.z}, // normal
				tex_coordinates, // texture coordinates
			}
		);
	}

	// process indinces
	for (unsigned int i = 0; i < mesh->mNumFaces;i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		textures = this->loadMaterialTextures(material, aiTextureType_DIFFUSE);
	}

	return std::make_unique<Mesh>(vertices, indices, textures);
}

std::optional<unsigned int> Model::getLoadedTextureIndex(const std::string& file_path) const
{
	for (unsigned int i = 0; i < this->m_loaded_textures.size(); i++)
	{
		if (this->m_loaded_textures[i].getFilePath() == file_path)
			return i;
	}
	std::nullopt;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString path;
		material->GetTexture(type, i, &path);
		std::optional<unsigned int> loaded_texture = this->getLoadedTextureIndex(path.C_Str());
		if (loaded_texture.has_value())
		{
			textures.push_back(this->m_loaded_textures[loaded_texture.value()]);
		}
		else {
			Texture texture(path.C_Str());
			textures.push_back(texture);
			this->m_loaded_textures.push_back(texture);
		}
	}
	return textures;
}
