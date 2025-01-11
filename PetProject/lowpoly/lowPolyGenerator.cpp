#include "lowPolyGenerator.h"

namespace VDPL //vertex data processing logik
{
	GeneratorInterface::GeneratorInterface(const std::filesystem::path& path, float edgeLimit)
	{
		std::cout << std::fixed << std::setprecision(6);

		Mesh myMesh(path);

		MeshProcessor proc;

		std::vector<int> faceToDel;

		std::vector<Edge> edgeToRemove = proc.transforming(myMesh, faceToDel, edgeLimit);

		FileWriter writer;
		
		std::cout << "\n\n\n" << faceToDel.size() << " face(s) was deleted.\n\n\n";
		lowpolyFilePath = writer.fastWriteToFile(path.generic_string(), myMesh, faceToDel);

		std::cout << "Mesh recalculation DONE.\n\n\n\n\n";
	}
	/*GeneratorInterface::GeneratorInterface(const char* path, float edgeLimit)
	{
		std::string strPath(path);
		GeneratorInterface(strPath, edgeLimit);
	}*/
}
