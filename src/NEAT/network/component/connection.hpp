#pragma once

#include "node.hpp"

//Represents a connection gene
struct Connection
{
	Node* in;
	Node* out;
	float weight;
	bool enabled;
	int innovation;
};

bool operator==(const Connection &a, const Connection &b);
bool operator!=(const Connection &a, const Connection &b);
std::ostream& operator<<(std::ostream&, const Connection&);