#include "../neat.h"

#include <vector>

struct Species
{
	std::vector<Network*> population;
	float sharedFitness;
	float score; //Temporary variable: sharedFitness divided by sum of sharedFitness from every species;
	int killed; //Temporary variable: killed networks during reproduction cycle
};

std::vector<Species> SpeciatePopulation(NEAT &neat);
void CalculateSharedFitness(std::vector<Species> &species);
void DoReproductionCycle(NEAT &neat, std::vector<Species> &species);