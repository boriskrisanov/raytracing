#pragma once

#include "AABB.hpp"
#include "SceneObject.hpp"

class Triangle;

// TODO: Memory management (destructor)
class BVH
{
public:
    explicit BVH(std::vector<Triangle*> triangles, int depth = 0);
    RayIntersection findClosestIntersection(const Ray& ray, const Interval& lambdaRange) const;
    const BVH* getLeft() const;
    const BVH* getRight() const;
    AABB getBoundingBox() const;

    static constexpr int maxDepth = 8;
private:
    std::vector<Triangle*> leafTriangles;
    BVH* left;
    BVH* right;
    AABB boundingBox;
};
