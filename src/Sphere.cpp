#include "Sphere.hpp"

RayIntersection Sphere::intersects(Ray ray, const Interval& lambdaRange) const
{
    RayIntersection hitRecord{};

    // Magnitude squared is equivalent to dotting a vector with itself
    // TODO: Use improved method with b = -2h

    const double a = ray.direction.magnitudeSquared();
    const double b = -2 * dot(ray.direction, center - ray.origin);
    const double c = (center - ray.origin).magnitudeSquared() - radius * radius;
    const double discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
    {
        // didHit is false
        return hitRecord;
    }

    const double root1 = (-b + sqrt(discriminant)) / (2 * a);
    const double root2 = (-b - sqrt(discriminant)) / (2 * a);
    double lambda;
    if (lambdaRange.contains(std::fmin(root1, root2)))
    {
        lambda = std::fmin(root1, root2);
    } else if (lambdaRange.contains(std::fmax(root1, root2)))
    {
        lambda = std::fmax(root1, root2);
    }
    else
    {
        return hitRecord;
    }

    hitRecord.point = ray.at(lambda);
    hitRecord.rayParameter = lambda;
    hitRecord.normal = (hitRecord.point - center).normalised();
    hitRecord.didHit = true;

    // if (dot(ray.direction, hitRecord.normal) > 0)
    // {
    //     hitRecord.normal *= -1;
    // }

    return hitRecord;
}
