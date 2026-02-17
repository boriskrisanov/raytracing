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
    for (Triangle& triangle : mesh->triangles)
    {
        boundingBox.includePoint(triangle.p1);
        boundingBox.includePoint(triangle.p2);
        boundingBox.includePoint(triangle.p3);
        trianglePointers.push_back(&triangle);
    }

    // bvh = new BVH{trianglePointers};
}

RayIntersection MeshInstance::intersects(const Ray& ray, Interval lambdaRange) const
{
    // TODO: BVH
    RayIntersection closestHit{};
    // Apply translation then rotation (opposite order when going back to world space)
    Ray localRay{ray.origin - position, ray.direction};
    localRay.origin.rotate(rotation, {});
    localRay.direction.rotate(rotation, {});

    if (!boundingBox.intersectsRay(localRay))
    {
        // if (ray.origin.y != 0)
        return {};
    }

    for (const Triangle& triangle : mesh->getTriangles())
    // std::cout << bvh->getPossibleIntersections(ray, lambdaRange).size() << "\n";
    // for (const Triangle* triangle : bvh->getPossibleIntersections(ray, lambdaRange))
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

    // if (!closestHit.didHit)
    // {
    //     boundingBox.intersectsRay(localRay);
    // }

    // Inverse linear transformation order
    closestHit.point.invertRotation(rotation, {});
    closestHit.normal.invertRotation(rotation, {});
    closestHit.point += position;
    closestHit.hitBoundingBox = true;
    // Lambda is unchanged because intersection point and origin are translated by the same amount

    return closestHit;
}
