#include "OBJ_Reader.h"

#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <iostream>

using SimplificationAlgorithm::ModelData;
using SimplificationAlgorithm::Face;
using SimplificationAlgorithm::Normal;
using SimplificationAlgorithm::Vertex;

namespace ModelInput
{
    constexpr int pointsPerFace = 3;


    void OBJ_Reader::readModel(const std::filesystem::path& path, ModelData& model)
    {
        std::vector<Normal> tempNormals;

        std::ifstream file(path);
        if (!file.is_open())
        {
            std::cerr << "Failed to open file: " << path << std::endl;
            return;
        }

        std::string line;

        uint32_t faceIndex = 0;
        uint32_t vertexIndex = 1;
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string prefix;
            ss >> prefix;

            if (prefix == "v")  // Vertex
            {
                Vertex vertex;
                ss >> vertex.coord.x >> vertex.coord.y >> vertex.coord.z;
                model.points[vertexIndex++] = vertex;
            }
            else if (prefix == "vn")  // Vertex
            {
                glm::vec3 normal;
                ss >> normal.x >> normal.y >> normal.z;
                tempNormals.push_back(normal);
            }
            else if (prefix == "f")  // Face
            {
                Face face;
                for (int i = 0; i < pointsPerFace; ++i)
                {

                    uint32_t vIndex = 0;
                    uint32_t vnIndex = 0;
                    uint32_t vtIndex = 0;
                    char slash;

                    ss >> vIndex >> slash >> vtIndex >> slash >> vnIndex;

                    face.points[i] = vIndex;
                    face.normal = tempNormals[vnIndex - 1];
                }

                model.polygons.push_back(face);

                for (auto point : face.points)
                {

                    model.pointPolygonMap[point].push_back(std::prev(model.polygons.end())); //rewrite to shared_ptr
                }

                // TODO
            }
        }

        file.close();
    }
}