#include "BVH.hpp"

#include <algorithm>
#include <functional>

#include "AABB.hpp"
#include "AABB.hpp"
#include "AABB.hpp"
#include "AABB.hpp"
#include "random.hpp"
#include "Ray.hpp"
#include "Triangle.hpp"


// TODO: This is incredibly inefficient with vector copies
BVH::BVH(std::vector<Triangle*> triangles, int depth)
    : leafTriangles({}), left(nullptr), right(nullptr), boundingBox({})
{
    // Create bounding box
    for (const Triangle* const t : triangles)
    {
        boundingBox.includePoint(t->p1);
        boundingBox.includePoint(t->p2);
        boundingBox.includePoint(t->p3);
    }

    if (depth >= maxDepth || triangles.size() <= 2)
    {
        leafTriangles = triangles;
        return;
    }

    const auto splitAxis = boundingBox.getLongestAxis();
    // const auto splitAxis = randomInt(0, 2) == 1 ? AABB::Axis::X : randomInt(0, 1) == 0 ? AABB::Axis::Y : AABB::Axis::Z;

    std::ranges::sort(triangles, [&](const Triangle* a, const Triangle* b)
    {
        return a->centre[splitAxis] < b->centre[splitAxis];
    });

    std::vector<Triangle*> l;
    std::vector<Triangle*> r;

    const size_t middleIndex = triangles.size() / 2;
    for (size_t i = 0; i < middleIndex; i++)
    {
        l.push_back(triangles[i]);
    }
    for (size_t i = middleIndex; i < triangles.size(); i++)
    {
        r.push_back(triangles[i]);
    }

    left = new BVH(l, depth + 1);
    right = new BVH(r, depth + 1);
}

RayIntersection BVH::findClosestIntersection(const Ray& ray, const Interval& lambdaRange) const
{
    // RayIntersection closestHit{};
    //
    // if (leafTriangles.size() > 0)
    // {
    //     for (const Triangle* triangle : leafTriangles)
    //     {
    //         RayIntersection hit = triangle->intersects(ray, lambdaRange);
    //         if (hit.didHit)
    //         {
    //             hit.material = triangle->material;
    //             if (!closestHit.didHit || hit.rayParameter < closestHit.rayParameter - 1e-8)
    //             {
    //                 closestHit = hit;
    //             }
    //         }
    //     }
    //
    //     return closestHit;
    // }

    // TODO: Don't use recursion for performance?
    if (!boundingBox.intersectsRay(ray))
    {
        return {};
    }
    if (left == nullptr || right == nullptr)
    {
        // Leaf node
        RayIntersection closestHit{};

        for (const Triangle* triangle : leafTriangles)
        {
            RayIntersection hit = triangle->intersects(ray, lambdaRange);
            if (hit.didHit)
            {
                hit.material = triangle->material;
                if (!closestHit.didHit || hit.rayParameter < closestHit.rayParameter - 1e-8)
                {
                    closestHit = hit;
                }
            }
        }

        return closestHit;
    }

    // TODO: Search order by distance to ray origin?

    const auto leftIntersection = left->findClosestIntersection(ray, lambdaRange);
    const auto rightIntersection = right->findClosestIntersection(ray, lambdaRange);
    if (!leftIntersection.didHit && !rightIntersection.didHit)
    {
        return {};
    }
    if (leftIntersection.didHit && !rightIntersection.didHit)
    {
        return leftIntersection;
    }
    if (rightIntersection.didHit && !leftIntersection.didHit)
    {
        return rightIntersection;
    }
    return leftIntersection.rayParameter < rightIntersection.rayParameter
               ? leftIntersection
               : rightIntersection;
}

const BVH* BVH::getLeft() const
{
    return left;
}

const BVH* BVH::getRight() const
{
    return right;
}

AABB BVH::getBoundingBox() const
{
    return boundingBox;
}
