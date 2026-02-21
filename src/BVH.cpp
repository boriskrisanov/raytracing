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

    std::ranges::sort(triangles, [&](const Triangle* a, const Triangle* b)
    {
        return a->centre[splitAxis] < b->centre[splitAxis];
    });

    std::vector<Triangle*> l;
    std::vector<Triangle*> r;

    // TODO: need to handle case of not being able ot split along longest axis (one child is empty) (or maybe not?)
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

    const std::optional<double> leftLambda = left->boundingBox.intersectsRay(ray);
    const std::optional<double> rightLambda = right->boundingBox.intersectsRay(ray);

    if (leftLambda.has_value() && rightLambda.has_value())
    {
        const BVH* child1 = leftLambda.value() < rightLambda.value() ? left : right;
        const BVH* child2 = child1 == right ? left : right;

        if (fp_utils::equals(leftLambda.value(), rightLambda.value()))
        {
            // Happens with fully overlapping (identical) bounding boxes (TODO: this shouldn't happen in the first place?)
            // Need to test both children

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

        if (const auto intersection1 = child1->findClosestIntersection(ray, lambdaRange); intersection1.didHit)
        {
            return intersection1;
        }
        return child2->findClosestIntersection(ray, lambdaRange);
    }
    if (leftLambda.has_value())
    {
        return left->findClosestIntersection(ray, lambdaRange);
    }
    if (rightLambda.has_value())
    {
        return right->findClosestIntersection(ray, lambdaRange);
    }
    return {};

    // if (!leftIntersection.didHit && !rightIntersection.didHit)
    // {
    //     return {};
    // }
    // if (leftIntersection.didHit && !rightIntersection.didHit)
    // {
    //     return leftIntersection;
    // }
    // if (rightIntersection.didHit && !leftIntersection.didHit)
    // {
    //     return rightIntersection;
    // }
    // return leftIntersection.rayParameter < rightIntersection.rayParameter
    //            ? leftIntersection
    //            : rightIntersection;
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
