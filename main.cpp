#include <iostream>
#include <fstream>
#include <string>
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
	double value;
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
	if ((netInfo.numLayers - 1) < 0)
	{
		std::cout << "initNetwork bruuu: " << (netInfo.numLayers - 1) << std::endl;
		return;
	}
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
			//	Not sure
			network.weightLayers[i].weightRows[j].numWeights = weights;
			network.weightLayers[i].weightRows[j].weights = new Weight[weights];
		}
	}
}

double activationFunc(double value, double bias)
{
	return 1 / (1 + pow(EULER, -value + bias));
}

NetworkInfo initNetworkInfo(int layers, ...)
{
	va_list vl;
	va_start(vl, layers);
	NetworkInfo netInfo;
	netInfo.numLayers = layers;
	netInfo.numRows = new int[layers];
	for (int i = 0; i < layers; i++)
	{
		int value = va_arg(vl, int);
		std::cout << "Value from index " << i << ": " << value << std::endl;
		netInfo.numRows[i] = value;
	}
	va_end(vl);
	return netInfo;
}

void evaluateNetwork(Network& network, double* input, double* output)
{
	for (int layer = 0; layer < network.numNodeLayers; layer++)
	{
		if (layer == 0)
		{
			for (int node = 0; node < network.nodeLayers[layer].numNodes; node++)
			{
				network.nodeLayers[layer].nodes[node].value = input[node];
				network.nodeLayers[layer].nodes[node].value = 0;
			}
		}
		else
		{
			for (int node = 0; node < network.nodeLayers[layer].numNodes; node++)
			{
				double newTotalValue = 0;
				for (int prevNode = 0; prevNode < network.nodeLayers[layer - 1].numNodes; prevNode++)
				{
					double newValue = 0;
					newValue = network.nodeLayers[layer].nodes[prevNode].value;
					newValue *= network.weightLayers[layer - 1].weightRows[node].weights[prevNode].value;
					newTotalValue += newValue;
				}
				network.nodeLayers[layer].nodes[node].value = activationFunc(newTotalValue, network.nodeLayers[layer].nodes[node].bias);
			}
			if (layer == network.numNodeLayers - 1)
			{
				for (int node = 0; node < network.nodeLayers[layer].numNodes; node++)
				{
					output[node] = network.nodeLayers[layer].nodes[node].value;
				}
			}
		}
	}
}

void saveNetwork(Network network, std::string path)
{
	std::ofstream outputFile;
	outputFile.open(path);
	std::cout << "writing Network to " << path << std::endl;
	for (int layer = 0; layer < network.numNodeLayers; layer++)
	{
		int layers = network.numNodeLayers;
		int rows = network.nodeLayers[layer].numNodes;
		outputFile << "S:" << layers << "," << layer << "," << rows << std::endl;
	}
	for (int layer = 0; layer < network.numNodeLayers; layer++)
	{
		for (int node = 0; node < network.nodeLayers[layer].numNodes; node++)
		{
			int value = network.nodeLayers[layer].nodes[node].value;
			int bias = network.nodeLayers[layer].nodes[node].bias;
			outputFile << "N:" << layer << "," << node << "," << value << "," << bias << std::endl;
		}
	}
	for (int layer = 0; layer < network.numWeightsLayers; layer++)
	{
		for (int row = 0; row < network.weightLayers[layer].numWeightRows; row++)
		{
			for (int weight = 0; weight < network.weightLayers[layer].weightRows[row].numWeights; weight++)
			{
				int value = network.weightLayers[layer].weightRows[row].weights[weight].value;
				outputFile << "W:" << layer << "," << row << "," << weight << "," << value << std::endl;
			}
		}
	}
	outputFile.close();
	std::cout << "finnished writing Network to " << path << std::endl;
}

void interpretLine(std::string line)
{
	// only an example
	//char line[14] = "W: 0, 2, 0, 0";

	char mode = line[0];
	int numbers[4];

	switch (mode)
	{
	case 'S':
		// Handle the structure of the network
		break;
	case 'N':
		// Handle the value of a single node in the network
		break;
	case 'W':
		// Handle the value of a single weight in the network
		break;
	default:
		break;
	}
}

void readNetwork(Network& network, std::string path)
{
	std::ifstream inputFile;
	inputFile.open(path);
	//	ToDo
	//	- write function that can read and interpret a single line
	//	- init the Network with the correct values
	//	- Loop over all and write them
	inputFile.close();
}

void printNetwork(Network network)
{
	std::cout << "printing Network Stucture:\nS: Layers, Index, Rows" << std::endl;
	for (int layer = 0; layer < network.numNodeLayers; layer++)
	{
		int layers = network.numNodeLayers;
		int rows = network.nodeLayers[layer].numNodes;
		std::cout << "S: " << layers << ", " << layer << ", " << rows << std::endl;
	}

	std::cout << "printing Nodes:\nN: Layer, Node, Value, Bias" << std::endl;
	for (int layer = 0; layer < network.numNodeLayers; layer++)
	{
		for (int node = 0; node < network.nodeLayers[layer].numNodes; node++)
		{
			int value = network.nodeLayers[layer].nodes[node].value;
			int bias = network.nodeLayers[layer].nodes[node].bias;
			std::cout << "N: " << layer << ", " << node << ", " << value << ", " << bias << std::endl;
		}
	}

	std::cout << "printing Weights:\nW: Layer, Row, Weight, Value" << std::endl;
	for (int layer = 0; layer < network.numWeightsLayers; layer++)
	{
		for (int row = 0; row < network.weightLayers[layer].numWeightRows; row++)
		{
			for (int weight = 0; weight < network.weightLayers[layer].weightRows[row].numWeights; weight++)
			{
				int value = network.weightLayers[layer].weightRows[row].weights[weight].value;
				std::cout << "W: " << layer << ", " << row << ", " << weight << ", " << value << std::endl;
			}
		}
	}
}

int main()
{
	Network network;
	NetworkInfo netInfo;

	std::cout << "bef init net info\n";
	netInfo = initNetworkInfo(5, 2, 3, 5, 5, 2);
	std::cout << "aft init net info\n";
	//initNetworkInfo(3, netInfo, 784, 200, 10);
	std::cout << "bef init net\n";

	//	Error is here!
	try {
		initNetwork(network, netInfo);
	}
	catch (std::bad_array_new_length& e) {
		std::cerr << "bad_array_new_length caught: " << e.what() << '\n';
	}

	/*
	bef init net info
	Value from index 0: -1248070104
	Value from index 1: 2
	Value from index 2: 3
	Value from index 3: 5
	Value from index 4: 5
	aft init net info
	bef init net
	bad_array_new_length caught: bad array new length
	aft init net
	*/

	std::cout << "aft init net\n";

	//_sleep(1000);

	printNetwork(network);
	saveNetwork(network, "savedNetwork.txt");

	char ch = getch();
}