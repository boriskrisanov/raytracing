#pragma once
#include <string>
#include <vector>

#include "Triangle.hpp"

class Mesh
{
public:
    Mesh(std::string source, Material* material);
    const std::vector<Triangle>& getTriangles() const;
    const Vector3& getCentre() const;
public:
    std::vector<Triangle> triangles;
    Vector3 centre;
};
