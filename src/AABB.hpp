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

private:
    Interval xRange;
    Interval yRange;
    Interval zRange;
};
