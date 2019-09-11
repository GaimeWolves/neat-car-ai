#include "neat.h"

#include "utilities/evolver.h"

#include <algorithm>
#include <iostream>

NEAT::NEAT(int inputNodes, int outputNodes, std::vector<std::string> names, int population)
{
	this->PopulationSize = population;
	this->inputNodes = inputNodes;
	this->outputNodes = outputNodes;
	this->names = names;
	this->population = population;

	for (int i = 0; i < population; i++)
		Networks.push_back(Network(MinWeightValue, MaxWeightvalue, inputNodes, outputNodes, names));
	
	if (population > 0)
		GlobalNodeGeneList.insert(GlobalNodeGeneList.end(), Networks[0].GetGenome()->nodeGenes.begin(), Networks[0].GetGenome()->nodeGenes.end());

	GlobalInnovationCounter = inputNodes * outputNodes;
}

bool NEAT::DeleteNetwork(Network* network)
{
	int foundNetwork = -1;

	for (int i = 0; i < Networks.size(); i++)
		if (&Networks[i] == network)
		{
			foundNetwork = i;
			break;
		}

	if (foundNetwork != -1)
	{
		Networks.erase(Networks.begin() + foundNetwork);
		return true;
	}

	return false;
}

void NEAT::DoEvolutionCycle()
{
	for (auto network : Networks)
	{
		if (network.GetFitness() == NAN)
		{
			std::cerr << "WARNING: Network has no fitness value set stopping evolution cycle!" << std::endl << "Affected network: " << network << std::endl;
			return;
		}
	}

	StepEvolution(*this);
}