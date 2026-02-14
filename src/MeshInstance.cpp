#include "MeshInstance.hpp"

#include <iostream>

#include "Mesh.hpp"
#include "Triangle.hpp"

RayIntersection MeshInstance::intersects(const Ray& ray, Interval lambdaRange) const
{
    // TODO: BVH
    RayIntersection closestHit{};
    Ray localRay{ray.origin - position, ray.direction};
    localRay.origin.rotate(rotation, {});
    localRay.direction.rotate(rotation, {});
    for (const Triangle& triangle : mesh->getTriangles())
    {
        RayIntersection hit = triangle.intersects(localRay, lambdaRange);
        if (hit.didHit)
        {
            hit.material = triangle.material;
            if (!closestHit.didHit || hit.rayParameter < closestHit.rayParameter)
            {
                closestHit = hit;
            }
        }
    }

    closestHit.point.rotate(-1 * rotation, {});
    closestHit.normal.rotate(-1 * rotation, {});
    // Lambda is unchanged because intersection point and origin are translated by the same amount
    closestHit.point += position;

    return closestHit;
}
