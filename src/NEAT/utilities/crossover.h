#pragma once

#include "../network/network.h"
#include "../neat.h"

Network DoCrossover(NEAT &neat, Network &a, Network &b);

float DistanceFunction(NEAT &neat, Network &a, Network &b);