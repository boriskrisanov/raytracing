#pragma once

#include <optional>

#include "Material.hpp"

class Emissive : public Material
{
public:
    Vector3 emissionColor;
    double emissionStrength;
    [[nodiscard]] Vector3 emit() const override;

    Emissive(const Vector3& emission_color, double emission_strength)
        : emissionColor(emission_color),
          emissionStrength(emission_strength)
    {
    }
};
