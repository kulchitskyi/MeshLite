#include "QEM_LowpollyGenerator.h"


namespace SimplificationAlgorithm 
{
    bool operator<(const PotentialPoint& p1, const PotentialPoint& p2)
    {
        return p1.error < p2.error;
    }

    QEMcalculator::QEMcalculator(ModelData& model) : _model{ model }
    {}

    auto QEMcalculator::calculateFaceQEM(const Face& f, const Vertex& point) -> glm::mat4x4
    {
        glm::vec3 normal = f.normal;

        float d = -(normal.x * point.coord.x + normal.y * point.coord.y + normal.z * point.coord.z);

        glm::vec4 planeEquasion = { normal, d };

        return glm::outerProduct(planeEquasion, planeEquasion);
    }

    auto QEMcalculator::calculateQEM(const Vertex& point, const uint32_t pointIndex) -> glm::mat4x4
    {
        glm::mat4x4 result{};

        if (_model.pointPolygonMap.at(pointIndex).empty())
        {
            return glm::mat4x4(666);
        }

        for (const auto& face : _model.pointPolygonMap.at(pointIndex))
        {
            result += calculateFaceQEM(*face, point);
        }

        return result;
    }

    void QEMcalculator::calculateAllQEM()
    {
        _QEMforPoints.reserve(_model.points.size());
        uint32_t index = 1;
        
        for (auto& [vertexIndex, vertex] : _model.points)        
        {
            glm::mat4x4 QEM = calculateQEM(vertex, vertexIndex);
            _QEMforPoints.insert({ vertexIndex, QEM });
        }
    }

    auto QEMcalculator::calculateNewPoint(Edge& edge) -> PotentialPoint
    {
        auto combinedQEM = _QEMforPoints[edge.point1] + _QEMforPoints[edge.point2];
        auto newPoint = calculateNewPoinPos(combinedQEM, { _model.points[edge.point1].coord, _model.points[edge.point2].coord });
        auto newPointError = calculateError(combinedQEM, newPoint);
        return { newPoint, newPointError, edge };
    }

    auto QEMcalculator::calculateCandidateMerges(std::vector<Edge> edges) -> std::multiset<PotentialPoint>
    {
        std::multiset<PotentialPoint> result;
        for (auto edge : edges)
        {
            result.insert(calculateNewPoint(edge));
        }
        return result;
    }

    auto QEMcalculator::calculateNewPoinPos(glm::mat4x4 pointQEM, std::pair<glm::vec3, glm::vec3> edge) -> glm::vec3
    {
        glm::mat3x3 matrix{ pointQEM };

        if (glm::determinant(matrix) == 0)
        {
            return edge.first;
        }

        matrix = glm::inverse(matrix);
        glm::vec3 partOfEquasion{ -pointQEM[0].w, -pointQEM[1].w, -pointQEM[2].w };

        glm::vec3 result = matrix * partOfEquasion;
        return result;
    }

    auto QEMcalculator::calculateError(glm::mat4x4 pointQEM, glm::vec3 newPoint) -> float
    {
        glm::vec4 extendedNewPoint{ newPoint, 1 };

        glm::vec4 errorVec = pointQEM * extendedNewPoint;

        return glm::dot(extendedNewPoint, errorVec);
    }

    auto QEMcalculator::recalculateNormal(Face& face) -> Normal
    {
        auto point1 = _model.points[face.points[0]].coord;
        auto point2 = _model.points[face.points[1]].coord;
        auto point3 = _model.points[face.points[2]].coord;

        auto edge1 = point2 - point1;
        auto edge2 = point3 - point1;

        auto newNormal = glm::cross(edge1, edge2);
        newNormal = glm::normalize(newNormal);

        return newNormal;
    }

    auto QEMcalculator::addQEMforPoint(uint32_t pointIndex)
    {
        _QEMforPoints[pointIndex] = calculateQEM(_model.points[pointIndex], pointIndex);
    }

    auto QEMcalculator::deletePointFromQEM(uint32_t pointIndex)
    {
        _QEMforPoints.erase(pointIndex);
    }

    auto IsPointInFace(uint32_t pointIndex, Face& face) -> bool
    {
        return std::find(face.points.begin(), face.points.end(), pointIndex) != face.points.end();
    }

    QEM_MeshSimplifierImplementation::QEM_MeshSimplifierImplementation(ModelData& model)
        : _model{ model },
        _QEM_Calculator{ QEMcalculator(model) },
        _vertexCount(model.points.size()),
        _indexCount{ _vertexCount }
    {
    }

    auto QEM_MeshSimplifierImplementation::replacePointInFace(Face& face, uint32_t oldPointIndex, uint32_t newPointIndex)
    {
        std::replace(face.points.begin(), face.points.end(), oldPointIndex, newPointIndex);
        face.normal = _QEM_Calculator.recalculateNormal(face);
    }

    auto QEM_MeshSimplifierImplementation::findFacesByPoint(uint32_t pointIndex) -> std::vector<FaceIterator>
    {
        return _model.pointPolygonMap[pointIndex];
    }

    auto QEM_MeshSimplifierImplementation::getAllEdges() -> std::vector<Edge>
    {
        std::vector<Edge> edges;

        edges.reserve(_model.points.size());

        for (auto& face : _model.polygons)
        {
            edges.push_back({face.points[0], face.points[1]});
            edges.push_back({face.points[1], face.points[2]});
            edges.push_back({face.points[2], face.points[0]});
        }

        return edges;
    }

    auto GetAffectedEdges(std::vector<FaceIterator> affectedFaces, uint32_t pointIndex) -> std::vector<Edge> //TODO vector<Face&>
    {
        std::vector<Edge> result;

        for (const auto& refToFace : affectedFaces)
        {
            for (const auto& pointIndex_ : refToFace->points)
            {
                if (pointIndex_ != pointIndex)
                {
                    result.push_back({ std::min(pointIndex_, pointIndex), std::max(pointIndex_, pointIndex) });
                }
            }
        }

        return result;
    }

    auto QEM_MeshSimplifierImplementation::initNewPoint(glm::vec3 vertex) -> uint32_t
    {
        auto newPointIndex = ++_indexCount;
        _model.points[newPointIndex] = Vertex{ vertex };
        return newPointIndex;
    }

    void QEM_MeshSimplifierImplementation::deleteMergeCandidates(std::vector<Edge> affectedEdges)
    {
        for (auto oldEdge : affectedEdges)
        {
            auto oldMergeCandidate = _QEM_Calculator.calculateNewPoint(oldEdge);
            _mergeCandidates.erase(oldMergeCandidate);
        }
    }

    void QEM_MeshSimplifierImplementation::addMergeCandidates(std::vector<Edge> affectedEdges)
    {
        for (auto newEdge : affectedEdges)
        {
            _mergeCandidates.insert(_QEM_Calculator.calculateNewPoint(newEdge));
        }
    }

    auto FindIntersection(std::vector<FaceIterator> vec1, std::vector<FaceIterator> vec2) -> std::vector<FaceIterator>
    {
        std::vector<FaceIterator> intersection;
        for (auto& i1 : vec1) {
            for (auto& i2 : vec2) {
                if (*i1 == *i2) {
                    intersection.push_back(i1);
                }
            }
        }

        return intersection;
    }

    void QEM_MeshSimplifierImplementation::deleteFace(FaceIterator faceItert)
    {
        for (auto pointIndex : faceItert->points)
        {
            auto& relatedFacesToPoint = _model.pointPolygonMap[pointIndex];
            relatedFacesToPoint.erase(std::remove_if(relatedFacesToPoint.begin(), relatedFacesToPoint.end(),
                [&](auto _face) { return *_face == *faceItert; }), relatedFacesToPoint.end());
            if (relatedFacesToPoint.empty())
            {
                // TODO PROPER DELETION !!!!!!!!!!!!!!!
                
                _model.pointPolygonMap.erase(pointIndex);
                _vertexCount--;
            }
        }
    }

    auto QEM_MeshSimplifierImplementation::mergePoints(PotentialPoint pp)
    {
        const auto parentPoint1Index = pp.parentEdge.point1;
        const auto parentPoint2Index = pp.parentEdge.point2;

        auto newPointIndex = initNewPoint(pp.coord);

        auto affectedFacesPart1 = findFacesByPoint(parentPoint1Index);
        auto affectedFacesPart2 = findFacesByPoint(parentPoint2Index);


        auto oldEdges = GetAffectedEdges(affectedFacesPart1, parentPoint1Index);

        auto oldEdgesPart2 = GetAffectedEdges(affectedFacesPart2, parentPoint2Index);

        oldEdges.insert(oldEdges.end(), oldEdgesPart2.begin(), oldEdgesPart2.end());

        deleteMergeCandidates(oldEdges);

        auto facesToDelete = FindIntersection(affectedFacesPart1, affectedFacesPart2);

        for (auto& faceIterator : facesToDelete)
        {
            deleteFace(faceIterator);
            affectedFacesPart1.erase(std::remove_if(affectedFacesPart1.begin(), affectedFacesPart1.end(), [&](auto iter) {return *iter == *faceIterator; }));
            affectedFacesPart2.erase(std::remove_if(affectedFacesPart2.begin(), affectedFacesPart2.end(), [&](auto iter) {return *iter == *faceIterator; }));

            _model.polygons.erase(faceIterator);
        }

        _model.pointPolygonMap.erase(parentPoint1Index);
        _model.pointPolygonMap.erase(parentPoint2Index);

        _QEM_Calculator.deletePointFromQEM(parentPoint1Index);
        _QEM_Calculator.deletePointFromQEM(parentPoint2Index);

        std::copy(affectedFacesPart1.begin(), affectedFacesPart1.end(), std::back_inserter(_model.pointPolygonMap[newPointIndex]));
        std::copy(affectedFacesPart2.begin(), affectedFacesPart2.end(), std::back_inserter(_model.pointPolygonMap[newPointIndex]));

        _QEM_Calculator.addQEMforPoint(newPointIndex);


        for (auto& face : affectedFacesPart1)
        {
            replacePointInFace(*face, parentPoint1Index, newPointIndex);
        }

        for (auto& face : affectedFacesPart2)
        {
            replacePointInFace(*face, parentPoint2Index, newPointIndex);
        }

        auto facesOfNewPoint = findFacesByPoint(newPointIndex);

        auto newEdges = GetAffectedEdges(facesOfNewPoint, newPointIndex);

        addMergeCandidates(newEdges);
        _vertexCount--;
    }

    void QEM_MeshSimplifierImplementation::simplifyMesh(uint32_t targetVertexCount)
    {    
        _QEM_Calculator.calculateAllQEM();

        addMergeCandidates(getAllEdges());

        while (_vertexCount > targetVertexCount)
        {
            mergePoints(*_mergeCandidates.begin());
        }
    }
    
    void QEM_MeshSimplifier::setModel(ModelData& model)
    {
        _simplifiedModel = model;
    }

    auto QEM_MeshSimplifier::getModel() -> ModelData
    {
        return _simplifiedModel;
    }

    void QEM_MeshSimplifier::simplifyMesh(ModelData& model, uint32_t targetVertexCount)
    {
        auto generator = QEM_MeshSimplifierImplementation(model);
        generator.simplifyMesh(targetVertexCount);
    }
}
