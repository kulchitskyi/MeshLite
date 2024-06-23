#include "ProcessingLogik.h"

namespace VDPL //vertex data processing logik
{
    uint32_t OBJReader::faceCounte = 1;
    uint32_t OBJReader::vertexCounte = 1;
    uint32_t OBJReader::normalCounte = 1;

    bool operator ==(const Vertex& ver1, const Vertex& ver2)
    {
        return (ver1.x == ver2.x) && (ver1.y == ver2.y) && (ver1.z == ver2.z);
    }

    bool operator ==(const Edge& edge1, const Edge& edge2)
    {
        return ((edge1.v1 == edge2.v1) && (edge1.v2 == edge2.v2)) || ((edge1.v1 == edge2.v2) && (edge1.v2 == edge2.v1));
    }

    OBJReader::OBJReader()
    {
        OBJReader::faceCounte = 1;
        OBJReader::vertexCounte = 1;
        OBJReader::normalCounte = 1;
    }

    void OBJReader::ResetCounters()
    {
        OBJReader::faceCounte = 1;
        OBJReader::vertexCounte = 1;
        OBJReader::normalCounte = 1;
    }

    Vertex OBJReader::fromStrToVer(std::string input)
    {
        char temp;
        std::stringstream stringstream(input);
        Vertex vert;
        vert.vertexNumber = vertexCounte;
        vertexCounte++;
        stringstream >> temp >> vert.x >> vert.y >> vert.z;
        return vert;
    }

    Vec3 OBJReader::fromStrToVec3(std::string input)
    {
        char temp;
        std::stringstream stringstream(input);
        Vec3 normal;
        normal.vectorNumber = normalCounte;
        normalCounte++;
        stringstream >> temp >> temp >> normal.x >> normal.y >> normal.z;
        return normal;
    }

    Face OBJReader::fromStrToFace(std::string input, std::vector<Vertex>& verticies)
    {
        std::stringstream stringstream(input);
        char temp;

        Face face;
        face.faceNumber = faceCounte;
        faceCounte++;
        stringstream >> temp;
        for (int i = 0; i < 3; i++)
        {
            stringstream >> face.vert[i].v >> temp >> face.vert[i].v_texture >> temp >> face.vert[i].normal;
            verticies[face.vert[i].v - 1].relatedFace.push_back(face.faceNumber);
        }


        return face;
    }

    void OBJReader::readFromObj(std::string path, std::vector<Vertex>& verticies, std::vector<Face>& faces, std::vector<Vec3>& normals)
    {
        ResetCounters();
        std::ifstream myfile(path);
        std::string line;
        if (!myfile.is_open())
        {
            std::cout << "Unable to open file";
            return;
        }
        while (std::getline(myfile, line))
        {
            if (line.compare(0, 2, "v ") == 0)
            {
                verticies.push_back(fromStrToVer(line));
            }
            else if (line.compare(0, 2, "vn") == 0)
            {
                normals.push_back(fromStrToVec3(line));
            }
            else if (line.compare(0, 2, "f ") == 0)
            {
                faces.push_back(fromStrToFace(line, verticies));
            }
        }
        myfile.close();
    }


    Mesh::Mesh(std::string path)
    {
        OBJReader reader;
        reader.readFromObj(path, allVerticies, allFaces, allNormals);
    }
   
    float MeshProcessor::calkDist(const Vertex& v1, const Vertex& v2)
    {
        return pow((v1.x - v2.x), 2) + pow((v1.y - v2.y), 2) + pow((v1.z - v2.z), 2);
    }

    std::vector<float> MeshProcessor::calculateLength(std::vector<Face>& faces, std::vector<Vertex>& verticies)
    {
        std::vector<float> edgeLengths;
        edgeLengths.push_back(666); //for indexies
        for (auto f : faces)
        {
            edgeLengths.push_back(calkDist(verticies[f.vert[0].v - 1], verticies[f.vert[1].v - 1]));
            f.edgeLength[0] = calkDist(verticies[f.vert[0].v - 1], verticies[f.vert[1].v - 1]);
            edgeLengths.push_back(calkDist(verticies[f.vert[1].v - 1], verticies[f.vert[2].v - 1]));
            f.edgeLength[1] = calkDist(verticies[f.vert[1].v - 1], verticies[f.vert[2].v - 1]);
            edgeLengths.push_back(calkDist(verticies[f.vert[0].v - 1], verticies[f.vert[2].v - 1]));
            f.edgeLength[2] = calkDist(verticies[f.vert[0].v - 1], verticies[f.vert[2].v - 1]);
        }
        return edgeLengths;
    }

    std::vector<int> MeshProcessor::selectEdgesToDelete(std::vector<float>& edgeLengths, float limit)
    {
        std::vector<int> indiñes;
        for (int i = 1; i < edgeLengths.size(); i++)
        {
            if (edgeLengths[i] < limit * limit)
            {
                indiñes.push_back(i);
            }
        }
        return indiñes;
    }

    bool MeshProcessor::isFaceContainSameVert(Face& f)
    {
        return (f.vert[0].v == f.vert[1].v) || (f.vert[2].v == f.vert[1].v) || (f.vert[0].v == f.vert[2].v);
    }

    std::vector<int> MeshProcessor::facesToDelete(std::vector<Face>& faces)
    {
        std::vector<int> facesToDel;
        for (auto f : faces)
        {
            if (isFaceContainSameVert(f))
            {
                facesToDel.push_back(f.faceNumber);
            }
        }
        return facesToDel;
    }

    void MeshProcessor::mergeEdge(Edge& edge, Mesh& mesh) // v1 will be deleted 
    {

        for (auto e : RemovedEdges)
        {
            if ((edge.v1 == e.v1) || (edge.v2 == e.v1) || (edge.v1 == e.v2) || (edge.v2 == e.v2))
            {
                return;
            }
        }

        RemovedEdges.push_back(edge);
        for (auto faceIndex : edge.v1.relatedFace)
        {
            for (int i = 0; i < 3; i++)
            {
                if (mesh.allFaces[faceIndex - 1].vert[i].v == edge.v1.vertexNumber)
                {
                    mesh.allFaces[faceIndex - 1].vert[i].v = edge.v2.vertexNumber;
                }
            }    
        }           
    }

    void MeshProcessor::mergeAllOddEdges(std::vector<Edge>& edgeToRemove, Mesh& mesh)
    {
        for (auto edge : edgeToRemove)
        {
            mergeEdge(edge, mesh);
        }
    }

    std::vector<Edge> MeshProcessor::transforming(Mesh& mesh, std::vector<int>& faceToDel, float edgeLimit)
    {
        std::vector<float> allEdgeLengths = calculateLength(mesh.allFaces, mesh.allVerticies);
        std::vector<int>   result         = selectEdgesToDelete(allEdgeLengths, edgeLimit);

        std::vector<Edge> edgesToRemove;
        for (auto index : result)
        {
            int tempIndex = index % 3;
            int faceIndex = ((index + 2) / 3) - 1;
            std::pair<int, int> vertexIndicies;
            switch (tempIndex)
            {
            case(edgeIndexes(v0v1)):
                vertexIndicies.first  = mesh.allFaces[faceIndex].vert[0].v - 1;
                vertexIndicies.second = mesh.allFaces[faceIndex].vert[1].v - 1;
                break;
            case(edgeIndexes(v1v2)):
                vertexIndicies.first  = mesh.allFaces[faceIndex].vert[1].v - 1;
                vertexIndicies.second = mesh.allFaces[faceIndex].vert[2].v - 1;
                break;
            case(edgeIndexes(v0v2)):
                vertexIndicies.first  = mesh.allFaces[faceIndex].vert[0].v - 1;
                vertexIndicies.second = mesh.allFaces[faceIndex].vert[2].v - 1;
                break;
            default:
                break;
            }
            edgesToRemove.push_back(Edge(mesh.allVerticies[vertexIndicies.first], mesh.allVerticies[vertexIndicies.second]));
        }

        mergeAllOddEdges(edgesToRemove, mesh);

        faceToDel = facesToDelete(mesh.allFaces);

        return edgesToRemove;
    }

    std::string FileWriter::fastWriteToFile(std::string path, Mesh& mesh, std::vector<int>& faceToDel)
    {
        std::ifstream originalFile(path);
        if (!originalFile.is_open())
        {
            std::cerr << "Failed to open the original file: " << path << "\n";
            return std::string();
        }

        std::ofstream copiedFile;
        std::string newPath = path.substr(0, path.find_last_of("\\") + 1) + "lowpoly_" + path.substr(path.find_last_of("\\") + 1);
        copiedFile.open(newPath);
        std::string line;
        std::string lable = "#Model was created with the help of lowpoly generator.\n";
        if (std::getline(originalFile, line) && line.compare(0, lable.length(), lable) != 0)
        {
            copiedFile << lable;
        }
        if (!copiedFile.is_open())
        {
            std::cerr << "Failed to create the lowpoly file: " << path << "\n";
            return std::string();
        }

        while (std::getline(originalFile, line) && line.compare(0, 2, "f ") != 0)
        {
            copiedFile << line << "\n";
        }
        originalFile.close();
 
        int offset = 1; // offset when deleting from vector;

        for (auto it = faceToDel.rbegin(); it != faceToDel.rend(); it++)
        {
            auto it2 = (mesh.allFaces.begin() + *it - offset);
            mesh.allFaces.erase(it2);
        }
        for (auto it = mesh.allFaces.begin(); it != mesh.allFaces.end(); it++)
        {
            copiedFile << "f ";
            for (int i = 0; i < 3; i++)
            {
                copiedFile << it->vert[i].v << "/" << it->vert[i].v_texture << "/" << it->vert[i].normal << " ";
            }
            copiedFile << "\n";
        }
        copiedFile.close();
        return newPath;
    }
}

