#pragma once

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
private:
    Vector3 normal; // Needs to be before planeConstant because of initializer list order (otherwise normal vector is undefined and everything breaks)
};
