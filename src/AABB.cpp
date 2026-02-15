#include "AABB.hpp"

#include <algorithm>

#include "Ray.hpp"

// Completely arbitrary to account for floating imprecision (TODO: Set this properly or determine if even needed)
constexpr double tolerance = 1e-5;

AABB::AABB(const std::vector<Vector3>& initialPoints) : xRange(0, 0), yRange(0, 0), zRange(0, 0)
{
    for (const Vector3& point : initialPoints)
    {
        includePoint(point);
    }
}

bool AABB::includes(const Vector3& point) const
{
    return xRange.contains(point.x) && yRange.contains(point.y) && zRange.contains(point.z);
}

Interval axisIntersection(const Interval& axisRange, double rayOriginAxisComponent, double rayDirectionAxisComponent)
{
    // min is always >= max, so t0 >= t1
    if (fp_utils::isZero(rayDirectionAxisComponent))
    {
        return {0, 0};
    }
    const double t0 = (axisRange.getMin() - rayOriginAxisComponent) / rayDirectionAxisComponent;
    const double t1 = (axisRange.getMax() - rayOriginAxisComponent) / rayDirectionAxisComponent;
    return {t0, t1};
}

bool AABB::intersectsRay(const Ray& ray) const
{
    // The empty checks aren't needed since the intersection will be empty later anyway, they are just to early return
    //  for performance (compiler probably won't optimise this TODO: Benchmark)
    const Interval tx = axisIntersection(xRange, ray.origin.x, ray.direction.x);
    if (tx.isEmpty())
    {
        return false;
    }
    const Interval ty = axisIntersection(yRange, ray.origin.y, ray.direction.y);
    if (ty.isEmpty())
    {
        return false;
    }
    const Interval tz = axisIntersection(zRange, ray.origin.z, ray.direction.z);
    if (tz.isEmpty())
    {
        return false;
    }

    return !tx.getIntersectionWith(ty).getIntersectionWith(tz).isEmpty();
}

void AABB::includePoint(const Vector3& point)
{
    xRange.include(point.x);
    yRange.include(point.y);
    zRange.include(point.z);
}
