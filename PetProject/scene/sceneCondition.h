#pragma once

#include <array>

class Camera;

class Scene
{
private:
	 
public:
	std::shared_ptr<Camera> camera;
	std::array<float, 3> lightDirection = {-0.2f, -1.0f, -0.3f};
	std::array<float, 3> lightIntencity = { 1.0f, 1.0f, 1.0f };
	Scene(std::shared_ptr<Camera> _camera) 
		: camera(std::move(_camera)) {}
	Scene(Scene& _scene)
	{
		camera = _scene.camera;
		lightDirection = _scene.lightDirection;
		lightIntencity = _scene.lightIntencity;
	}
};