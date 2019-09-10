#include "network.h"

Network::Network(int inputNodes, int outputNodes, std::vector<std::string> names)
{
	for (int i = 0; i < inputNodes + outputNodes; i++)
		genome.nodeGenes.push_back({i, (i < inputNodes) ? Sensor : Output, names[i]});
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