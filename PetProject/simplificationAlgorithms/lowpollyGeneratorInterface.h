#pragma once
#include <filesystem>


namespace SimplificationAlgorithm
{
	struct ModelData;

	class BaseLowpollyGenerator
	{
	public:
		virtual void simplifyMesh(ModelData &model, uint32_t targetVertexCount) = 0;
		virtual ModelData getModel() = 0;
		virtual ~BaseLowpollyGenerator() = default;
	};
}
