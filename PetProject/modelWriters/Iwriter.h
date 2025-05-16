#pragma once
#include "simplificationAlgorithms/modelData.h"

#include <filesystem>

namespace ModelOutput
{
    class ModelWriter
    {
    public:
        virtual void saveModel(SimplificationAlgorithm::ModelData& model, const std::filesystem::path& path) = 0;
        virtual ~ModelWriter() = default;
    };
};

