#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <iomanip>
#include <algorithm>

namespace VDPL //vertex data processing logik
{

    enum edgeIndexes
    {
        v0v1 = 1,
        v1v2 = 2,
        v0v2 = 0,
    };

    struct Vertex
    {
        uint32_t vertexNumber;
        float x;
        float y;
        float z;
        std::vector<uint32_t> relatedFace;
    };

    struct Vec3
    {
        uint32_t vectorNumber;
        float x;
        float y;
        float z;
        std::vector<uint32_t> relatedFace;
    };

    struct Edge
    {
        Vertex v1;
        Vertex v2;
    };

    struct VertexCombination
    {
        uint32_t v;
        uint32_t v_texture;
        uint32_t normal;
    };

    struct Face
    {
        uint32_t faceNumber;
        VertexCombination vert[3];
        float edgeLength[3];
    };


    class OBJReader
    {
    private:
        static uint32_t faceCounte;
        static uint32_t vertexCounte;
        static uint32_t normalCounte;

    public:
        OBJReader();

        void ResetCounters();

        Vertex readVer(std::string input);
        Vec3 readVec3(std::string input);
        Face readFace(std::string input, std::vector<Vertex>& verticies);
        void readFromObj(std::string path, std::vector<Vertex>& verticies, std::vector<Face>& faces, std::vector<Vec3>& normals);
    };

    class Mesh
    {
    public:
        std::vector<Vertex> allVerticies;
        std::vector<Face> allFaces;
        std::vector<Vec3> allNormals;

        Mesh(std::string path);
    };


    class MeshProcessor
    {
    private:

        std::vector<Edge> RemovedEdges;

    public:

        float calkDist(const Vertex& v1, const Vertex& v2);

        std::vector<float> calculateLength(std::vector<Face>& faces, std::vector<Vertex>& verticies);

        std::vector<int> selectEdgesToDelete(std::vector<float>& edgeLengths, float limit);

        bool isFaceContainSameVert(Face& f);

        std::vector<int> facesToDelete(std::vector<Face>& faces);

        void mergeEdge(Edge& edge, Mesh& mesh);

        void mergeAllOddEdges(std::vector<Edge>& edgeToRemove, Mesh& mesh);

        std::vector<Edge> transforming(Mesh& mesh, std::vector<int>& faceToDel, float edgeLimit);
    };

    class FileWriter
    {
    public:

        std::string fastWriteToFile(std::string path, Mesh& mesh, std::vector<int>& faceToDel);
    };
}