#pragma once

#include <algorithm>
#include <filesystem>

#include "shaders/shader.h"
#include "model/model.h"

#include "fileDialog/FileDialog.h"

#include "lowpoly/lowPolyGenerator.h"
#include "scene/sceneCondition.h"

class Controller
{
private:
	std::shared_ptr<Model> model;
	std::shared_ptr<Shader> modelShader;
	std::shared_ptr<Scene> scene;
	std::filesystem::path currentModelPath;
	float edgeLimit;
public:
	Controller(std::shared_ptr<Model> _model, 
		std::shared_ptr<Shader> _modelShader, 
		std::shared_ptr<Scene> _scene);
	void OpenModel();
	void SetCurrentModelPath(const std::filesystem::path& file_path);
	void SetModelColor(float _modelColor[3]);
	void SetModelSize(float value);
	void SetEdgeLimit(float value);
	void SetLigthIntencity(float value);
	void SetLigthDirection(float _lightDir[3]);
	void CreateLowPolyVersion();
};