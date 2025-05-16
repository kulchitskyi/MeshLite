#pragma once
#include "Iwriter.h"

#include <filesystem>



struct Model;

namespace ModelOutput
{
	class OBJ_Writer : public ModelWriter
	{
		void saveModel(SimplificationAlgorithm::ModelData& model, const std::filesystem::path &targetPath) override;
	};
}