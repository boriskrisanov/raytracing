#pragma once

#include <cmath>

struct Vector3
{
    double x;
    double y;
    double z;

    constexpr Vector3 operator+(Vector3 rhs) const
    {
        return {x + rhs.x, y + rhs.y, z + rhs.z};
    }

    constexpr void operator+=(Vector3 rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
    }

    Vector3 operator-(Vector3 rhs) const
    {
        return {x - rhs.x, y - rhs.y, z - rhs.z};
    }

    Vector3 operator*(double scalar) const
    {
        return {x * scalar, y * scalar, z * scalar};
    }

    Vector3 operator*(Vector3 v) const
    {
        return {x * v.x, y * v.y, z * v.z};
    }

    void operator*=(double scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
    }

    // Component multiplication
    void operator*=(Vector3 vector)
    {
        x *= vector.x;
        y *= vector.y;
        z *= vector.z;
    }

    Vector3 operator/(double scalar) const
    {
        return {x / scalar, y / scalar, z / scalar};
    }

    double magnitude() const
    {
        return std::sqrt(magnitudeSquared());
    }

    double magnitudeSquared() const
    {
        return x * x + y * y + z * z;
    }

    Vector3 normalised() const
    {
        return *this / magnitude();
    }

    bool isMagnitudeNearZero() const
    {
        const double epsilon = 1e-8;
        return magnitudeSquared() < epsilon;
    }

    Vector3 reflect(const Vector3& normal) const;
};

inline Vector3 operator*(double lhs, const Vector3& rhs)
{
    return rhs * lhs;
}

inline double dot(const Vector3& a, const Vector3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vector3 Vector3::reflect(const Vector3& normal) const
{
    return *this - 2*dot(*this, normal) * normal;
}


struct Interval
{
    const double min;
    const double max;

    bool contains(double val) const
    {
        return val >= min && val <= max;
    }
};