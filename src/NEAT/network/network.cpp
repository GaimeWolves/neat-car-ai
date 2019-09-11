#include "network.h"

#include "../utilities/util.h"

Network::Network(float minWeight, float maxWeight, int inputNodes, int outputNodes, std::vector<std::string> names)
{
	for (int i = 0; i < inputNodes + outputNodes; i++)
		genome.nodeGenes.push_back({i, (i < inputNodes) ? Sensor : Output, names[i]});

	int innovation = 0;
	for (int i = 0; i < inputNodes; i++)
		for (int j = 0; j < outputNodes; j++)
			genome.connectionGenes.push_back({genome.nodeGenes[i], genome.nodeGenes[inputNodes + j], RandomFloat(minWeight, maxWeight), true, ++innovation});
}

Network::Network()
{
	
}

#ifdef USE_SDL2_GFX
void Network::RenderNetwork(int x, int y, int width, int height)
{

}
#endif

std::ostream& operator<<(std::ostream &strm, const Network &network) 
{
	return strm << std::endl << "{" << std::endl << "Genome: " << network.genome << std::endl << "}";
}