#pragma once
#include "Material.hpp"

class Diffuse : public Material
{
public:
    Vector3 color = {1, 1, 1};

    [[nodiscard]] ScatteredRay scatter(const Ray& incidentRay, const HitInfo& hitInfo) const override;

    explicit Diffuse(const Vector3& color) : color{color}
    {
    }
};