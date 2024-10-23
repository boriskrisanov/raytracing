#pragma once

#include <array>

struct Vector3
{
    double x;
    double y;
    double z;

    constexpr Vector3 operator+(Vector3 rhs) const
    {
        return {x + rhs.x, y + rhs.y, z + rhs.z};
    }

    Vector3 operator-(Vector3 rhs) const
    {
        return {x - rhs.x, y - rhs.y, z - rhs.z};
    }

    Vector3 operator*(double scalar) const
    {
        return {x * scalar, y * scalar, z * scalar};
    }

    Vector3 operator/(double scalar) const
    {
        return {x / scalar, y / scalar, z / scalar};
    }

    Vector3 normalised() const
    {
        return *this / std::sqrt(x * x + y * y + z * z);
    }
};

inline Vector3 operator*(double lhs, const Vector3& rhs)
{
    return rhs * lhs;
}
