#pragma once

#include "src/math.hpp"
#include "src/Ray.hpp"

struct Sphere
{
public:
    Sphere(const Vector3& center, double radius)
        : center(center), radius(radius)
    {
    }

    bool intersects(const Ray& ray) const;

    Vector3 center;
    double radius;
};
