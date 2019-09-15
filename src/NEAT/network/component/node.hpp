#pragma once

#include <string>
#include <ostream>
#include <vector>

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
	std::string name; 	//Used for drawing in sensor and output nodes;
	float value;		//Saves the current node value
};

bool operator==(const Node& a, const Node& b);
bool operator!=(const Node& a, const Node& b);

std::ostream& operator<<(std::ostream&, const Node&);