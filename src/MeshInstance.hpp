#pragma once
#include "math.hpp"
#include "SceneObject.hpp"

class Mesh;

struct MeshInstance : SceneObject
{
    Mesh *mesh;
    Vector3 position;
    Vector3 rotation;

    MeshInstance(Material* material, Mesh* mesh, const Vector3& translation, const Vector3& rotation)
        : SceneObject(material),
          mesh(mesh),
          position(translation),
          rotation(rotation)
    {
    }

    RayIntersection intersects(const Ray& ray, Interval lambdaRange) const override;
};
