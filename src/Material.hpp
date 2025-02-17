#pragma once

#include "math.hpp"

struct Material
{
    Vector3 color = {1, 1, 1};
    Vector3 emissionColor = {0, 0, 0};
    double emissionStrength = 0;
};
