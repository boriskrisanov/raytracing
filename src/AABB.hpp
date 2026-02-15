#pragma once

#include "math.hpp"
#include <array>
#include <vector>

struct Ray;

class AABB
{
public:
    explicit AABB(const std::vector<Vector3>& initialPoints);
    bool includes(const Vector3& point) const;
    bool intersectsRay(const Ray& ray) const;
    void includePoint(const Vector3& point);

    static bool comparatorX(const AABB& a, const AABB& b);
    static bool comparatorY(const AABB& a, const AABB& b);
    static bool comparatorZ(const AABB& a, const AABB& b);

    enum class Axis
    {
        X,
        Y,
        Z
    };

    Axis getLongestAxis() const;
private:
    Interval xRange;
    Interval yRange;
    Interval zRange;
};
