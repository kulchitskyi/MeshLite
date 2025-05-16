#include "command.h"
#include "scene/sceneCondition.h"
#include "model/model.h"

void SceneCommand::setScene(std::shared_ptr<Scene> scene)
{
	_scene = scene;
}

auto SceneCommand::getScene() -> std::shared_ptr<Scene>
{
	return _scene;
}

void CopyCommand::execute()
{
	using Model = AssimpLoader::Model;
	for (auto& model : _scene->selectedModels)
	{
		std::shared_ptr<Model> newModel = std::make_shared<Model>(*(model->clone()));
		newModel->modelPosition += glm::vec3{0.2f, 0.2f, 0.2f};
		_scene->addModel(newModel);
	}
}