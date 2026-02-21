#pragma once

#include "math.hpp"
#include <optional>
#include <vector>

struct Ray;

class AABB
{
public:
    explicit AABB(const std::vector<Vector3>& initialPoints);
    bool includes(const Vector3& point) const;
    // Returns ray parameter of first intersection if intersects, empty optional if misses
    std::optional<double> intersectsRay(const Ray& ray) const;
    bool intersectsRayNearEdge(const Ray& ray, double epsilon) const;
    void includePoint(const Vector3& point);

    Interval operator[](int index) const;

    enum class Axis
    {
        X = 0,
        Y = 1,
        Z = 2
    };

    int getLongestAxis() const;
private:
    Interval xRange;
    Interval yRange;
    Interval zRange;
};
