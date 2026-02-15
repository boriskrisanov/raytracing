#pragma once

#include <array>
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
    void invertRotation(const Vector3& initialDegrees, const Vector3& origin);

private:
    void rotateX(double radians);
    void rotateY(double radians);
    void rotateZ(double radians);
};

Vector3 operator*(double lhs, const Vector3& rhs);
Vector3 operator-(Vector3 v);

double dot(const Vector3& a, const Vector3& b);
Vector3 cross(const Vector3& a, const Vector3& b);

struct Matrix3
{
    // rows, columns
    std::array<std::array<double, 3>, 3> m;
    void setColumn(int index, Vector3 vector);
    double det() const;
};

Vector3 solve3Unknowns(const Vector3& coefficient1, const Vector3& coefficient2, const Vector3& coefficient3,
                       const Vector3& rhs);

class Interval
{
public:
    Interval(double a, double b);

    bool contains(double value) const;
    void include(double value);

    double getMin() const;
    double getMax() const;

    bool isEmpty() const;
    Interval getIntersectionWith(Interval other) const;

private:
    double min;
    double max;
};

namespace fp_utils
{
    bool isZero(double val);
    bool equals(double lhs, double rhs);
    bool equals(const Vector3& lhs, const Vector3& rhs);
}
