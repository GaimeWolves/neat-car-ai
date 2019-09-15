#include "util.hpp"

#include <cstdlib>
#include <cmath>

constexpr float e = 2.71828182845904523536028747135266249775724709369995957496696762772407663;

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

float sigmoid(float t)
{
	return 1 / (1 + pow(e, -t));
}