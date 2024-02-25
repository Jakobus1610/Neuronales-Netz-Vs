#include <cmath>

struct Node
{
	double value;
	double bias;
};

struct Weight
{
	double weight;
};

struct Network
{
	Node nodes[][];
	Weight
};

struct NetworkInfo
{
	int numLayers;
	int *numRows;
};

const double EULER = 2.71828182845904523536;

void initNetwork(Network& network, NetworkInfo netInfo)
{
	network.numNodeLayers = netInfo.numLayers;
	network.nodeLayers = new NodeLayer[netInfo.numLayers];
	for (int i = 0; i < netInfo.numLayers; i++)
	{
		network.nodeLayers[i].numNodes = netInfo.numRows[i];
		network.nodeLayers[i].nodes = new Node[netInfo.numRows[i]];
	}

	network.numWeightsLayers = netInfo.numLayers - 1;
	network.weightLayers = new WeightLayer[network.numWeightsLayers];
	for (int i = 0; i < network.numWeightsLayers; i++)
	{
		network.weightLayers[i].numWeightRows = netInfo.numRows[i];
		network.weightLayers[i].weightRows = new WeightRow[netInfo.numRows[i]];
		int weights = i < network.numNodeLayers ? network.nodeLayers[i + 1].numNodes : 0;
		for (int j = 0; j < netInfo.numRows[i]; j++)
		{
			network.weightLayers[i].weightRows[j].numWeights = network.nodeLayers[weights].numNodes;
			network.weightLayers[i].weightRows[j].weights = new Weight[network.nodeLayers[weights].numNodes];
		}
	}
}

double activationFunc(double value, double bias)
{
	return 1 / (1 + pow(EULER, -value + bias));
}

double evaluate(int NodeLayer, int NodeRow)
{
	double result = 0;
	return result;
}

int main()
{
	Network network;
	NetworkInfo netInfo;
	netInfo.numLayers = 5;
	netInfo.numRows = new int[netInfo.numLayers] { 2, 3, 5, 5, 2 };

	initNetwork(network, netInfo);

}