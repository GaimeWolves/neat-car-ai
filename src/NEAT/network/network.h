#pragma once

#include "genome.h"

//Represents a network or phenotype.
class Network
{
public:

#ifdef USE_SDL2_GFX
	void RenderNetwork(int x, int y, int width, int height);
#endif

private:
	Genome genome;
	float fitness;
};