#include <iostream>
#include <cmath>
#include <stdarg.h>
//#include <conio.h>

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
	//	Approved
	network.numNodeLayers = netInfo.numLayers;
	network.nodeLayers = new NodeLayer[netInfo.numLayers];

	for (int i = 0; i < netInfo.numLayers; i++)
	{
		network.nodeLayers[i].numNodes = netInfo.numRows[i];
		network.nodeLayers[i].nodes = new Node[netInfo.numRows[i]];
	}

	//	Approved
	network.numWeightsLayers = (netInfo.numLayers - 1);
	network.weightLayers = new WeightLayer[(netInfo.numLayers - 1)];
	for (int i = 0; i < (netInfo.numLayers - 1); i++)
	{
		int row = (i + 1) < netInfo.numLayers ? (i + 1) : 0;
		network.weightLayers[i].numWeightRows = netInfo.numRows[row];
		network.weightLayers[i].weightRows = new WeightRow[netInfo.numRows[row]];

		//	Not sure
		int weights = network.nodeLayers[i].numNodes;
		for (int j = 0; j < netInfo.numRows[row]; j++)
		{
			//	Aswell
			network.weightLayers[i].weightRows[j].numWeights = weights;
			network.weightLayers[i].weightRows[j].weights = new Weight[weights];
		}
	}
}

double activationFunc(double value, double bias)
{
	return 1 / (1 + pow(EULER, -value + bias));
}

void initNetworkInfo(int layers, NetworkInfo& netInfo, ...)
{
	va_list vl;
	va_start(vl, layers);
	netInfo.numLayers = layers;
	netInfo.numRows = new int[layers];
	for (int i = 0; i < layers; i++)
	{
		int value = va_arg(vl, int);
		std::cout << "Value from index " << i << ": " << value << std::endl;
		netInfo.numRows[i] = value;
	}
	va_end(vl);
}

void evaluateNetwork(Network& network, double* input, double* output)
{
	for(int layer = 0; layer < network.numNodeLayers; layer++)
	{
		if(layer == 0)
		{
			for(int node = 0; node < network.nodeLayers[layer].numNodes; node++)
			{
				network.nodeLayers[layer].nodes[node].value = input[node];
				network.nodeLayers[layer].nodes[node].value = 0;
			}
		}
		else
		{
			for(int node = 0; node < network.nodeLayers[layer].numNodes; node++)
			{
				double newTotalValue = 0;
				for(int prevNode = 0; prevNode < network.nodeLayers[layer-1].numNodes; prevNode++)
				{	
					double newValue = 0;
					newValue = network.nodeLayers[layer].nodes[prevNode].value;
					newValue *= network.weightLayers[layer-1].weightRows[node].weights[prevNode].weight;
					newTotalValue += newValue;
				}
				network.nodeLayers[layer].nodes[node].value = activationFunc(newTotalValue, network.nodeLayers[layer].nodes[node].bias);
			}
			if(layer == network.numNodeLayers-1)
			{
				for(int node = 0; node < network.nodeLayers[layer].numNodes; node++)
				{
					output[node] = network.nodeLayers[layer].nodes[node].value;
				}
			}
		}
	}
}

int main()
{
	Network network;
	NetworkInfo netInfo;

	initNetworkInfo(5, netInfo, 2, 3, 5, 5, 2);
	initNetwork(network, netInfo);

	//ch = getch();
}