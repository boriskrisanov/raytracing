#pragma once
#include <string>
#include <vector>

#include "Triangle.hpp"

class Mesh
{
public:
    Mesh(std::string source, Material* material);
    void translate(const Vector3& translation);
    std::vector<Triangle> triangles;
    std::vector<Vector3> objVertexes;
};
