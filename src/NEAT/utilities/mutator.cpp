#include "mutator.hpp"
#include "util.hpp"
#include <algorithm>

#include <iostream>

void MutateNetwork(NEAT &neat, Network &network)
{
	if (network.GetGenome()->connectionGenes.size() > 0 && RandomFloat() < neat.ChangeWeightChance)
	{
		if (RandomFloat() < neat.SmoothChangeChance)
		{
			MutateRandomizeWeight(neat, network);
		}
		else
		{
			MutateRandomizeWeight(neat, network);
		}
	}

	if (RandomFloat() < neat.AddConnectionChance)
	{
		MutateAddConnection(neat, network);
	}

	if (RandomFloat() < neat.MaxAddNodeChance)
	{
		if (network.GetGenome()->connectionGenes.size() > 0)
			MutateAddNode(neat, network);
	}
}


void MutateChangeWeight(NEAT &neat, Network &network)
{
	Genome* genome = network.GetGenome();

	int index = -1;
	{
		int tmp = 0;
		while (index == -1) //There is always a connection enabled
		{
			tmp = rand() % genome->connectionGenes.size();
			if (genome->connectionGenes[tmp].enabled)
				index = tmp;
		}
	}

	Connection* connection = &genome->connectionGenes[index];

	if (connection->weight == neat.MinWeightValue || connection->weight == neat.MaxWeightvalue)
		connection->weight *= 1 - neat.UniformWeightChange;
	else
	{
		connection->weight *= 1 + (neat.UniformWeightChange * (rand() % 2 ? -1 : 1)); //Randomly multply with positive or negative value.
		connection->weight = std::min(neat.MaxWeightvalue, std::max(neat.MinWeightValue, connection->weight));
	}
}

void MutateRandomizeWeight(NEAT &neat, Network &network)
{
	Genome* genome = network.GetGenome();

	int index = -1;
	{
		int tmp = 0;
		while (index == -1) //There is always a connection enabled
		{
			tmp = rand() % genome->connectionGenes.size();
			if (genome->connectionGenes[tmp].enabled)
				index = tmp;
		}
	}

	float value = RandomFloat(neat.MinWeightValue, neat.MaxWeightvalue);
	genome->connectionGenes[index].weight = value;
}

//Returns if suceeded
bool MutateAddConnection(NEAT &neat, Network &network)
{
	Genome* genome = network.GetGenome();
	std::vector<int> fullNodes;

	while(fullNodes.size() != genome->nodeGenes.size())
	{
		int nodeA = -1;
		{
			int tmp = 0;
			while (nodeA == -1)
			{
				tmp = rand() % genome->nodeGenes.size();
				if (std::find(fullNodes.begin(), fullNodes.end(), tmp) == fullNodes.end())
					nodeA = tmp;
			}
		}

		std::vector<int> checkedNodes;
		checkedNodes.push_back(nodeA);
		if (genome->nodeGenes[nodeA].type == Sensor)
		{
			for (auto node : genome->nodeGenes)
				if (node.type == Sensor)
					checkedNodes.push_back(node.index);
		}
		else if (genome->nodeGenes[nodeA].type == Output)
		{
			for (auto node : genome->nodeGenes)
				if (node.type == Output)
					checkedNodes.push_back(node.index);
		}

		bool foundPair = false;
		int nodeB = -1;
		while(checkedNodes.size() != genome->nodeGenes.size())
		{
			nodeB = -1;
			int tmp = 0;
			while (nodeB == -1)
			{
				tmp = rand() % genome->nodeGenes.size();
				if (std::find(checkedNodes.begin(), checkedNodes.end(), tmp) == checkedNodes.end())
					nodeB = tmp;
			}

			if (HasConnection(genome->nodeGenes[nodeA], genome->nodeGenes[nodeB], *genome))
				checkedNodes.push_back(nodeB);
			else
			{
				foundPair = true;
				break;
			}
		}

		if (foundPair)
		{
			bool flip = genome->nodeGenes[nodeA].type == Output || genome->nodeGenes[nodeB].type == Sensor;

			int in = flip ? nodeB : nodeA;
			int out = flip ? nodeA : nodeB;

			int actualInnovation = -1;
			for (auto c : *neat.GetGlobalConnectionList())
			{
				if (c.in->index == in && c.out->index == out)
				{
					actualInnovation = c.innovation;
					break;
				}
			}

			Connection connection = {
				&genome->nodeGenes[in],
				&genome->nodeGenes[out],
				RandomFloat(neat.MinWeightValue, neat.MaxWeightvalue),
				true,
				actualInnovation == -1 ? ++neat.GlobalInnovationCounter : actualInnovation
			};

			if (actualInnovation == -1)
				neat.AddConnectionToGlobalPool(connection);

			genome->connectionGenes.push_back(connection);
			return true;
		}
		else
		{
			fullNodes.push_back(nodeA);
		}
	}

	return false;
}

void MutateAddNode(NEAT &neat, Network &network)
{
	Genome* genome = network.GetGenome();
	int index = rand() % genome->connectionGenes.size();
	genome->connectionGenes[index].enabled = false;
	genome->nodeGenes.push_back({(int)genome->nodeGenes.size(), Hidden});
	genome->connectionGenes.push_back({
		genome->connectionGenes[index].in,
		&genome->nodeGenes[genome->nodeGenes.size() - 1],
		1,
		true,
		++neat.GlobalInnovationCounter
	});

	genome->connectionGenes.push_back({
		&genome->nodeGenes[genome->nodeGenes.size() - 1],
		genome->connectionGenes[index].out,
		genome->connectionGenes[index].weight,
		true,
		++neat.GlobalInnovationCounter
	});
}