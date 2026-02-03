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
    virtual ~Material() = default;
    [[nodiscard]] virtual ScatteredRay scatter(const Ray& incidentRay, const HitInfo& hitInfo) const = 0;
};
