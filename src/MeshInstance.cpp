#include "MeshInstance.hpp"

#include <algorithm>

#include "Mesh.hpp"
#include "Triangle.hpp"

RayIntersection MeshInstance::intersects(const Ray& ray, Interval lambdaRange) const
{
    // TODO: BVH
    RayIntersection closestHit{};
    for (const Triangle triangle : mesh->getTriangles())
    {
        RayIntersection hit = triangle.intersects({ray.origin - position, ray.direction}, lambdaRange);
        if (hit.didHit)
        {
            hit.material = triangle.material;
            if (!closestHit.didHit || hit.rayParameter < closestHit.rayParameter)
            {
                closestHit = hit;
            }
        }
    }
    return closestHit;
}
