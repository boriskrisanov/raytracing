#include "MeshInstance.hpp"

#include <iostream>

#include "Mesh.hpp"
#include "Triangle.hpp"

MeshInstance::MeshInstance(Material* material, Mesh* mesh, const Vector3& translation, const Vector3& rotation)
    : SceneObject(material),
      mesh(mesh),
      position(translation),
      rotation(rotation),
      boundingBox({})
{
    for (const Triangle& triangle : mesh->triangles)
    {
        boundingBox.includePoint(triangle.p1);
        boundingBox.includePoint(triangle.p2);
        boundingBox.includePoint(triangle.p3);
    }
}

RayIntersection MeshInstance::intersects(const Ray& ray, Interval lambdaRange) const
{
    // TODO: BVH

    if (!boundingBox.intersectsRay(ray))
    {
        return {};
    }

    RayIntersection closestHit{};
    // Apply translation then rotation (opposite order when going back to world space)
    Ray localRay{ray.origin - position, ray.direction};
    localRay.origin.rotate(rotation, {});
    localRay.direction.rotate(rotation, {});

    for (const Triangle& triangle : mesh->getTriangles())
    {
        RayIntersection hit = triangle.intersects(localRay, lambdaRange);
        if (hit.didHit)
        {
            hit.material = triangle.material;
            if (!closestHit.didHit || hit.rayParameter < closestHit.rayParameter - 1e-5)
            {
                closestHit = hit;
            }
        }
    }

    // Inverse linear transformation order
    closestHit.point.invertRotation(rotation, {});
    closestHit.normal.invertRotation(rotation, {});
    closestHit.point += position;
    closestHit.hitBoundingBox = true;
    // Lambda is unchanged because intersection point and origin are translated by the same amount

    return closestHit;
}
