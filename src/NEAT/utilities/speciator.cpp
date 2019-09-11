#include "speciator.h"

#include "crossover.h"

#include <cstdlib>
#include <algorithm>
#include <cmath>

std::vector<Species> SpeciatePopulation(NEAT &neat)
{
	std::vector<Species> result;

	for (auto network : neat.Networks)
	{
		bool inserted = false;
		for (auto species : result)
		{
			int index = rand() % species.population.size();
			if (DistanceFunction(neat, network, *species.population[index]) < neat.distanceThreshold)
			{
				inserted = true;
				species.population.push_back(&network);
				break;
			}
		}

		if (!inserted)
			result.push_back({{&network}, NAN, 0});
	}

	//Sort population of species by fitness.
	for (auto s : result)
		std::sort(s.population.begin(), s.population.end(), [] (Network* const &a, Network* const &b) -> bool { return a->GetFitness() < b->GetFitness(); });

	return result;
}

void CalculateSharedFitness(std::vector<Species> &species)
{
	for (auto s : species)
	{
		float sharedFitness;
		for (auto network : s.population)
			sharedFitness += network->GetFitness() / s.population.size();

		s.sharedFitness = sharedFitness;
	}
}

void DoReproductionCycle(NEAT &neat, std::vector<Species> &species)
{
	float sumSharedFitness;
	for (auto s : species)
		sumSharedFitness += s.sharedFitness;
	
	for (int i = 0; i < species.size(); i++)
	{
		species[i].score = species[i].sharedFitness / sumSharedFitness;

		if (species[i].population.size() == 1)
		{
			//TODO: Remove from neat.Networks vector
			species.erase(species.begin() + i);
		}
		else if (species[i].population.size() == 2 && species[i].score < 0.5f)
		{
			//TODO: Remove from neat.Networks vector
			species.erase(species.begin() + i);
		}
		else
		{
			species[i].killed = std::round(species[i].population.size() * species[i].score);
		}
		
	}
}