#pragma once

#include "glm/glm.hpp"

#include <unordered_map>
#include <array>

namespace SimplificationAlgorithm
{
    struct Vertex
    {
        glm::vec3 coord;
    };

    using Normal = glm::vec3;

    struct Edge {
        uint32_t point1; 
        uint32_t point2;

        Edge(uint32_t a, uint32_t b) 
            : point1(std::min(a, b)),
            point2(std::max(a, b))
        {}


        bool operator<(const Edge& other) const {
            return std::tie(point1, point2) < std::tie(other.point1, other.point2);
        }
        bool operator==(const Edge& other) const {
            return (point1 == other.point1 && point2 == other.point2) ||
                (point1 == other.point2 && point2 == other.point1);
        }
    };
    
    struct Face
    {
        std::array<uint32_t, 3> points;
        Normal normal;

        bool operator<(const Face& other) const {
            return points < other.points;
        }

        bool operator==(const Face& other) const {
            return points == other.points &&
                normal == other.normal;
        }
    };

    struct PotentialPoint
    {
        glm::vec3 coord;
        float error;
        Edge parentEdge;
    };
    
    using FaceIterator = std::list<Face>::iterator;

    struct ModelData
    {
        std::unordered_map<uint32_t, Vertex> points;
        std::list<Face> polygons;
        std::unordered_map<uint32_t, std::vector<FaceIterator>> pointPolygonMap;
    };
}