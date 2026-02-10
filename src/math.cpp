#include "math.hpp"

double radians(double x)
{
    return x * pi / 180;
}

Vector3 Vector3::operator+(Vector3 rhs) const
{
    return {x + rhs.x, y + rhs.y, z + rhs.z};
}

void Vector3::operator+=(Vector3 rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
}

Vector3 Vector3::operator-(Vector3 rhs) const
{
    return {x - rhs.x, y - rhs.y, z - rhs.z};
}

void Vector3::operator-=(Vector3 rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
}

Vector3 Vector3::operator*(double scalar) const
{
    return {x * scalar, y * scalar, z * scalar};
}

Vector3 Vector3::operator*(Vector3 v) const
{
    return {x * v.x, y * v.y, z * v.z};
}

void Vector3::operator*=(double scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
}

void Vector3::operator*=(Vector3 vector)
{
    x *= vector.x;
    y *= vector.y;
    z *= vector.z;
}

Vector3 Vector3::operator/(double scalar) const
{
    return {x / scalar, y / scalar, z / scalar};
}

double Vector3::magnitude() const
{
    return std::sqrt(magnitudeSquared());
}

double Vector3::magnitudeSquared() const
{
    return x * x + y * y + z * z;
}

Vector3 Vector3::normalised() const
{
    return *this / magnitude();
}

bool Vector3::isMagnitudeNearZero() const
{
    constexpr double epsilon = 1e-8;
    return magnitudeSquared() < epsilon;
}

void Vector3::rotate(const Vector3& degrees, const Vector3& origin)
{

    const double rx = radians(degrees.x);
    const double ry = radians(degrees.y);
    const double rz = radians(degrees.z);

    operator-=(origin);

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

Vector3 operator*(double lhs, const Vector3& rhs)
{
    return rhs * lhs;
}

double dot(const Vector3& a, const Vector3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 cross(const Vector3& a, const Vector3& b)
{
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

bool Interval::contains(double val) const
{
    return val >= min && val <= max;
}

namespace fp_utils
{
    constexpr double epsilon = 1e-20; // Completely arbitrary (TODO: Set this properly)

    bool isZero(double val)
    {
        return std::abs(val) < epsilon;
    }

    bool equals(double lhs, double rhs)
    {
        return std::abs(lhs - rhs) < epsilon;
    }

    bool equals(const Vector3& lhs, const Vector3& rhs)
    {
        return std::abs(lhs.x - rhs.x) < epsilon && std::abs(lhs.y - rhs.y) < epsilon && std::abs(lhs.z - rhs.z) <
            epsilon;
    }
}

