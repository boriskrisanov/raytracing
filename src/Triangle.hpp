#pragma once
#include "math.hpp"
#include "Ray.hpp"
#include "SceneObject.hpp"

class Triangle : public SceneObject
{
public:
    Triangle(const Vector3& p1, const Vector3& p2, const Vector3& p3, Material* material);
    RayIntersection intersects(const Ray& ray, Interval lambdaRange) const override;
    void translate(const Vector3& translation);
    void rotate(const Vector3& angles, const Vector3& origin);
private:
    Vector3 p1;
    Vector3 p2;
    Vector3 p3;
    Ray l1;
    Ray l2;
    Ray l3;
    Vector3 normal; // Needs to be before planeConstant because of initializer list order (otherwise normal vector is undefined and everything breaks)
};
