#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:

	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	Model(std::string const& path, bool gamma = false);

	void Draw(Shader& shader);

	void loadModel(std::string const& path);

private:
	void processNode(aiNode* node, const aiScene* scene);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};