#pragma once

#include "node.h"

//Represents a connection gene
struct Connection
{
	Node in;
	Node out;
	float weight;
	bool enabled;
	int innovation;
};