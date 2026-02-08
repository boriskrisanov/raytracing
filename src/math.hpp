#pragma once

#include <cmath>

// TODO: Move definitions to cpp file

constexpr double pi = 3.14159265359;

inline double radians(double x)
{
    return x * pi / 180;
}

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

    void operator-=(Vector3 rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
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
        constexpr double epsilon = 1e-8;
        return magnitudeSquared() < epsilon;
    }

    void rotate(Vector3 degrees, Vector3 origin)
    {
        operator-=(origin);

        const double rx = radians(degrees.x);
        const double ry = radians(degrees.y);
        const double rz = radians(degrees.z);

        // x
        y = y * cos(rx) - z * sin(rx);
        z = y * sin(rx) + z * cos(rx);
        // y
        x = x * cos(ry) + z * sin(ry);
        z = z * cos(ry) - x * sin(ry);
        // z
        x = x * cos(rz) - y * sin(rz);
        y = x * sin(rz) + y * cos(rz);

        operator+=(origin);
    }
};

inline Vector3 operator*(double lhs, const Vector3& rhs)
{
    return rhs * lhs;
}

inline double dot(const Vector3& a, const Vector3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vector3 cross(const Vector3& a, const Vector3& b)
{
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
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

namespace fp_utils
{
    constexpr double epsilon = 1e-20; // Completely arbitrary (TODO: Set this properly)

    inline bool isZero(double val)
    {
        return std::abs(val) < epsilon;
    }

    inline bool equals(double lhs, double rhs)
    {
        return std::abs(lhs - rhs) < epsilon;
    }

    inline bool equals(const Vector3& lhs, const Vector3& rhs)
    {
        return std::abs(lhs.x - rhs.x) < epsilon && std::abs(lhs.y - rhs.y) < epsilon && std::abs(lhs.z - rhs.z) <
            epsilon;
    }
}
