#pragma once

#include <vector>

#include "network/network.h"

class NEAT
{
public:

private:
	int inputNodes;
	int outputNodes;

	std::vector<Network> networks;

	std::vector<Node> GlobalNodeGeneList;
	std::vector<Connection> GlobalConnectionGeneList;
};