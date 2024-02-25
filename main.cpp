#include <iostream>
#include <cmath>
#include <stdarg.h>
#include <conio.h>

struct Node
{
	double value;
	double bias;
};

struct NodeLayer
{
	int numNodes;
	Node* nodes;
};

struct Weight
{
	double weight;
};


struct WeightRow
{
	int numWeights;
	Weight* weights;
};

struct WeightLayer
{
	int numWeightRows;
	WeightRow* weightRows;
};

struct Network
{
	int numNodeLayers;
	NodeLayer* nodeLayers;
	int numWeightsLayers;
	WeightLayer* weightLayers;
};

struct NetworkInfo
{
	int numLayers;
	int* numRows;
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
	network.weightLayers = new WeightLayer[netInfo.numLayers - 1];
	for (int i = 0; i < netInfo.numLayers - 1; i++)
	{
		int row = i + 1 < netInfo.numLayers ? i + 1 : 0;
		network.weightLayers[i].numWeightRows = netInfo.numRows[row];
		network.weightLayers[i].weightRows = new WeightRow[netInfo.numRows[row]];

		int weights = network.nodeLayers[row].numNodes;
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

void initNetworkInfo(int layers, NetworkInfo& netInfo, ...)
{
	va_list vl;
	va_start(vl, layers);
	netInfo.numLayers = layers;
	netInfo.numRows = new int[layers];
	for (int i = 0; i < layers; i++)
	{
		netInfo.numRows[i] = va_arg(vl, int);
	}
	va_end(vl);
}

int main()
{
	Network network;
	NetworkInfo netInfo;

	initNetworkInfo(5, netInfo, 2, 3, 5, 5, 2);
	initNetwork(network, netInfo);

	char ch = getch();
}