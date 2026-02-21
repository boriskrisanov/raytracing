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

std::optional<double> AABB::intersectsRay(const Ray& ray) const
{
    const Interval tx = axisIntersection(xRange, ray.origin.x, ray.direction.x);
    const Interval ty = axisIntersection(yRange, ray.origin.y, ray.direction.y);
    const Interval tz = axisIntersection(zRange, ray.origin.z, ray.direction.z);

    if (!(tx.overlaps(ty) && ty.overlaps(tz) && tx.overlaps(tz)))
    {
        return {};
    }
    // first t value within all three intersection ranges
    return {std::max(tx.getMin(), std::max(ty.getMin(), tz.getMin()))};
}

bool AABB::intersectsRayNearEdge(const Ray& ray, double epsilon) const
{
    const Interval tx = axisIntersection(xRange, ray.origin.x, ray.direction.x);
    const Interval ty = axisIntersection(yRange, ray.origin.y, ray.direction.y);
    const Interval tz = axisIntersection(zRange, ray.origin.z, ray.direction.z);

    return tx.overlaps(ty) && ty.overlaps(tz) && tx.overlaps(tz) && (tx.size() < epsilon || ty.size() < epsilon || tz.
        size() < epsilon);
}

void AABB::includePoint(const Vector3& point)
{
    xRange.include(point.x);
    yRange.include(point.y);
    zRange.include(point.z);
}

Interval AABB::operator[](int index) const
{
    if (index == 0) return xRange;
    if (index == 1) return yRange;
    return zRange;
}

bool AABB::operator==(const AABB& rhs) const
{
    return xRange == rhs.xRange && yRange == rhs.yRange && zRange == rhs.zRange;
}

int AABB::getLongestAxis() const
{
    if (xRange.size() >= yRange.size() && xRange.size() >= zRange.size())
    {
        return 0;
    }
    if (yRange.size() >= xRange.size() && yRange.size() >= zRange.size())
    {
        return 1;
    }
    if (zRange.size() >= xRange.size() && zRange.size() >= yRange.size())
    {
        return 2;
    }
    std::unreachable();
}
