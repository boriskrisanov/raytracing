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

    // TODO: Make this a material parameter
    if (randomDouble(0, 1) < 0.3)
    {
        return {};
    }
    return ScatteredRay{scatteredRay, color};

    // TODO: Compare
    // return ScatteredRay{scatteredRay, 0.5 * color};
}
