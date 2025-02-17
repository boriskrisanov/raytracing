#pragma once

#include "Material.hpp"
#include "math.hpp"

class Ray;

struct HitInfo
{
    Vector3 point{};
    Vector3 normal{};
    double rayParameter = 0;
    bool didHit = false;
    Material material{};
};

class SceneObject
{
public:
    virtual ~SceneObject() = default;
    explicit SceneObject(Material material) : material{material} {};
    virtual HitInfo intersects(const Ray& ray, Interval lambdaRange) const = 0;
    Material material;
};
