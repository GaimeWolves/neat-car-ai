#pragma once

#include <vector>

#include "component/connection.hpp"
#include "component/node.hpp"

struct Genome
{
	std::vector<Connection> connectionGenes;
	std::vector<Node*> nodeGenes;
};

std::vector<Node*> DeduceNodesFromConnections(Genome &genome);
bool HasConnection(Node a, Node b, Genome genome);

bool operator==(const Genome &a, const Genome &b);
std::ostream& operator<<(std::ostream&, const Genome&);