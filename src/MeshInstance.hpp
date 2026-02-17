#pragma once
#include "AABB.hpp"
#include "BVH.hpp"
#include "math.hpp"
#include "SceneObject.hpp"

class Mesh;

struct MeshInstance : SceneObject
{
    Mesh *mesh;
    Vector3 position;
    Vector3 rotation;
    AABB boundingBox;
    BVH* bvh;

    MeshInstance(Material* material, Mesh* mesh, const Vector3& translation, const Vector3& rotation);

    RayIntersection intersects(const Ray& ray, Interval lambdaRange) const override;
private:
    std::vector<Triangle*> trianglePointers;
};
