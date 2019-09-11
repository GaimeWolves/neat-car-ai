#pragma once

#include <vector>

#include "network/network.h"

class NEAT
{
public:
	NEAT(int inputNodes, int outputNodes, std::vector<std::string> names, int population);

	void AddConnectionToGlobalPool(Connection connection) { GlobalConnectionGeneList.push_back(connection); }
	void AddNodeToGlobalPool(Node node) { GlobalNodeGeneList.push_back(node); }

	void DoEvolutionCycle();

	const int GetInputNodeCount() { return inputNodes; }
	const int GetOutputNodeCount() { return outputNodes; }

	const std::vector<Connection>* GetGlobalConnectionList() { return &GlobalConnectionGeneList; }
	const std::vector<Node>* GetGlobalNodeList() { return &GlobalNodeGeneList; }

	std::vector<Network> Networks;
	Network CreateStandardNetwork() { return Network(MinWeightValue, MaxWeightvalue, inputNodes, outputNodes, names); }
	bool DeleteNetwork(Network* network);

	//////NEAT Parameters//////
	//Must be a positive and negative value.
	float MinWeightValue = -2;
	float MaxWeightvalue = 2;

	float UniformWeightChange = 0.1f;

	float ChangeWeightChance = 0.8f;
	float SmoothChangeChance = 0.9f;
	float MaxAddNodeChance = 0.3f;
	float AddConnectionChance = 0.05f;

	//Used for distance fuction
	float c1 = 1;
	float c2 = 1;
	float c3 = 0.4f;
	int nThreshold = 20;

	//Used for speciation
	float distanceThreshold = 3;

	int PopulationSize = 100;
	int GlobalInnovationCounter = 0;

private:
	int inputNodes;
	int outputNodes;

	int population;

	std::vector<std::string> names;

	std::vector<Node> GlobalNodeGeneList;
	std::vector<Connection> GlobalConnectionGeneList;
};