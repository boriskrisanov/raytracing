#pragma once

#include "math.hpp"
#include "Ray.hpp"

struct HitInfo;

struct ScatteredRay
{
    Ray ray;
    Vector3 color;
};

struct Material
{
    Vector3 color = {1, 1, 1};
    Vector3 emissionColor = {0, 0, 0};
    double emissionStrength = 0;

    ScatteredRay scatter(const Ray& incidentRay, const HitInfo& hitInfo) const;
};
