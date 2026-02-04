#pragma once

#include <optional>

#include "Material.hpp"

class Emissive : public Material
{
public:
    Vector3 emissionColor;
    double emissionStrength;
    [[nodiscard]] std::optional<ScatteredRay> scatter(const Ray& incidentRay, const HitInfo& hitInfo) const override;
    [[nodiscard]] Vector3 emit() const override;
};
