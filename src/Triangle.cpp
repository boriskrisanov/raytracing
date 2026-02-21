#include "Triangle.hpp"

#include <algorithm>
#include "Ray.hpp"

using std::min, std::max;

RayIntersection Triangle::intersects(const Ray& ray, Interval lambdaRange) const
{
    // Check plane intersection
    if (fp_utils::isZero(dot(ray.direction, normalCross)))
    {
        // Ray is parallel to triangle, no intersection
        return {};
    }

    const auto d = ray.direction;
    const auto r = ray.origin;

    // const auto [lambda, mu, t] = solve3Unknowns(a, b, -d, r - p1);
    // Idea for determinant computation with triple product from https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection.html
    // TODO: Derivation/explanation in comments

    const auto x = r - p1;
    const double detReciprocal = 1 / dot(-d, normalCross);

    const double detLambda = dot(-d, cross(x, b));
    const double lambda = detLambda * detReciprocal;
    if (lambda < 0 || lambda > 1) return {};

    const double detMu = dot(-d, cross(a, x));
    const double mu = detMu * detReciprocal;
    if (mu < 0 || mu > 1) return {};

    const double detT = dot(x, normalCross);
    const double t = detT * detReciprocal;

    if (lambda + mu > 1 || !lambdaRange.contains(t)) return {};

    // Roughly equal in performance (TODO: Benchmark further)

    // Matrix3 matrix;
    // matrix.setColumn(0, a);
    // matrix.setColumn(1, b);
    // matrix.setColumn(2, -d);
    //
    // matrix.setColumn(0, x);
    // const double lambda = matrix.det() * detReciprocal;
    // if (lambda < 0 || lambda > 1) return {};
    // matrix.setColumn(0, a);
    //
    // matrix.setColumn(1, x);
    // const double mu = matrix.det() * detReciprocal;
    // if (mu < 0 || mu > 1) return {};
    // matrix.setColumn(1, b);
    //
    // matrix.setColumn(2, x);
    // const double t = matrix.det() * detReciprocal;
    //
    // if (lambda + mu > 1 || !lambdaRange.contains(t)) return {};

    RayIntersection intersection;
    intersection.didHit = true;
    intersection.point = ray.at(t);
    intersection.rayParameter = t;
    intersection.material = material;
    intersection.normal = normalisedNormal;
    if (dot(intersection.normal, ray.direction) > 0)
    {
        // Normal should point towards oncoming ray
        intersection.normal *= -1;
    }
    return intersection;
}

Triangle::Triangle(const Vector3& p1, const Vector3& p2, const Vector3& p3, Material* material)
    : SceneObject(material),
      p1(p1), p2(p2), p3(p3),
      // boundingBox({p1, p2, p3}),
      centre((p1 + p2 + p3) / 3),
      normalCross(cross(p2 - p1, p3 - p1)),
      normalisedNormal(normalCross.normalised()),
      a(p2 - p1), b(p3 - p1)
{
}
