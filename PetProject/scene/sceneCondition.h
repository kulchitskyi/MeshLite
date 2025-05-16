#pragma once

#include <array>
#include <vector>
#include <map>
#include <memory>
#include <glm/glm.hpp>

class Camera;

namespace AssimpLoader
{
	class Model;
}

class Scene
{
private:
	uint32_t modelCount = 0;
public:
	std::shared_ptr<Camera> camera;
	std::vector<std::shared_ptr<AssimpLoader::Model>> models;
	std::vector<std::shared_ptr<AssimpLoader::Model>> selectedModels;
	std::array<float, 3> lightDirection = {-0.2f, -1.0f, -0.3f};
	std::array<float, 3> lightIntencity = { 1.0f, 1.0f, 1.0f };
	std::map<int, glm::vec4> idToColor{
		{-1, {0.0f,0.0f,0.0f,1.0f}} };
	Scene(std::shared_ptr<Camera> _camera) 
		: camera(std::move(_camera)) 
	{	
	}

	Scene(Scene& _scene)
	{
		camera = _scene.camera;
		lightDirection = _scene.lightDirection;
		lightIntencity = _scene.lightIntencity;
	}
	void selectModel(int id);
	void addModel(std::shared_ptr<AssimpLoader::Model> model);
};