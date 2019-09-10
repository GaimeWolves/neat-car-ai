#pragma once

#include <vector>

#include "component/connection.h"
#include "component/node.h"

struct Genome
{
	std::vector<Connection> connectionGenes;
	std::vector<Node> nodeGenes;
};

std::vector<Node> DeduceNodesFromConnections(Genome genome);
bool HasConnection(Node a, Node b, Genome genome);
std::ostream& operator<<(std::ostream&, const Genome&);