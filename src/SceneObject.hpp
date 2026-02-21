#pragma once

#include "math.hpp"
#include "SceneObject.hpp"

struct Ray;
struct Material;

// TODO: Create full constructor to prevent bugs
struct RayIntersection
{
    Vector3 point{};
    Vector3 normal{};
    double rayParameter = 0;
    bool didHit = false;
    Material* material{};
};

class SceneObject
{
public:
    virtual ~SceneObject() = default;
    explicit SceneObject(Material* material) : material{material} {}
    virtual RayIntersection intersects(Ray ray, const Interval& lambdaRange) const = 0;
    Material* material;
};
