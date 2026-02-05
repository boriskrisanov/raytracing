#pragma once

#include "Material.hpp"

class Reflective : public Material
{
public:
    Vector3 color;
    double roughness = 0;

    [[nodiscard]] std::optional<ScatteredRay> scatter(const Ray& incidentRay, const RayIntersection& hitInfo) const override;

    explicit Reflective(const Vector3& color) : color{color}
    {

    }

    Reflective(const Vector3& color, double roughness) : color{color}, roughness{roughness}
    {

    }
};
