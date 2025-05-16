#include "model_IO_Factory.h"
#include "ModelReaders/OBJ_reader.h"
#include "ModelWriters/OBJ_writer.h"


using ModelReader = ModelInput::ModelReader;
using ModelWriter = ModelOutput::ModelWriter;
using OBJ_Reader = ModelInput::OBJ_Reader;
using OBJ_Writer = ModelOutput::OBJ_Writer;
 
namespace Model_IO
{
    std::unique_ptr<ModelReader> OBJ_Model_IO_Factory::createReader()
    {
        return std::make_unique<OBJ_Reader>();
    }
    std::unique_ptr<ModelWriter> OBJ_Model_IO_Factory::createWriter()
    {
        return std::make_unique<OBJ_Writer>();
    }
}
