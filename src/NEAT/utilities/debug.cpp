#include "debug.hpp"

#include <limits>

void PrintGenerationInfo(NEAT &neat)
{
	Network* best;
	float maxFitness = -std::numeric_limits<float>::max();
	float averageFitness = 0;
	float averageHiddenNodes = 0;
	float averageConnectionCount = 0;

	for (int i = 0; i < neat.Networks.size(); i++)
	{
		if (neat.Networks[i]->GetFitness() > maxFitness)
		{
			maxFitness = neat.Networks[i]->GetFitness();
			best = neat.Networks[i];
		}

		averageFitness += neat.Networks[i]->GetFitness() / (float)neat.PopulationSize;

		averageConnectionCount += neat.Networks[i]->GetGenome()->connectionGenes.size() / (float)neat.PopulationSize;
		int hiddenNodeCount = 0;

		for (auto n : neat.Networks[i]->GetGenome()->nodeGenes)
			if (n->type == Hidden)
				hiddenNodeCount++;

		averageHiddenNodes += (float)(hiddenNodeCount / neat.PopulationSize);
	}

	std::cout << "Generation " << neat.GetGeneration() << ":" << std::endl;
	std::cout << "Population size (should stay constant): " << neat.Networks.size() << "/" << neat.PopulationSize << std::endl;
	std::cout << "Best network: " << *best << "" << std::endl;
	std::cout << "Maximum fitness value: " << maxFitness << "" << std::endl;
	std::cout << "Average fitness value: " << averageFitness << "" << std::endl;
	std::cout << "Average connection count: " << averageConnectionCount << "" << std::endl;
	std::cout << "Average hidden node count: " << averageHiddenNodes << "" << std::endl;
}