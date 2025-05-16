#pragma once

#include "ModelReaders/IReader.h"
#include "ModelWriters/IWriter.h"

#include <memory>

using ModelReader = ModelInput::ModelReader;
using ModelWriter = ModelOutput::ModelWriter;

namespace Model_IO
{
	class Model_IO_Factory
	{
	public: 
		virtual std::unique_ptr<ModelReader> createReader() = 0;
		virtual std::unique_ptr<ModelWriter> createWriter() = 0;
		virtual ~Model_IO_Factory() = default;
	};

	class OBJ_Model_IO_Factory : public Model_IO_Factory
	{
	public:
		std::unique_ptr<ModelReader> createReader();
		std::unique_ptr<ModelWriter> createWriter();
	};
}

