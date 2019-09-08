#pragma once

#include <vector>

#include "component/connection.h"
#include "component/node.h"

struct Genome
{
	std::vector<Connection> connectionGenes;
	std::vector<Node> nodeGenes;
};