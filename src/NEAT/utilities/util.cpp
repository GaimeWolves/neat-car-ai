#include "util.h"

#include <cstdlib>

float RandomFloat()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float RandomFloat(float max)
{
    return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / max));
}

float RandomFloat(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}