#include "Reflective.hpp"

#include "random.hpp"
#include "SceneObject.hpp"

std::optional<ScatteredRay> Reflective::scatter(const Ray& incidentRay, const HitInfo& hitInfo) const
{
    // Reflect incident ray in normal
    Vector3 reflected = incidentRay.direction - 2 * hitInfo.normal * dot(incidentRay.direction, hitInfo.normal);

    const Vector3 offsetVector = randomUnitVector() * roughness;
    const auto reflectedRay = Ray{hitInfo.point, reflected.normalised() + offsetVector};

    return ScatteredRay{reflectedRay, color};
}
