#pragma once

#include "math.hpp"
#include "Ray.hpp"
#include <optional>

struct HitInfo;

struct ScatteredRay
{
    Ray ray;
    Vector3 color;
};

struct Material
{
    virtual ~Material() = default;
    [[nodiscard]] virtual std::optional<ScatteredRay> scatter(const Ray& incidentRay, const HitInfo& hitInfo) const = 0;

    [[nodiscard]] virtual Vector3 emit() const
    {
        return {0, 0, 0};
    }
};
