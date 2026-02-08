#include "Triangle.hpp"

#include <algorithm>

using std::min, std::max;

constexpr auto boundingBoxTolerance = Vector3{1e-10, 1e-10, 1e-10};

RayIntersection Triangle::intersects(const Ray& ray, Interval lambdaRange) const
{
    // Check plane intersection
    if (fp_utils::isZero(dot(ray.direction, normal)))
    {
        // Ray is parallel to triangle, no intersection
        return {};
    }

    // TODO: This might be faster (instead of computing t with Cramer's rule), need to benchmark
    // const double t = (planeConstant - dot(ray.origin, normal)) / rayDotNormal;
    // const Vector3 point = ray.at(t);
    // if (point.x > maxPoint.x || point.y > maxPoint.y || point.z > maxPoint.z || point.x < minPoint.x || point.y <
    //     minPoint.y || point.z < minPoint.z)
    // {
    //     // Intersects plane containing the triangle outside the smallest bounding box containing the triangle
    //     //  (=> doesn't intersect triangle)
    //     return {};
    // }

    // Möller–Trumbore algorithm?

    const auto [a1, a2, a3] = p2 - p1;
    const auto [b1, b2, b3] = p3 - p1;
    const auto [d1, d2, d3] = ray.direction;
    // Triangle origin is p1
    const double x1 = ray.origin.x - p1.x;
    const double x2 = ray.origin.y - p1.y;
    const double x3 = ray.origin.z - p1.z;
    const double det = a1 * (b2 * d3 - d2 * a3) - b1 * (a2 * d3 - d2 * a3) + d1 * (a2 * b3 - b2 * a3);
    const double detLambda = x1 * (b2 * d3 - d2 * a3) - b1 * (x2 * d3 - d2 * x3) + d1 * (x2 * b3 - b2 * x3);
    const double detMu = a1 * (x2 * d3 - d2 * x3) - x1 * (a2 * d3 - d2 * a3) + d1 * (a2 * x3 - x2 * a3);
    const double detT = a1 * (b2 * x3 - x2 * b3) - b1 * (a2 * x3 - x2 * a3) + x1 * (a2 * b3 - b2 * a3);
    const double lambda = detLambda / det;
    const double mu = detMu / det;
    const double t = detT / det;

    if (lambda <= 0 || mu <= 0 || lambda + mu > 1)
    {
        return {};
    }

    RayIntersection intersection;
    intersection.didHit = true;
    intersection.rayParameter = t;
    intersection.material = material;
    intersection.normal = normal;
    if (dot(intersection.normal, ray.direction) < 0)
    {
        // Normal should point towards oncoming ray
        intersection.normal *= -1;
    }
    return intersection;
}

void Triangle::translate(const Vector3& translation)
{
    p1 += translation;
    p2 += translation;
    p3 += translation;
    l1.origin += translation;
    l2.origin += translation;
    l3.origin += translation;
}

Triangle::Triangle(const Vector3& p1, const Vector3& p2, const Vector3& p3, Material* material)
    : SceneObject(material),
      p1(p1), p2(p2), p3(p3),
      l1{p1, p2 - p1}, l2{p2, p3 - p2}, l3{p1, p1 - p3},
      normal(cross(l1.direction, l2.direction).normalised()),
      planeConstant(dot(p1, normal))
{
}
