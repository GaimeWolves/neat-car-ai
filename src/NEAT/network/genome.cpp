#include "genome.hpp"

#include <iostream>
#include <sstream>

std::vector<Node> DeduceNodesFromConnections(Genome genome)
{
	std::vector<Node> deducedNodes = genome.nodeGenes;

	for (auto g : genome.connectionGenes)
	{
		bool addIn = true;
		bool addOut = true;
		for (auto n : deducedNodes)
		{
			if (n.index == g.in->index)
				addIn = false;
			else if (n.index == g.out->index)
				addOut = false;

			if (!addIn && !addOut)
				break;
		}

		if (addIn)
		{
			deducedNodes.push_back(*g.in);
			g.in = &deducedNodes[deducedNodes.size() - 1];
		}

		if (addOut)
		{
			deducedNodes.push_back(*g.out);
			g.out = &deducedNodes[deducedNodes.size() - 1];
		}
	}

	return deducedNodes;
}

bool HasConnection(Node a, Node b, Genome genome)
{
	for (auto connection : genome.connectionGenes)
		if ((connection.in->index == a.index && connection.out->index == b.index) || (connection.in->index == b.index && connection.out->index == a.index))
			return true;
	return false;
}

std::ostream& operator<<(std::ostream& strm, const Genome& genome)
{
	std::stringstream nodeGenes;
	for (auto n : genome.nodeGenes)
		nodeGenes << "    " << n << "," << std::endl;
	std::string nodeGenesString = nodeGenes.str();

	std::stringstream connectionGenes;
	for (auto n : genome.connectionGenes)
		connectionGenes << "    " << n << "," << std::endl;
	std::string connectionGenesString = connectionGenes.str();

	return strm << std::endl <<
	"{" << std::endl <<
	"  Node genes:" << std::endl <<
	"  {" << std::endl <<
	nodeGenesString <<
	"  }," << std::endl <<
	"  Connection genes:" << std::endl <<
	"  {" << std::endl <<
	connectionGenesString <<
	"  }" << std::endl <<
	"}";
}