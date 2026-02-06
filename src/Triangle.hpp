#pragma once
#include "math.hpp"
#include "Ray.hpp"
#include "SceneObject.hpp"

class Triangle : public SceneObject
{
public:
    Triangle(const Vector3& p1, const Vector3& p2, const Vector3& p3, Material* material);
    RayIntersection intersects(const Ray& ray, Interval lambdaRange) const override;
    const Vector3 p1;
    const Vector3 p2;
    const Vector3 p3;
private:
    const Ray l1;
    const Ray l2;
    const Ray l3;
    const Vector3 minPoint;
    const Vector3 maxPoint;
    const Vector3 normal; // Needs to be before planeConstant because of initializer list order (otherwise normal vector is undefined and everything breaks)
    const double planeConstant;
    const Vector3 center;
    bool edgeIntersects(const Vector3& R, const Vector3& T, const Ray& edge) const;
};
