#pragma once

#include "math.hpp"

struct Ray
{
    Vector3 origin;
    Vector3 direction;

    Vector3 at(double parameter) const
    {
        return origin + parameter * direction;
    }
};
