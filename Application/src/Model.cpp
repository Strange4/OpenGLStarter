#include "pch.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"

Model::Model(std::string directory_path)
	: m_transform(glm::mat4(1.0f))
{
	this->loadModel(directory_path);
}

void Model::draw(ShaderProgram& shader_program) const
{
	for (auto& mesh : this->m_meshes)
	{
	    mesh.get()->draw(shader_program);
	}
}

void Model::addTransform(const glm::mat4& transform)
{
	this->m_transform *= transform;
}

void Model::setTransform(const glm::mat4& transform)
{
	this->m_transform = transform;
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
	aiMatrix4x4 rootTransform = scene->mRootNode->mTransformation;
	
	this->m_transform *= glm::make_mat4(&rootTransform.a1);
	this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->m_meshes.push_back(processMesh(mesh, scene));
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

	// default color if there are none
	glm::vec3 color(1.0f, 0.34f, 0.2f);

	// process materials
	if (mesh->mMaterialIndex >= 0)
	{
		// TODO: handle if it's a material color and not a texture
		// -> https://assimp-docs.readthedocs.io/en/v5.3.0/usage/use_the_lib.html#material-system
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		aiString materialName = material->GetName();
		aiColor3D retrievedColor;
		if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, retrievedColor)) {
			color = glm::vec3(retrievedColor.r, retrievedColor.g, retrievedColor.b);
		}

		textures = this->loadMaterialTextures(material, aiTextureType_DIFFUSE);
	}
	

	// process vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		aiVector3D position = mesh->mVertices[i];
		glm::vec2 texCoordinates(0.0f);
		glm::vec3 normal(0.0f);
		if (mesh->HasNormals())
		{
			aiVector3D importedNormal = mesh->mNormals[i];
			normal.x = importedNormal.x;
			normal.y = importedNormal.y;
			normal.z = importedNormal.z;
		}

		// we only allow 1 texture coodinate
		if (mesh->mTextureCoords[0])
		{
			texCoordinates.x = mesh->mTextureCoords[0][i].x;
			texCoordinates.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(
			Vertex{
				glm::vec3{position.x, position.y, position.z}, // position
				normal, // normal
				color, // color
				texCoordinates, // texture coordinates
			}
		);
	}

	// process indinces
	for (unsigned int i = 0; i < mesh->mNumFaces;i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			unsigned int index = face.mIndices[j];
			indices.push_back(index);
		}
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
	return std::nullopt;
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
			textures.emplace_back(this->m_loaded_textures[loaded_texture.value()]);
		}
		else {
			Texture texture(path.C_Str());
			textures.emplace_back(texture);
			this->m_loaded_textures.emplace_back(texture);
		}
	}
	return textures;
}
