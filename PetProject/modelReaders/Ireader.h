#pragma once
#include "simplificationAlgorithms/ModelData.h"

#include <filesystem>

namespace ModelInput
{
    class ModelReader
    {
    public:
        virtual void readModel(const std::filesystem::path& path, SimplificationAlgorithm::ModelData& model) = 0;
        virtual ~ModelReader() = default;
    };
}