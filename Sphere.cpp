#include "Sphere.hpp"

bool Sphere::intersects(const Ray& ray) const
{
    // Magnitude squared is equivalent to dotting a vector with itself
    // TODO: Write proper comments
    Vector3 rayToCenter = center - ray.origin;
    double a = ray.direction.magnitudeSquared();
    double h = dot(ray.direction, rayToCenter);
    double c = rayToCenter.magnitudeSquared() - radius * radius;
    double discriminant = h * h - a * c;
    return discriminant >= 0;
}
