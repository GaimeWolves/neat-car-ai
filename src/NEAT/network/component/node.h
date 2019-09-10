#pragma once

#include <string>
#include <ostream>

enum NodeType
{
	Sensor,
	Hidden,
	Output
};

//Represents a node gene
struct Node
{
	int index;
	NodeType type;
	std::string name; //Used for drawing in sensor and output nodes;
};

std::ostream& operator<<(std::ostream&, const Node&);