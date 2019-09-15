#include "evolver.hpp"

#include <vector>
#include <cstdlib>
#include <limits>

#include "mutator.hpp"
#include "crossover.hpp"
#include "speciator.hpp"

void StepEvolution(NEAT &neat)
{
	std::vector<Species> species = SpeciatePopulation(neat);
	CalculateSharedFitness(species);
	DoKillCycle(neat, species);

	for (int i = 0; i < species.size(); i++)
	{
		while (species[i].killed-- > 0)
		{
			int parentA = rand() % species[i].population.size();
			int parentB = 0;
			Network* parentBNetwork;

			if (species[i].population.size() > 1)
			{
				do
				{
					parentB = rand() % species[i].population.size();
				} while (parentB == parentA);

				parentBNetwork = species[i].population[parentB];
			}
			else
			{
				int next = (i + 1) % species.size();
				parentBNetwork = species[next].population[rand() % species[next].population.size()];
			}

			Network* network = DoCrossover(neat, *species[i].population[parentA], *parentBNetwork);
			MutateNetwork(neat, *network);
			neat.Networks.push_back(network);
		}
	}

	while(neat.Networks.size() < neat.PopulationSize)
	{
		int parentA = rand() % neat.Networks.size();
		int parentB = 0;

		do
		{
			parentB = rand() % neat.Networks.size();
		} while (parentB == parentA);

		Network* network = DoCrossover(neat, *neat.Networks[parentA], *neat.Networks[parentB]);
		MutateNetwork(neat, *network);
		neat.Networks.push_back(network);
	}

	for (auto network : neat.Networks)
		network->SetFitness(0);
}