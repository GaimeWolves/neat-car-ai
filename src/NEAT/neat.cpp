#include "neat.h"

NEAT::NEAT(int inputNodes, int outputNodes, std::vector<std::string> names, int population)
{
	this->inputNodes = inputNodes;
	this->outputNodes = outputNodes;
	this->names = names;
	this->population = population;

	for (int i = 0; i < population; i++)
		Networks.push_back(Network(inputNodes, outputNodes, names));
	
	if (population > 0)
		GlobalNodeGeneList.insert(GlobalNodeGeneList.end(), Networks[0].GetGenome()->nodeGenes.begin(), Networks[0].GetGenome()->nodeGenes.end());
}