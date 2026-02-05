#pragma once

#include <optional>
#include "Material.hpp"

class Diffuse : public Material
{
public:
    Vector3 color = {1, 1, 1};

    [[nodiscard]] std::optional<ScatteredRay> scatter(const Ray& incidentRay, const RayIntersection& hitInfo) const override;

    explicit Diffuse(const Vector3& color) : color{color}
    {
    }
};
