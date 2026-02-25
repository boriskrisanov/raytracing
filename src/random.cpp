#include "random.hpp"
#include "math.hpp"
#include <random>

Random::Random()
    : rng{0}
{
}

Random::Random(unsigned long long seed)
    : rng{seed}
{
}

Vector3 Random::randomUnitVector()
{
    std::uniform_real_distribution<> distribution(-1, 1);
    while (true)
    {
        Vector3 v{distribution(rng), distribution(rng), distribution(rng)};
        if (v.magnitudeSquared() > 1e-160 && v.magnitudeSquared() <= 1)
        {
            return v.normalised();
        }
    }
}

Vector3 Random::randomUnitVectorInHemisphere(Vector3 normal)
{
    Vector3 unitVector = randomUnitVector();
    if (dot(normal, unitVector) < 0)
    {
        unitVector *= -1;
    }
    return unitVector;
}

double Random::randomDouble(double min, double max)
{
    std::uniform_real_distribution distribution(min, max);
    return distribution(rng);
}

int Random::randomInt(int min, int max)
{
    std::uniform_int_distribution distribution(min, max);
    return distribution(rng);
}
