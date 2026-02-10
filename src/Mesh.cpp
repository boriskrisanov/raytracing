#include "Mesh.hpp"

#include <fstream>
#include <iostream>

using std::string;
using std::vector;

string loadFile(string path)
{
    // TODO: Error handling
    std::ifstream sourceFile{path};
    std::string currentLine;
    std::string sourceString;

    while (std::getline(sourceFile, currentLine))
    {
        currentLine += "\n";
        sourceString += currentLine;
    }

    sourceFile.close();

    return sourceString;
}

std::vector<string> splitString(string str, string delimiter)
{
    std::vector<string> result;

    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != string::npos)
    {
        token = str.substr(0, pos);
        result.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    result.push_back(str);

    return result;
}

Mesh::Mesh(std::string source, Material* material)
{
    const string modelSource = loadFile(source);
    std::vector<Vector3> objVertexes;

    for (string line : splitString(modelSource, "\n"))
    {
        const vector<string> splitLine = splitString(line, " ");
        const string firstToken = splitLine[0];

        if (firstToken == "v")
        {
            // Vertex
            const double x = std::stod(splitLine[1]);
            const double y = std::stod(splitLine[2]);
            const double z = std::stod(splitLine[3]);
            objVertexes.push_back({x, y, z});
        }
        else if (firstToken == "f")
        {
            // Face
            // vertex/textureCoordinate/normal

            vector<string> faceVertexDefinitions{splitLine};
            faceVertexDefinitions.erase(faceVertexDefinitions.begin());

            if (faceVertexDefinitions.size() != 3) [[unlikely]]
            {
                std::cout << ("OBJ loading failed: mesh " + source + " contains non triangular faces. OBJ file must be exported with triangulation enabled.\n");
                return;
            }

            std::vector<Vector3> face;

            for (string vertexDefinition : faceVertexDefinitions)
            {
                const vector<string> splitDefinition = splitString(vertexDefinition, "/");

                // Subtract 1 from each index because OBJ indexes start at 1
                const long vertexIndex = std::stol(splitDefinition[0]) - 1;
                // const long textureCoordinateIndex = splitDefinition[1].empty() ? -1 : std::stol(splitDefinition[1]) - 1;
                // const long normalIndex = std::stol(splitDefinition[2]) - 1;

                face.push_back(objVertexes[vertexIndex]);
            }

            triangles.emplace_back(face[0], face[1], face[2], material);

        }
    }

    Vector3 sum;
    for (Vector3 vertex : objVertexes)
    {
        sum += vertex;
    }
    centre = sum / objVertexes.size();
}

void Mesh::translate(const Vector3& translation)
{
    for (Triangle& triangle : triangles)
    {
        triangle.translate(translation);
    }
    centre += translation;
}

void Mesh::rotate(const Vector3& degrees)
{
    for (Triangle& triangle : triangles)
    {
        triangle.rotate(degrees, centre);
    }
}

const std::vector<Triangle>& Mesh::getTriangles() const
{
    return triangles;
}
