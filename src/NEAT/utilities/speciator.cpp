#include "speciator.hpp"

#include "crossover.hpp"

#include <cstdlib>
#include <algorithm>
#include <cmath>

std::vector<Species> SpeciatePopulation(NEAT &neat)
{
	std::vector<Species> result;

	for (int i = 0; i < neat.Networks.size(); i++)
	{
		bool inserted = false;
		for (auto species : result)
		{
			int index = rand() % species.population.size();
			if (DistanceFunction(neat, neat.Networks[i], *species.population[index]) < neat.distanceThreshold)
			{
				inserted = true;
				species.population.push_back(&neat.Networks[i]);
				break;
			}
		}

		if (!inserted)
			result.push_back({{&neat.Networks[i]}, 0, 0, 0});
	}

	//Sort population of species by fitness (lowest to highest).
	for (auto s : result)
		std::sort(s.population.begin(), s.population.end(), [] (Network* const &a, Network* const &b) -> bool { return a->GetFitness() > b->GetFitness(); });

	return result;
}

void CalculateSharedFitness(std::vector<Species> &species)
{
	for (auto s : species)
	{
		float sharedFitness = 0;
		for (auto network : s.population)
			sharedFitness += network->GetFitness() / s.population.size();

		s.sharedFitness = sharedFitness;
	}
}

void DoKillCycle(NEAT &neat, std::vector<Species> &species)
{
	float sumSharedFitness = 0;
	for (auto s : species)
		sumSharedFitness += s.sharedFitness;
	
	for (int i = 0; i < species.size(); i++)
	{
		species[i].score = species[i].sharedFitness / sumSharedFitness;

		if (species[i].population.size() == 1)
		{
			neat.DeleteNetwork(species[i].population[0]);
			species.erase(species.begin() + i--);
		}
		else
		{
			species[i].killed = std::round(neat.PopulationSize * species[i].score * 0.5f);

			for (int j = 0; j < species[i].killed; j++)
			{
				neat.DeleteNetwork(species[i].population[0]);
				species[i].population.erase(species[i].population.begin());
			}

			if (species[i].population.size() == 0)
				species.erase(species.begin() + i--);
		}
	}
}