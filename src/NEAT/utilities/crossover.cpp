#include "crossover.hpp"

#include <cstdlib>
#include <vector>
#include <algorithm>

#include <iostream>

Network* DoCrossover(NEAT &neat, Network &a, Network &b)
{
	int maxInnovationNumber = -1;

	Network* result = neat.CreateStandardNetwork();
	result->GetGenome()->connectionGenes.clear();

	for (auto g : a.GetGenome()->connectionGenes)
		if (g.innovation > maxInnovationNumber)
			maxInnovationNumber = g.innovation;

	for (auto g : b.GetGenome()->connectionGenes)
		if (g.innovation > maxInnovationNumber)
			maxInnovationNumber = g.innovation;

	if (maxInnovationNumber == -1)
		return result;

	for (int i = 0; i <= maxInnovationNumber; i++)
	{
		bool aHasGene = false;
		Connection aGene;
		bool bHasGene = false;
		Connection bGene;

		for (auto g : b.GetGenome()->connectionGenes)
		{
			if (g.innovation == i)
			{
				bHasGene = true;
				bGene = g;
				break;
			}
		}

		for (auto g : a.GetGenome()->connectionGenes)
		{
			if (g.innovation == i)
			{
				aHasGene = true;
				aGene = g;
				break;
			}
		}

		if (!aHasGene && !bHasGene)
			continue;

		Connection connection;

		if (aHasGene && bHasGene)
		{
			connection = rand() % 2 ? aGene : bGene;
		}
		else if (a.GetFitness() == b.GetFitness())
		{
			if (rand() % 2)
				connection = aHasGene ? aGene : bGene;
		}
		else if (a.GetFitness() > b.GetFitness())
		{
			if (aHasGene)
				connection = aGene;
		}
		else if (a.GetFitness() < b.GetFitness())
		{
			if (bHasGene)
				connection = bGene;
		}

		connection.enabled = rand() % 4 ? connection.enabled : true; //specified chance of enabling connection otherwise inherit if enabled

		for (int j = 0; j < result->GetGenome()->nodeGenes.size(); j++)
		{
			if (connection.in->index == result->GetGenome()->nodeGenes[j]->index)
				connection.in = result->GetGenome()->nodeGenes[j];
			else if (connection.out->index == result->GetGenome()->nodeGenes[j]->index)
				connection.out = result->GetGenome()->nodeGenes[j];
		}

		result->GetGenome()->connectionGenes.push_back(connection);
	}

	std::vector<Node*> nodes = DeduceNodesFromConnections(*result->GetGenome());

	result->GetGenome()->nodeGenes = nodes;

	return result;
}

float DistanceFunction(NEAT &neat, Network &a, Network &b)
{
	int maxInnovationA = -1;
	int maxInnovationB = -1;
	int maxInnovation = -1;

	for (auto g : a.GetGenome()->connectionGenes)
		if (g.innovation > maxInnovationA)
			maxInnovationA = g.innovation;

	for (auto g : b.GetGenome()->connectionGenes)
		if (g.innovation > maxInnovationB)
			maxInnovationB = g.innovation;

	if (maxInnovationB == -1 && maxInnovationA == -1)
		return 0;

	maxInnovation = maxInnovationA > maxInnovationB ? maxInnovationA : maxInnovationB;

	int numberOfGenes = maxInnovationB > maxInnovationA ? (int)b.GetGenome()->connectionGenes.size() : (int)a.GetGenome()->connectionGenes.size();
	if (numberOfGenes < neat.nThreshold)
		numberOfGenes = 1;

	int excess = 0;
	int disjoint = 0;
	float sharedweightSum = 0;
	int sharedWeightNum = 0;

	for (int i = 0; i <= maxInnovation; i++)
	{
		bool aHasGene = false;
		Connection aGene;
		bool bHasGene = false;
		Connection bGene;

		for (auto g : b.GetGenome()->connectionGenes)
			if (g.innovation == i)
			{
				bHasGene = true;
				bGene = g;
				break;
			}

		for (auto g : a.GetGenome()->connectionGenes)
			if (g.innovation == i)
			{
				aHasGene = true;
				aGene = g;
				break;
			}

		if (aHasGene && bHasGene)
		{
			sharedWeightNum++;
			sharedweightSum += (aGene.weight - bGene.weight);
		}
		else if (aHasGene || bHasGene)
		{
			if (aHasGene)
			{
				if (maxInnovationB < maxInnovationA)
					excess++;
				else
					disjoint++;
			}
			else
			{
				if (maxInnovationA < maxInnovationB)
					excess++;
				else
					disjoint++;
			}
		}
	}

	return (neat.c1 * excess) / numberOfGenes + (neat.c2 * disjoint) / numberOfGenes + neat.c3 * (sharedweightSum / sharedWeightNum);
}