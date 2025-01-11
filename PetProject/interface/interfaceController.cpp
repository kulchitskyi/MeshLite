#include "interfaceController.h"


Controller::Controller(std::shared_ptr<Model> _model, 
		std::shared_ptr<Shader> _modelShader, 
		std::shared_ptr<Scene> _scene)
	: model(_model), 
	  modelShader(_modelShader), 
	  scene(_scene),
	  edgeLimit(0.001f)
	{
		 
	}

void Controller::OpenModel()
{
	FileDialog fileDialog;
	std::filesystem::path modelPath = fileDialog.OpenFile("OBJ files (*.obj)\0*.obj\0"); //filters for .obj only
	if (modelPath != "")
	{
		model->loadModel(modelPath);
		currentModelPath = modelPath;
	}
}

void Controller::SetCurrentModelPath(const std::filesystem::path& file_path)
{
	currentModelPath = file_path;
}

void Controller::SetModelColor(float _modelColor[3])
{
	std::copy(_modelColor, _modelColor+3, model->modelColor.begin());
}

void Controller::SetModelSize(float value)
{
	model->modelSize = value;
}

void Controller::SetEdgeLimit(float value)
{
	edgeLimit = value;
}

void Controller::SetLigthIntencity(float value)
{
	std::fill(scene->lightIntencity.begin(), scene->lightIntencity.end(), value);
}

void Controller::SetLigthDirection(float lightDir[3])
{
	std::copy(lightDir, lightDir+3, scene->lightDirection.begin());
}


void Controller::CreateLowPolyVersion()
{
	VDPL::GeneratorInterface generator(model->modelPath, edgeLimit);
	std::cout << "PATH:" << generator.lowpolyFilePath;
	currentModelPath = generator.lowpolyFilePath;
	model->loadModel(currentModelPath);
	//cond.loadModel = false;
}