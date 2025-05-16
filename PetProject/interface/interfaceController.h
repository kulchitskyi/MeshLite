#pragma once
#include "fileDialog/fileDialog.h"

#include <algorithm>
#include <filesystem>

class Controller;
class Scene;
class Shader;

class GUIController final
{
private:
	std::shared_ptr<Controller> _controller;
	std::shared_ptr<Scene> _scene;
	FileDialog _fileDialog;
	uint32_t _vertexLimit = 3;
public:
	GUIController(
		std::shared_ptr<Scene> scene,
		std::shared_ptr<Controller> controller);
	void openModel();
	void setModelColor(float _modelColor[3]);
	void setModelSize(float value);
	void setVertexLimit(uint32_t targetVertexCount);
	void setLigthIntencity(float value);
	void setLigthDirection(float _lightDir[3]);
	void createLowPolyVersion();
};