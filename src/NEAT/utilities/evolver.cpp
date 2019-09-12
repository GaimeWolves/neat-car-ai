#include "evolver.hpp"

#include <vector>
#include <cstdlib>
#include <limits>

#include "mutator.hpp"
#include "crossover.hpp"
#include "speciator.hpp"

void StepEvolution(NEAT &neat)
{
	std::vector<Species> speciesVector = SpeciatePopulation(neat);
	CalculateSharedFitness(speciesVector);
	DoKillCycle(neat, speciesVector);

	for (int i = 0; i < neat.Networks.size(); i++)
		MutateNetwork(neat, neat.Networks[i]);

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
		network.SetFitness(std::numeric_limits<float>::quiet_NaN());
}