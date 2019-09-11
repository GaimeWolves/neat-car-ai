#pragma once

#include "genome.h"
#include <ostream>

//Represents a network or phenotype.
class Network
{
public:
	Network(float minWeight, float maxWeight, int inputNodes, int outputNodes, std::vector<std::string> names);
	Network();

	Genome* GetGenome() { return &genome; }

	void ChangeFitness(float value) { fitness += value; }
	void SetFitness(float value) { fitness = value; }
	float GetFitness() { return fitness; }

#ifdef USE_SDL2_GFX
	void RenderNetwork(int x, int y, int width, int height);
#endif

private:
	Genome genome;
	float fitness;

	friend std::ostream& operator<<(std::ostream&, const Network&);
};