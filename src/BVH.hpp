#pragma once

#include "AABB.hpp"
#include "SceneObject.hpp"

class Triangle;

// TODO: Memory management (destructor)
class BVH
{
public:
    explicit BVH(std::vector<Triangle*> triangles);
    std::vector<Triangle*> getPossibleIntersections(const Ray& ray);
private:
    Triangle* triangle;
    BVH* left;
    BVH* right;
    AABB boundingBox;
};
