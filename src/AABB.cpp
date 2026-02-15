#include "AABB.hpp"

#include <algorithm>

#include "Ray.hpp"

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
    if (fp_utils::isZero(rayDirectionAxisComponent))
    {
        return {-1e50, 1e50}; // No intersection point on this axis (interval is entire axis)
    }
    const double t0 = (axisRange.getMin() - rayOriginAxisComponent) / rayDirectionAxisComponent;
    const double t1 = (axisRange.getMax() - rayOriginAxisComponent) / rayDirectionAxisComponent;
    return {t0, t1};
}

bool AABB::intersectsRay(const Ray& ray) const
{
    // double overlapMin = -1e50;
    // double overlapMax = 1e50;

    const Interval tx = axisIntersection(xRange, ray.origin.x, ray.direction.x);
    // overlapMin = std::max(overlapMin, tx.getMin());
    // overlapMax = std::min(overlapMax, tx.getMax());
    //
    // if (overlapMax < overlapMin) return false;

    const Interval ty = axisIntersection(yRange, ray.origin.y, ray.direction.y);
    // overlapMin = std::max(overlapMin, ty.getMin());
    // overlapMax = std::min(overlapMax, ty.getMax());
    //
    // if (overlapMax < overlapMin) return false;

    const Interval tz = axisIntersection(zRange, ray.origin.z, ray.direction.z);
    // overlapMin = std::max(overlapMin, tz.getMin());
    // overlapMax = std::min(overlapMax, tz.getMax());
    //
    // if (overlapMax < overlapMin) return false;
    //
    // return true;

    return tx.overlaps(ty) && ty.overlaps(tz) && tx.overlaps(tz);
}

void AABB::includePoint(const Vector3& point)
{
    xRange.include(point.x);
    yRange.include(point.y);
    zRange.include(point.z);
}
