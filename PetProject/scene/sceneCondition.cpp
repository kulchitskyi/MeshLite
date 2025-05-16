#include "sceneCondition.h"
#include "Utils/utils.h"

void Scene::selectModel(int id)
{
	
	selectedModels.push_back(models[id]);
}

void Scene::addModel(std::shared_ptr<AssimpLoader::Model> model)
{
	idToColor[modelCount] = glm::vec4(Utils::idToColor(modelCount), 1.0f);
	modelCount++;
	models.push_back(model);
}