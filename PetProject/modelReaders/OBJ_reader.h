#pragma once
#include "Ireader.h"

namespace ModelInput
{
    class OBJ_Reader : public ModelReader
    {
    public:
        void readModel(const std::filesystem::path& path, SimplificationAlgorithm::ModelData& model) override;
    };
}
