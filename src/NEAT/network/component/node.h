#pragma once

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
};