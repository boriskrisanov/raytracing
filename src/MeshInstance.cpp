#include "MeshInstance.hpp"

#include <iostream>

#include "Mesh.hpp"
#include "Triangle.hpp"

RayIntersection MeshInstance::intersects(const Ray& ray, Interval lambdaRange) const
{
    // TODO: BVH
    RayIntersection closestHit{};
    // Apply translation then rotation (order reversed when going back to world space)
    Ray localRay{ray.origin - position, ray.direction};
    localRay.origin.rotate(rotation, {});
    localRay.direction.rotate(rotation, {});

    for (const Triangle& triangle : mesh->getTriangles())
    {
        RayIntersection hit = triangle.intersects(localRay, lambdaRange);
        if (hit.didHit)
        {
            hit.material = triangle.material;
            // hit = triangle.intersects(localRay, lambdaRange);
            // TODO: Epsilon
            bool closer = hit.rayParameter < closestHit.rayParameter;
            // bool closer = (localRay.origin - hit.point).magnitude() < (localRay.origin - closestHit.point).magnitude();
            if (!closestHit.didHit || closer)
            {
                closestHit = hit;
            }
        }
    }

    // Must be done first (reverse order that it was applied in initially because this is a linear transformation => matrix multiplication)
    closestHit.point += position;

    closestHit.point.rotate(-rotation, {});
    closestHit.normal.rotate(-rotation, {});
    // Lambda is unchanged because intersection point and origin are translated by the same amount

    return closestHit;
}
