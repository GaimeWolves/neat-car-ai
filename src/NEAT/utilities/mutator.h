#include "../network/network.h"
#include "../neat.h"

void MutateNetwork(NEAT &neat, Network &network);

void MutateChangeWeight(NEAT &neat, Network &network);
void MutateRandomizeWeight(NEAT &neat, Network &network);

bool MutateAddConnection(NEAT &neat, Network &network);
void MutateAddNode(NEAT &neat, Network &network);