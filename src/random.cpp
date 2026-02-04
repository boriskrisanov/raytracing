#include "random.hpp"
#include "math.hpp"
#include <random>

static std::random_device randomDevice;
static std::default_random_engine rng{randomDevice()};
static std::uniform_real_distribution<double> uniformRealDistribution(-1, 1);

Vector3 randomUnitVector()
{
    while (true)
    {
        Vector3 v{uniformRealDistribution(rng), uniformRealDistribution(rng), uniformRealDistribution(rng)};
        if (v.magnitudeSquared() > 1e-160 && v.magnitudeSquared() <= 1)
        {
            return v.normalised();
        }
    }
}

Vector3 randomUnitVectorInHemisphere(Vector3 normal)
{
    Vector3 unitVector = randomUnitVector();
    if (dot(normal, unitVector) < 0)
    {
        unitVector *= -1;
    }
    return unitVector;
}