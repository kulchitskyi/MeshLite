#pragma once

#include "../lowpollyGeneratorInterface.h"
#include "../modelData.h"


#include <sstream>
#include <vector>
#include <set>
#include <unordered_set>

#include <algorithm>

#include "glm/glm.hpp"

#include <unordered_map>
#include <array>


namespace SimplificationAlgorithm
{
    struct PotentialPoint;

    bool operator<(const PotentialPoint& p1, const PotentialPoint& p2);

    class QEMcalculator final
    {
    private:
        ModelData& _model;
        std::unordered_map<uint32_t, glm::mat4x4> _QEMforPoints;

    public:

        QEMcalculator(ModelData& model);

        auto calculateFaceQEM(const Face& f, const Vertex& point) -> glm::mat4x4;
        auto calculateQEM(const Vertex& point, const uint32_t pintIndex) -> glm::mat4x4;
        void calculateAllQEM();

        auto calculateNewPoint(Edge& edge) -> PotentialPoint;
        auto calculateCandidateMerges(std::vector<Edge> edges) -> std::multiset<PotentialPoint>;
        auto calculateNewPoinPos(glm::mat4x4 pointQEM, std::pair<glm::vec3, glm::vec3> edge) -> glm::vec3;
        auto calculateError(glm::mat4x4 pointQEM, glm::vec3 newPoint) -> float;

        auto recalculateNormal(Face& face) -> Normal;

        auto addQEMforPoint(uint32_t pointIndex);
        auto deletePointFromQEM(uint32_t pointIndex);
    };

    class QEM_MeshSimplifier : public BaseLowpollyGenerator
    {
    private:
        ModelData& _simplifiedModel;
    public:
        QEM_MeshSimplifier(ModelData& model) : _simplifiedModel(model) {};
        void setModel(ModelData& model);
        auto getModel() -> ModelData override;
        void simplifyMesh(ModelData& model, uint32_t targetVertexCount) override;  
    };

    class QEM_MeshSimplifierImplementation
    {
    private:
        ModelData& _model;
        QEMcalculator _QEM_Calculator;
        std::multiset<PotentialPoint> _mergeCandidates;
        uint32_t _vertexCount = 0;
        uint32_t _indexCount = 0;
    public:

        QEM_MeshSimplifierImplementation(ModelData& model);

        void simplifyMesh(uint32_t targetVertexCount);

        auto initNewPoint(glm::vec3 vertex) -> uint32_t;


        auto mergePoints(PotentialPoint pp);

        auto findFacesByPoint(uint32_t pointIndex) -> std::vector<FaceIterator>; 
        auto replacePointInFace(Face& face, uint32_t oldPointIndex, uint32_t newPointIndex);
        void deleteFace(FaceIterator refToFace);

        auto getAllEdges() -> std::vector<Edge>;

        void addMergeCandidates(std::vector<Edge> affectedEdges);
        void deleteMergeCandidates(std::vector<Edge> affectedEdges);
    };
}