#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <stdarg.h>
#include <chrono>
#include "pngwriter.h"

#define DebugLog(x)// cout << x << endl;
#define Log(x) cout << x << endl;

using namespace std;
using namespace std::chrono;

struct Node
{
	double value = 0;
	double bias = 0;
};

struct NodeLayer
{
	int numNodes = 0;
	vector<Node> nodes;
	//Node* nodes;
};

struct Weight
{
	double value = 0;
};


struct WeightRow
{
	int numWeights = 0;
	vector<Weight> weights;
	//Weight* weights;
};

struct WeightLayer
{
	int numWeightRows = 0;
	vector<WeightRow> weightRows;
	//WeightRow* weightRows;
};

struct Network
{
	int numNodeLayers = 0;
	vector<NodeLayer> nodeLayers;
	//NodeLayer* nodeLayers;
	int numWeightsLayers = 0;
	vector<WeightLayer> weightLayers;
	//WeightLayer* weightLayers;
	bool Network::operator==(const Network& net);
};

bool Network::operator==(const Network& net)
{
	if (net.numNodeLayers == numNodeLayers &&
		net.numWeightsLayers == numWeightsLayers)
		return true;
	return false;
}


struct NetworkInfo
{
	int numLayers = 0;
	vector<int> numRows;
	//int* numRows;
};

enum Mode { S_MODE, N_MODE, W_MODE, NO_MODE };

const double EULER = 2.71828182845904523536;

ifstream trainingSet;
//int currentLineTrainingSet = 0;

high_resolution_clock::time_point startT;
high_resolution_clock::time_point stopT;

void startTime()
{
	startT = high_resolution_clock::now();
	Log("start Watch");
}

void stopTime()
{
	stopT = high_resolution_clock::now();
	duration<double, milli> timeDif = duration_cast<duration<double, milli>>(stopT - startT);
	Log("stop Watch");
	Log("Duration: " + to_string(timeDif.count()) + " milliseconds");
}

void initNetwork(Network& network, NetworkInfo netInfo)
{
	//	Approved
	network.numNodeLayers = netInfo.numLayers;
	network.nodeLayers.resize(netInfo.numLayers);
	//network.nodeLayers = new NodeLayer[netInfo.numLayers];

	for (int i = 0; i < netInfo.numLayers; i++)
	{
		network.nodeLayers[i].numNodes = netInfo.numRows[i];
		network.nodeLayers[i].nodes.resize(netInfo.numRows[i]);
		//network.nodeLayers[i].nodes = new Node[netInfo.numRows[i]];
	}

	//	Approved
	if ((netInfo.numLayers - 1) < 0)
	{
		cout << "initNetwork bruuu: " << (netInfo.numLayers - 1) << endl;
		return;
	}
	network.numWeightsLayers = (netInfo.numLayers - 1);
	network.weightLayers.resize(netInfo.numLayers - 1);
	//network.weightLayers = new WeightLayer[(netInfo.numLayers - 1)];
	for (int i = 0; i < (netInfo.numLayers - 1); i++)
	{
		int row = (i + 1) < netInfo.numLayers ? (i + 1) : 0;
		network.weightLayers[i].numWeightRows = netInfo.numRows[row];
		network.weightLayers[i].weightRows.resize(netInfo.numRows[row]);
		//network.weightLayers[i].weightRows = new WeightRow[netInfo.numRows[row]];

		//	Not sure
		int weights = network.nodeLayers[i].numNodes;
		for (int j = 0; j < netInfo.numRows[row]; j++)
		{
			//	Not sure
			network.weightLayers[i].weightRows[j].numWeights = weights;
			network.weightLayers[i].weightRows[j].weights.resize(weights);
			//network.weightLayers[i].weightRows[j].weights = new Weight[weights];
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
	netInfo.numRows.resize(layers);
	//netInfo.numRows = new int[layers];
	for (int i = 0; i < layers; i++)
	{
		int value = va_arg(vl, int);
		cout << "Value from index " << i << ": " << value << endl;
		netInfo.numRows[i] = value;
	}
	va_end(vl);
	return netInfo;
}

void evaluateNetwork(Network& network, vector<double> input, vector<double> output)
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

void openTrainingSet(string path)
{
	trainingSet.open(path);
}

void closeTrainingSet()
{
	trainingSet.close();
}

vector<double> getNumberFromSet(int& number, vector<double>& vec)
{
	if (!trainingSet.is_open())
	{
		cout << "Abort. Trainingset not opend" << endl;
		return vector<double>();
	}
	vector<double> output;
	output.resize(784);
	vec.resize(784);

	string line, argument;

	getline(trainingSet, line);
	stringstream ss(line);
	bool is_first_num = true;
	int index = 0;

	number = -1;

	while (getline(ss, argument, ','))
	{
		if (is_first_num)
		{
			number = stoi(argument);
			is_first_num = false;
		}
		else
		{
			double result = (stoi(argument) / static_cast<double>(255));
			output[index] = result;
		//	cout << argument << endl;
		//	cout << result << endl;
		//	cout << output[index] << endl;
			//output.push_back(stod(argument));
		}

	}
	vec = output;
	return output;
	//currentLineTrainingSet = currentLineTrainingSet < 50000 ? currentLineTrainingSet + 1 : 0;
}

void trainNetwork(Network& network)
{

}

void plotNumber()
{
	vector<double> vec;
	vector<double> vec2;
	int num;
	vec = getNumberFromSet(num, vec2);
	string name = "testNum" + to_string(num) + ".png";
	pngwriter png(28, 28, 0, name.c_str());
	cout << num << endl;
	int c = 0;
	for (int x = 0; x < 28; x++)
	{
		for (int y = 0; y < 28; y++)
		{
			png.plot(x, y, vec[c], vec[c], vec[c]);
			cout << c << ", " << vec[c] << endl;
			cout << c << ", " << vec2[c] << endl;
			c++;
		}
	}
	png.close();
	cout << endl;
}

string getDataFormat(string mode, int layer, int row, double value, double bias)
{
	return mode + "," + to_string(layer) + "," + to_string(row) + "," + to_string(value) + "," + to_string(bias) + "\n";
}

string getDataFormat(string mode, int layer, int row, int weight, double value)
{
	return mode + "," + to_string(layer) + "," + to_string(row) + "," + to_string(weight) + "," + to_string(value) + "\n";
}

string getDataFormat(string mode, int layers, int row, int nodes)
{
	return mode + "," + to_string(layers) + "," + to_string(row) + "," + to_string(nodes) + "\n";
}

void saveNetwork(Network network, string path)
{
	ofstream outputFile;
	outputFile.open(path);
	cout << "writing Network to " << path << endl;
	for (int layer = 0; layer < network.numNodeLayers; layer++)
	{
		int layers = network.numNodeLayers;
		int rows = network.nodeLayers[layer].numNodes;
		//outputFile << "S:" << layers << "," << layer << "," << rows << endl;
		outputFile << getDataFormat("s", layers, layer, rows);
	}
	for (int layer = 0; layer < network.numNodeLayers; layer++)
	{
		for (int node = 0; node < network.nodeLayers[layer].numNodes; node++)
		{
			int value = network.nodeLayers[layer].nodes[node].value;
			int bias = network.nodeLayers[layer].nodes[node].bias;
			//outputFile << "N:" << layer << "," << node << "," << value << "," << bias << endl;
			outputFile << getDataFormat("n", layer, node, value, bias);
		}
	}
	for (int layer = 0; layer < network.numWeightsLayers; layer++)
	{
		for (int row = 0; row < network.weightLayers[layer].numWeightRows; row++)
		{
			for (int weight = 0; weight < network.weightLayers[layer].weightRows[row].numWeights; weight++)
			{
				int value = network.weightLayers[layer].weightRows[row].weights[weight].value;
				//outputFile << "W:" << layer << "," << row << "," << weight << "," << value << endl;
				outputFile << getDataFormat("w", layer, row, weight, value);
			}
		}
	}
	outputFile.close();
	cout << "finnished writing Network to " << path << endl;
}

void readNetwork(Network& network, string path)
{
	cout << "reading Network from " << path << endl;
	NetworkInfo netInfo;
	ifstream inputFile;
	inputFile.open(path);

	string line, argument;
	bool initSizeSet = false;
	bool initNet = false;

	while (getline(inputFile, line))
	{
		Mode mode = NO_MODE;
		int modeCounter = 0;
		stringstream ss(line);
		int iNum1 = 0;
		int iNum2 = 0;
		int iNum3 = 0;
		double dNum1 = 0;
		double dNum2 = 0;
		DebugLog("Get SS: " + line);
		while (getline(ss, argument, ','))
		{
			switch (modeCounter)
			{
			case 0:
				DebugLog("Case 0");
				if (argument == "s")
					mode = S_MODE;
				if (argument == "n")
					mode = N_MODE;
				if (argument == "w")
					mode = W_MODE;
				break;

				//	Case 1:
			case 1:
				DebugLog("Case 1");
				switch (mode)
				{
				case S_MODE:
					iNum1 = stoi(argument);
					if (!initSizeSet)
					{
						netInfo.numLayers = iNum1;
						netInfo.numRows.resize(iNum1);
						initSizeSet = true;
					}
					break;
				case N_MODE:
					if (!initNet)
					{
						initNetwork(network, netInfo);
						initNet = true;
					}
					iNum1 = stoi(argument);
					break;
				case W_MODE:
					iNum1 = stoi(argument);
					break;
				default:
					cout << "What happend? Read Network -> switch(mode) = " << mode << endl;
					break;
				}
				break;

				//	Case 2:
			case 2:
				DebugLog("Case 2");
				switch (mode)
				{
				case S_MODE:
					iNum2 = stoi(argument);
					break;
				case N_MODE:
					iNum2 = stoi(argument);
					break;
				case W_MODE:
					iNum2 = stoi(argument);
					break;
				default:
					cout << "What happend? Read Network -> switch(mode) = " << mode << endl;
					break;
				}
				break;

				//	Case 3:
			case 3:
				DebugLog("Case 3");
				switch (mode)
				{
				case S_MODE:
					DebugLog("Get Num");
					iNum3 = stoi(argument);
					DebugLog("success");
					DebugLog("Fill Row");
					DebugLog("Num2: " + to_string(iNum2) + ", Num3: " + to_string(iNum3));
					netInfo.numRows[iNum2] = iNum3;
					DebugLog("success");
					break;
				case N_MODE:
					dNum1 = stod(argument);
					network.nodeLayers[iNum1].nodes[iNum2].value = dNum1;
					break;
				case W_MODE:
					iNum3 = stoi(argument);
					break;
				default:
					cout << "What happend? Read Network -> switch(mode) = " << mode << endl;
					break;
				}
				break;

				//	Case 4:
			case 4:
				DebugLog("Case 4");
				switch (mode)
				{
				case S_MODE:
					break;
				case N_MODE:
					dNum2 = stod(argument);
					network.nodeLayers[iNum1].nodes[iNum2].bias = dNum2;
					break;
				case W_MODE:
					dNum1 = stod(argument);
					network.weightLayers[iNum1].weightRows[iNum2].weights[iNum3].value = dNum1;
					break;
				default:
					cout << "What happend? Read Network -> switch(mode) = " << mode << endl;
					break;
				}
				break;
			default:
				DebugLog("Default");
				cout << "What happend? Read Network -> switch(modeCounter) = " << modeCounter << endl;
				break;
			}
			modeCounter++;
		}
		//cout << mode << "| " << iNum1 << ", " << iNum2 << ", " << iNum3 << "| " << dNum1 << ", " << dNum1 << endl;
	}

	inputFile.close();
	cout << "finished reading Network from " << path << endl;
}

void printNetwork(Network network)
{
	cout << "printing Network Stucture:\nS: Layers, Index, Rows" << endl;
	for (int layer = 0; layer < network.numNodeLayers; layer++)
	{
		int layers = network.numNodeLayers;
		int rows = network.nodeLayers[layer].numNodes;
		cout << "S: " << layers << ", " << layer << ", " << rows << endl;
	}

	cout << "printing Nodes:\nN: Layer, Node, Value, Bias" << endl;
	for (int layer = 0; layer < network.numNodeLayers; layer++)
	{
		for (int node = 0; node < network.nodeLayers[layer].numNodes; node++)
		{
			int value = network.nodeLayers[layer].nodes[node].value;
			int bias = network.nodeLayers[layer].nodes[node].bias;
			cout << "N: " << layer << ", " << node << ", " << value << ", " << bias << endl;
		}
	}

	cout << "printing Weights:\nW: Layer, Row, Weight, Value" << endl;
	for (int layer = 0; layer < network.numWeightsLayers; layer++)
	{
		for (int row = 0; row < network.weightLayers[layer].numWeightRows; row++)
		{
			for (int weight = 0; weight < network.weightLayers[layer].weightRows[row].numWeights; weight++)
			{
				int value = network.weightLayers[layer].weightRows[row].weights[weight].value;
				cout << "W: " << layer << ", " << row << ", " << weight << ", " << value << endl;
			}
		}
	}
}

int main()
{
	Network network;
	NetworkInfo netInfo;

	/*

	netInfo = initNetworkInfo(5, 2, 3, 5, 5, 2);
	//netInfo = initNetworkInfo(3, 784, 200, 10);
	initNetwork(network, netInfo);
	//printNetwork(network);
	DebugLog("Before Read");
	startTime();
	saveNetwork(network, "savedNetwork.txt");	// ~ 1540 millis
	stopTime();
	Network network2;
	startTime();
	readNetwork(network2, "savedNetwork.txt");	// ~ 1040 millis
	stopTime();

	if (network == network2)
		Log("Equal");

	*/

	openTrainingSet("mnist_train.csv");
	plotNumber();
	plotNumber();
	plotNumber();
	closeTrainingSet();

	DebugLog("After Read");

	cout << "press Key to close\n";

	char ch;
	cin >> ch;
}