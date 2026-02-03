#include "Reflective.hpp"

#include "SceneObject.hpp"

ScatteredRay Reflective::scatter(const Ray& incidentRay, const HitInfo& hitInfo) const
{
    // Reflect incident ray in normal
    Vector3 reflected = incidentRay.direction - 2 * hitInfo.normal * dot(incidentRay.direction, hitInfo.normal);
    const auto reflectedRay = Ray{hitInfo.point, reflected.normalised()};

    return {reflectedRay, color};
}
