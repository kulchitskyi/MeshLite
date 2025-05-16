#include "interface/interfaceController.h"
#include "controls/controller.h"
#include "shaders/shader.h"
#include "model/model.h"
#include "scene/sceneCondition.h"

GUIController::GUIController(
	std::shared_ptr<Scene> scene,
	std::shared_ptr<Controller> controller)
	: 
	_scene(scene),
	_controller(controller)
{}

void GUIController::openModel()
{
	std::filesystem::path modelPath = _fileDialog.openFile("OBJ files (*.obj)\0*.obj\0"); //filters for .obj only
	if (modelPath != "")
	{
		auto model = std::make_shared<AssimpLoader::Model>(modelPath);
		_scene->addModel(model);
		_scene->selectedModels.clear();
		_scene->selectedModels.push_back(model);
	}
}

void GUIController::setModelColor(float _modelColor[3])
{
	if (_scene->selectedModels.empty())
	{
		return;
	}
	for (auto model : _scene->selectedModels)
	{
		std::copy(_modelColor, _modelColor + 3, model->modelColor.begin());
	}
}

void GUIController::setModelSize(float value)
{
	if (_scene->selectedModels.empty())
	{
		return;
	}
	for (auto model : _scene->selectedModels)
	{
		model->modelSize = value; //TODO here like active model
	}
}

void GUIController::setVertexLimit(uint32_t targetVertexCount)
{
	_vertexLimit = targetVertexCount;
}

void GUIController::setLigthIntencity(float value)
{
	std::fill(_scene->lightIntencity.begin(), _scene->lightIntencity.end(), value);
}

void GUIController::setLigthDirection(float lightDir[3]) //TODO std::array
{
	std::copy(lightDir, lightDir+3, _scene->lightDirection.begin());
}


void GUIController::createLowPolyVersion()
{
	_controller->simplifySelectedModels(_vertexLimit);
}