#include "BVH.hpp"

#include <algorithm>
#include <functional>

#include "AABB.hpp"
#include "Triangle.hpp"


// TODO: This is incredibly inefficient with vector copies
BVH::BVH(std::vector<Triangle*> triangles)
    : triangle(nullptr), left(nullptr), right(nullptr), boundingBox({})
{
    // Create bounding box
    for (const Triangle* const t : triangles)
    {
        boundingBox.includePoint(t->p1);
        boundingBox.includePoint(t->p2);
        boundingBox.includePoint(t->p3);
    }

    if (triangles.size() == 1)
    {
        triangle = triangles[0];
        return;
    }

    // Sort and split along longest axis
    const auto longestAxis = boundingBox.getLongestAxis();
    auto comparator = AABB::comparatorX;
    if (longestAxis == AABB::Axis::X)
    {
        comparator = AABB::comparatorX;
    }
    else if (longestAxis == AABB::Axis::Y)
    {
        comparator = AABB::comparatorY;
    }
    else
    {
        comparator = AABB::comparatorZ;
    }

    std::sort(triangles.begin(), triangles.end(), [comparator](const Triangle* a, const Triangle* b)
    {
        return comparator(a->boundingBox, b->boundingBox);
    });

    const size_t middleIndex = triangles.size() / 2;
    std::vector<Triangle*> l;
    std::vector<Triangle*> r;
    for (size_t i = 0; i < middleIndex; i++)
    {
        l.push_back(triangles[i]);
    }
    for (size_t i = middleIndex; i < triangles.size(); i++)
    {
        r.push_back(triangles[i]);
    }
    left = new BVH(l);
    right = new BVH(r);
}

std::vector<Triangle*> BVH::getPossibleIntersections(const Ray& ray, const Interval& lambdaRange)
{
    // TODO: null child case (!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)
    if (!boundingBox.intersectsRay(ray))
    {
        return {};
    }
    if (triangle != nullptr)
    {
        return {triangle};
    }
    std::vector<Triangle*> triangles;
    if (left->boundingBox.intersectsRay(ray))
    {
        for (Triangle* t : left->getPossibleIntersections(ray, lambdaRange))
        {
            triangles.push_back(t);
        }
    }
    if (right->boundingBox.intersectsRay(ray))
    {
        for (Triangle* t : right->getPossibleIntersections(ray, lambdaRange))
        {
            triangles.push_back(t);
        }
    }
    return triangles;
}
