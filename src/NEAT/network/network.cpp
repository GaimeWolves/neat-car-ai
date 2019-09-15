#include "network.hpp"

#include <iostream>

#include "../utilities/util.hpp"

static float Preactivation(Genome &genome, Node* node);
static float Activation(Genome &genome, Node* node);

Network::Network(float minWeight, float maxWeight, int inputNodes, int outputNodes, std::vector<std::string> names)
{
	for (int i = 0; i < inputNodes + outputNodes; i++)
		genome.nodeGenes.push_back(new Node {i, (i < inputNodes) ? Sensor : Output, names[i]});

	for (int i = 0; i < genome.nodeGenes.size(); i++)
	{
		if (genome.nodeGenes[i]->type == Sensor)
			input.push_back(genome.nodeGenes[i]);
		else if (genome.nodeGenes[i]->type == Output)
			output.push_back(genome.nodeGenes[i]);
	}

	int innovation = 0;
	for (int i = 0; i < inputNodes; i++)
		for (int j = 0; j < outputNodes; j++)
			genome.connectionGenes.push_back({genome.nodeGenes[i], genome.nodeGenes[inputNodes + j], RandomFloat(minWeight, maxWeight), true, ++innovation});
}

Network::Network()
{
	
}

float Network::GetOutput(std::string name)
{
	for (auto n : output)
		if (n->name == name)
			return n->value;

	return 0;
}

void Network::SetInput(std::string name, float value)
{
	for (auto n : input)
		if (n->name == name)
		{
			n->value = value;
			return;
		}
}

static float Preactivation(Genome &genome, Node* node)
{
	float value = 0;
	for (auto c : genome.connectionGenes)
	{
		if (*c.out == *node && c.enabled)
			value += Activation(genome, c.in) * c.weight;
	}
	return value;
}

static float Activation(Genome &genome, Node* node)
{
	return node->type == Sensor ? node->value : sigmoid(Preactivation(genome, node));
}

void Network::Propagate()
{
	for (auto node : output)
		node->value = Activation(genome, node);
}

#ifdef USE_SDL2_GFX
void Network::RenderNetwork(int x, int y, int width, int height)
{

}
#endif

bool operator==(const Network &a, const Network &b)
{
	return a.fitness == b.fitness && a.genome == b.genome;
}

std::ostream& operator<<(std::ostream &strm, const Network &network) 
{
	return strm << std::endl << "{" << std::endl << "Genome: " << network.genome << std::endl << "}";
}