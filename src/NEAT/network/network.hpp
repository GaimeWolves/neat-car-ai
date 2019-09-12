#pragma once

#include "genome.hpp"
#include <ostream>

#include <vector>
#include <string>

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

	float GetOutput(std::string name);
	void SetInput(std::string name, float value);

	void Propagate();

#ifdef USE_SDL2_GFX
	void RenderNetwork(int x, int y, int width, int height);
#endif

private:
	Genome genome;
	float fitness;

	std::vector<Node*> input;
	std::vector<Node*> output;

	friend std::ostream& operator<<(std::ostream&, const Network&);
};