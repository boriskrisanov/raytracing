#pragma once

#include <random>

struct Vector3;

class Random
{
public:
    Random();
    Random(unsigned long long seed);
    Vector3 randomUnitVector();
    Vector3 randomUnitVectorInHemisphere(Vector3 normal);
    double randomDouble(double min, double max);
    int randomInt(int min, int max);
private:
    std::mt19937_64 rng;
};