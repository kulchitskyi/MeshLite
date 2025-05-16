#include "OBJ_writer.h"

#include <fstream>

using namespace SimplificationAlgorithm;

namespace ModelOutput
{
    void OBJ_Writer::saveModel(SimplificationAlgorithm::ModelData& model, const std::filesystem::path& targetPath)
    {
        std::unordered_map<uint32_t, uint32_t> oldToNewIndex;
        std::unordered_map<uint32_t, bool> isUsed;

        for (const auto& face : model.polygons)
        {
            for (auto pointIndex : face.points)
            {
                isUsed[pointIndex] = true;
            }
        }

        std::ofstream file(targetPath);
        if (!file.is_open()) {
            return;
        }

        uint32_t newIndex = 1;
        for (uint32_t i = 1; i <= model.points.size(); ++i)
        {
            if (isUsed[i])
            {
                if (i < 1 || i > model.points.size()) {
                    return; //TODO throw
                }
                const auto& p = model.points.at(i);
                file << "v " << p.coord.x << " " << p.coord.y << " " << p.coord.z << '\n';
                oldToNewIndex[i] = newIndex++;
            }
        }

        file << "vt 0.000 0.000\n";

        for (const auto& face : model.polygons)
        {
            file << "vn " << face.normal.x << " " << face.normal.y << " " << face.normal.z << '\n';
        }

        uint32_t normalCounter = 1;
        for (const auto& face : model.polygons)
        {
            file << "f ";
            for (auto oldIndex : face.points)
            {
                auto it = oldToNewIndex.find(oldIndex);
                if (it == oldToNewIndex.end()) {
                   return; //TODO throw
                }
                uint32_t newIdx = it->second;

                uint32_t normalIdx = normalCounter;

                file << newIdx << "/1/" << normalIdx << ' ';
            }
            file << '\n';
            normalCounter++;
        }

        file.close();
    }
}
