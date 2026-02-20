#pragma once

#include "AABB.hpp"
#include "math.hpp"
#include "SceneObject.hpp"

class Triangle : public SceneObject
{
public:
    Triangle(const Vector3& p1, const Vector3& p2, const Vector3& p3, Material* material);
    RayIntersection intersects(const Ray& ray, Interval lambdaRange) const override;
    const Vector3 p1;
    const Vector3 p2;
    const Vector3 p3;
    // const AABB boundingBox;
    const Vector3 centre;
private:
    Vector3 normal;
};
