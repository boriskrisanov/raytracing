#pragma once
#include "Material.hpp"

class Reflective : public Material
{
public:
    Vector3 color;
    [[nodiscard]] ScatteredRay scatter(const Ray& incidentRay, const HitInfo& hitInfo) const override;

    explicit Reflective(const Vector3& color) : color{color}
    {

    }
};
