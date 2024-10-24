#include "Sphere.hpp"

bool Sphere::intersects(const Ray& ray) const
{
    Vector3 v = center - ray.origin;
    double a = dot(ray.direction, ray.direction);
    double b = -2.0 * dot(ray.direction, v);
    double c = dot(v, v) - radius * radius;
    double discriminant = b * b - 4 * a * c;
    return discriminant >= 0;
}
