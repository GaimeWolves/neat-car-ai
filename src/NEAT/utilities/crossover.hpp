#pragma once

#include "../network/network.hpp"
#include "../neat.hpp"

Network DoCrossover(NEAT &neat, Network &a, Network &b);

float DistanceFunction(NEAT &neat, Network &a, Network &b);