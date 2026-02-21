#pragma once

#include "math.hpp"
#include "Ray.hpp"
#include "SceneObject.hpp"

class Sphere : public SceneObject
{
public:
    Sphere(const Vector3& center, double radius, Material* material)
        : SceneObject(material), center(center), radius(radius)
    {
    }

    RayIntersection intersects(Ray ray, const Interval& lambdaRange) const override;

    Vector3 center;
    double radius;
};
