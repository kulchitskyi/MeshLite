#include "controller.h"
#include "scene/sceneCondition.h"
#include "simplificationAlgorithms/QEM_Algorithm/QEM_LowpollyGenerator.h"
#include "model/model.h"

#include <memory>

void Controller::simplifySelectedModels(uint32_t targetVertexCount)
{
	auto modelToSimplify = SimplificationAlgorithm::ModelData();

	using SimplificationAlgorithm::BaseLowpollyGenerator;
	using SimplificationAlgorithm::QEM_MeshSimplifier;

	std::unique_ptr<BaseLowpollyGenerator> MeshSimplifier
		= std::make_unique<QEM_MeshSimplifier>(QEM_MeshSimplifier(modelToSimplify));

	using Model_IO::OBJ_Model_IO_Factory;
	using ModelInput::ModelReader;
	using ModelOutput::ModelWriter;

	IO_factory = std::make_unique<OBJ_Model_IO_Factory>(OBJ_Model_IO_Factory());

	std::unique_ptr<ModelReader> modelReader = IO_factory->createReader();
	std::unique_ptr<ModelWriter> modelWriter = IO_factory->createWriter();
	
	for (auto& model : _scene->selectedModels)
	{

		auto tempModel = SimplificationAlgorithm::ModelData{};
		modelReader->readModel(model->modelPath, tempModel);
		MeshSimplifier->simplifyMesh(tempModel, targetVertexCount);

		auto modifiedFilename = model->modelPath.stem().string() + "_low" + model->modelPath.extension().string();
		auto lowpolyModelPath = model->modelPath.parent_path() / modifiedFilename;

		modelWriter->saveModel(tempModel, lowpolyModelPath);
 		model->loadModel(lowpolyModelPath);
	}
}

Controller::Controller(std::shared_ptr<Scene> scene, GLFWwindow* window) : _scene(scene), _window(window)
{
}

Controller::~Controller() = default;
