#include "Diffuse.hpp"

#include "random.hpp"
#include "SceneObject.hpp"

ScatteredRay Diffuse::scatter(const Ray& incidentRay, const HitInfo& hitInfo) const
{
    // Lambertian distribution
    Vector3 scatterDirection = hitInfo.normal + randomUnitVector();
    if (scatterDirection.isMagnitudeNearZero())
    {
        scatterDirection = hitInfo.normal;
    }

    // TODO: Normalisation might not be needed
    const auto scatteredRay = Ray{hitInfo.point, scatterDirection.normalised()};

    return {scatteredRay, 0.5 * color};
}
