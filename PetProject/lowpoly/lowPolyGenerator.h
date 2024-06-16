#pragma once

#include "ProcessingLogik.h"

namespace VDPL
{

	class GeneratorInterface
	{
	public:
		std::string lowpolyFilePath;
		GeneratorInterface(const std::string& path, float edgeLimit);
		GeneratorInterface(const char* path, float edgeLimit);
	};
}