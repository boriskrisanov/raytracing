#include "Diffuse.hpp"
#include "random.hpp"
#include "SceneObject.hpp"

std::optional<ScatteredRay> Diffuse::scatter(const Ray& incidentRay, const HitInfo& hitInfo) const
{
    // Lambertian distribution
    Vector3 scatterDirection = hitInfo.normal + randomUnitVector();
    if (scatterDirection.isMagnitudeNearZero())
    {
        scatterDirection = hitInfo.normal;
    }

    const auto scatteredRay = Ray{hitInfo.point, scatterDirection};

    return ScatteredRay{scatteredRay, 0.5 * color};
}
