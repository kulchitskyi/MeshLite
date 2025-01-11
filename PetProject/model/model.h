#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "model/mesh.h"
#include "shaders/shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <filesystem>

unsigned int TextureFromFile(const char* path, const std::filesystem::path& directory, bool gamma = false);

class Model
{
public:

	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::filesystem::path modelPath;
	bool gammaCorrection;

	float modelSize = 1.0f;
	std::array<float, 4> modelColor = { 0.7f, 1.0f, 0.7f, 1.0f };

	Model() = default;

	Model(const std::filesystem::path& path, bool gamma = false);

	void Draw(Shader& shader);

	void loadModel(const std::filesystem::path& path);

private:
	void processNode(aiNode* node, const aiScene* scene);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};