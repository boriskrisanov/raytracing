#include "Triangle.hpp"

#include <algorithm>

using std::min, std::max;

constexpr auto boundingBoxTolerance = Vector3{1e-10, 1e-10, 1e-10};

RayIntersection Triangle::intersects(const Ray& ray, Interval lambdaRange) const
{
    const double rayDotNormal = dot(ray.direction, normal);
    // Check plane intersection
    if (fp_utils::isZero(rayDotNormal))
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


    // int intersectionCount = 0;
    //
    // if (edgeIntersects(center, point, l1)) intersectionCount++;
    // if (edgeIntersects(center, point, l2)) intersectionCount++;
    // if (edgeIntersects(center, point, l3)) intersectionCount++;
    //
    // if (intersectionCount % 2 == 0)
    // {
    //     // Inside bounding box but outside triangle
    //     return {};
    // }
    //
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

Triangle::Triangle(const Vector3& p1, const Vector3& p2, const Vector3& p3, Material* material)
    : SceneObject(material),
      p1(p1), p2(p2), p3(p3),
      l1{p1, p2 - p1}, l2{p2, p3 - p2}, l3{p1, p1 - p3},
      minPoint{
          min(p1.x, min(p2.x, p3.x)),
          min(p1.y, min(p2.y, p3.y)),
          min(p1.z, min(p2.z, p3.z))
      },
      maxPoint{
          max(p1.x, max(p2.x, p3.x)),
          max(p1.y, max(p2.y, p3.y)),
          max(p1.z, max(p2.z, p3.z))
      },
      normal(cross(l1.direction, l2.direction).normalised()),
      planeConstant(dot(p1, normal)),
      center((p1 + p2 + p3) / 3)
{
}

bool Triangle::edgeIntersects(const Vector3& R, const Vector3& T, const Ray& edge) const
{
    const Vector3 d = R - T;
    const Vector3 v = edge.direction;
    const Vector3 p = edge.origin;

    if (fp_utils::equals(R, T))
    {
        return true;
    }

    // Solve for lambda and mu using the first two equations
    const double determinant = d.x * v.y - v.x * d.y;
    if (fp_utils::isZero(determinant))
    {
        return false;
    }

    // clang-format off
    const double lambda = (v.x * (T.y - p.y) - v.y * (T.x - p.x)) / determinant;
    const double mu     = (d.x * (T.y - p.y) - d.y * (T.x - p.x)) / determinant;
    // clang-format on

    if (lambda < 0) return false;

    // Check if satisfies third equation
    const double lhs = v.z * mu - d.z * lambda;
    const double rhs = T.z - p.z;

    if (!fp_utils::equals(lhs, rhs))
    {
        return false;
    }

    // Make sure that the intersection point (with edge) is still within the bounding box (edge lines continue past it)
    const Vector3 point = T + lambda * d;
    return point > (minPoint - boundingBoxTolerance) && point < (maxPoint + boundingBoxTolerance);
}
