#pragma once

#include <cmath>

// TODO: Move definitions to cpp file

constexpr double pi = 3.14159265359;

double radians(double x);

struct Vector3
{
    double x = 0;
    double y = 0;
    double z = 0;

    Vector3 operator+(Vector3 rhs) const;
    void operator+=(Vector3 rhs);
    Vector3 operator-(Vector3 rhs) const;
    void operator-=(Vector3 rhs);
    Vector3 operator*(double scalar) const;
    Vector3 operator*(Vector3 v) const;
    void operator*=(double scalar);
    // Component multiplication
    void operator*=(Vector3 vector);
    Vector3 operator/(double scalar) const;

    double magnitude() const;
    double magnitudeSquared() const;
    Vector3 normalised() const;
    bool isMagnitudeNearZero() const;

    void rotate(const Vector3& degrees, const Vector3& origin);
};

Vector3 operator*(double lhs, const Vector3& rhs);

double dot(const Vector3& a, const Vector3& b);
Vector3 cross(const Vector3& a, const Vector3& b);

struct Interval
{
    const double min;
    const double max;

    bool contains(double val) const;
};

namespace fp_utils
{
    bool isZero(double val);
    bool equals(double lhs, double rhs);
    bool equals(const Vector3& lhs, const Vector3& rhs);
}
