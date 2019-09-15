#include "neat.hpp"

#include "utilities/evolver.hpp"
#include "utilities/debug.hpp"

#include <algorithm>
#include <limits>
#include <iostream>

NEAT::NEAT(int inputNodes, int outputNodes, std::vector<std::string> names, int population)
{
	this->PopulationSize = population;
	this->inputNodes = inputNodes;
	this->outputNodes = outputNodes;
	this->names = names;
	this->population = population;

	for (int i = 0; i < population; i++)
		Networks.push_back(new Network(MinWeightValue, MaxWeightvalue, inputNodes, outputNodes, names));

	GlobalInnovationCounter = inputNodes * outputNodes;
}

bool NEAT::DeleteNetwork(Network* network)
{

	for (int i = 0; i < Networks.size(); i++)
		if (Networks[i] == network)
		{
			delete Networks[i];
			Networks.erase(Networks.begin() + i);
			return true;
		}

	return false;
}

void NEAT::DoEvolutionCycle()
{
	for (auto network : Networks)
	{
		if (network->GetFitness() == std::numeric_limits<float>::quiet_NaN())
		{
			std::cerr << "WARNING: Network has no fitness value set stopping evolution cycle!" << std::endl << "Affected network: " << network << std::endl;
			return;
		}
	}

	PrintGenerationInfo(*this);

	StepEvolution(*this);
	generation++;
}