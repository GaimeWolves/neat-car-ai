#include "debug.hpp"

void PrintGenerationInfo(NEAT &neat)
{
		Network best;
	float maxFitness = 0;
	float averageFitness = 0;
	float averageHiddenNodes = 0;
	float averageConnectionCount = 0;

	for (auto network : neat.Networks)
	{
		if (network.GetFitness() > maxFitness)
		{
			maxFitness = network.GetFitness();
			best = network;
		}

		averageFitness += network.GetFitness() / neat.PopulationSize;

		averageConnectionCount += network.GetGenome()->connectionGenes.size() / neat.PopulationSize;
		int hiddenNodeCount = 0;

		for (auto n : network.GetGenome()->nodeGenes)
			if (n.type == Hidden)
				hiddenNodeCount++;

		averageHiddenNodes += (float)(hiddenNodeCount / neat.PopulationSize);
	}

	std::cout << "Generation " << neat.GetGeneration() << ":" << std::endl;
	std::cout << "Best network: " << std::endl << best << "" << std::endl;
	std::cout << "Maximum fitness value: " << maxFitness << "" << std::endl;
	std::cout << "Average fitness value: " << averageFitness << "" << std::endl;
	std::cout << "Average connection count: " << averageConnectionCount << "" << std::endl;
	std::cout << "Average hidden node count: " << averageHiddenNodes << "" << std::endl;
}