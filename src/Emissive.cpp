#include "Emissive.hpp"

Vector3 Emissive::emit() const
{
    return emissionColor * emissionStrength;
}
