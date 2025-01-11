#pragma once

#include "ProcessingLogik.h"
#include <filesystem>

namespace VDPL
{

	class GeneratorInterface
	{
	public:
		std::filesystem::path lowpolyFilePath;
		GeneratorInterface(const std::filesystem::path& path, float edgeLimit);
		GeneratorInterface(const char* path, float edgeLimit);
	};
}