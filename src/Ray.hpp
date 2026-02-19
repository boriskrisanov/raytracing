#pragma once

#include "math.hpp"

struct Ray
{
    Vector3 origin;
    Vector3 direction;

    Ray(const Vector3& origin, const Vector3& direction) : origin(origin), direction(direction.normalised())
    {
    }

    Vector3 at(double parameter) const
    {
        return origin + parameter * direction;
    }
};
