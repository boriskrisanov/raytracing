#pragma once

#include "math.hpp"
#include "Ray.hpp"
#include <optional>

class Random;
struct RayIntersection;

struct ScatteredRay
{
    Ray ray;
    Vector3 color;
};

struct Material
{
    virtual ~Material() = default;
    [[nodiscard]] virtual std::optional<ScatteredRay> scatter(const Ray& incidentRay, const RayIntersection& hitInfo, Random& rng) const
    {
        return std::nullopt;
    }

    [[nodiscard]] virtual Vector3 emit() const
    {
        return {0, 0, 0};
    }
};
