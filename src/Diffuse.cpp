#include "Diffuse.hpp"

#include <complex>

#include "random.hpp"
#include "SceneObject.hpp"

std::optional<ScatteredRay> Diffuse::scatter(const Ray& incidentRay, const RayIntersection& hitInfo) const
{
    // Lambertian distribution
    Vector3 scatterDirection = hitInfo.normal + randomUnitVector();
    if (scatterDirection.isMagnitudeNearZero())
    {
        scatterDirection = hitInfo.normal;
    }

    const auto scatteredRay = Ray{hitInfo.point, scatterDirection};

    // TODO: Use scatter/absorption probability for attenuation
    return ScatteredRay{scatteredRay, 0.5 * color};
}
