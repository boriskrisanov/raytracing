#pragma once

#include "math.hpp"
#include "Ray.hpp"
#include "SceneObject.hpp"

class Sphere : public SceneObject
{
public:
    Sphere(const Vector3& center, double radius, const Material& material)
        : SceneObject(material), center(center), radius(radius)
    {
    }

    HitInfo intersects(const Ray& ray, Interval lambdaRange) const override;

    Vector3 center;
    double radius;
};
