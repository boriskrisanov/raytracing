#pragma once

#include "AABB.hpp"
#include "math.hpp"
#include "Ray.hpp"
#include "SceneObject.hpp"

class Triangle : public SceneObject
{
public:
    Triangle(const Vector3& p1, const Vector3& p2, const Vector3& p3, Material* material);
    RayIntersection intersects(const Ray& ray, Interval lambdaRange) const override;
    Vector3 p1;
    Vector3 p2;
    Vector3 p3;
    AABB boundingBox;
private:
    Vector3 normal;
};
