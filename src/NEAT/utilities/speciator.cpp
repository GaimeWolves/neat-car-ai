#include "speciator.hpp"

#include "crossover.hpp"

#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <limits>

std::vector<Species> SpeciatePopulation(NEAT &neat)
{
	std::vector<Species> result;

	for (int i = 0; i < neat.Networks.size(); i++)
	{
		bool inserted = false;
		for (int j = 0; j < result.size(); j++)
		{
			int index = rand() % result[j].population.size();
			if (DistanceFunction(neat, *neat.Networks[i], *result[j].population[index]) < neat.distanceThreshold)
			{
				inserted = true;
				result[j].population.push_back(neat.Networks[i]);
				break;
			}
		}

		if (!inserted)
			result.push_back({{neat.Networks[i]}, 0, 0, 0});
	}

	//Sort population of species by fitness (lowest to highest).
	for (int i = 0; i < result.size(); i++)
		std::sort(result[i].population.begin(), result[i].population.end(), [] (Network* const &a, Network* const &b) -> bool { return a->GetFitness() > b->GetFitness(); });

	return result;
}

void CalculateSharedFitness(std::vector<Species> &species)
{
	for (int i = 0; i < species.size(); i++)
	{
		float sharedFitness = 0;
		for (auto network : species[i].population)
			sharedFitness += network->GetFitness() / species[i].population.size();

		species[i].sharedFitness = sharedFitness;
	}
}

void DoKillCycle(NEAT &neat, std::vector<Species> &species) //Killcount may vary depending on the score distribution
{
	float maxFitness = -std::numeric_limits<float>::max();
	float minFitness = std::numeric_limits<float>::max();
	for (auto s : species)
	{
		if (s.sharedFitness > maxFitness)
			maxFitness = s.sharedFitness;
		if (s.sharedFitness < minFitness)
			minFitness = s.sharedFitness;
	}

	float maxDistance = sqrt(pow(maxFitness - minFitness, 2));

	for (int i = 0; i < species.size(); i++)
	{
		species[i].score = maxDistance == 0 ? 1 : 1 - sqrt(pow(maxFitness - species[i].sharedFitness, 2)) / maxDistance;

		if (species[i].population.size() == 1 && species[i].score == 1)
		{
			neat.DeleteNetwork(species[i].population[0]);
			species.erase(species.begin() + i--);
		}
		else
		{
			species[i].killed = std::round(neat.Networks.size() * species[i].score * 0.5f);

			if (species[i].killed > species[i].population.size())
				species[i].killed -= species[i].killed - (int)species[i].population.size();

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