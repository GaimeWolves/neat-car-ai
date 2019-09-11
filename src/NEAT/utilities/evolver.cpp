#include "evolver.h"

#include <vector>
#include <cstdlib>

#include "mutator.h"
#include "crossover.h"
#include "speciator.h"

void StepEvolution(NEAT &neat)
{
	std::vector<Species> speciesVector = SpeciatePopulation(neat);
	CalculateSharedFitness(speciesVector);
	DoKillCycle(neat, speciesVector);

	for (auto network : neat.Networks)
		MutateNetwork(neat, network);

	for (auto species : speciesVector)
	{
		while (species.killed-- > 0)
		{
			int parentA = rand() % species.population.size();
			int parentB = 0;

			do
			{
				int parentB = rand() % species.population.size();
			} while (parentB == parentA);

			Network network = DoCrossover(neat, *species.population[parentA], *species.population[parentB]);
			neat.Networks.push_back(network);
		}
	}

	for (auto network : neat.Networks)
		network.SetFitness(NAN); //Reset fitness
}