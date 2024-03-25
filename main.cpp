#include "Includes.h"

#include "Network.h"

//#include <climits>
//#include <concepts>
//#include <cstdint>
////#include <iostream>
//#include <type_traits>
//#include <utility>
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <vector>
//#include <string>
//#include <cmath>
//#include <stdarg.h>
//#include <chrono>
//#include "pngwriter.h"

//#define DebugLog(x)// cout << x << endl;
//#define Log(x) //cout << x << endl;

//using namespace std;
//using namespace std::chrono;

struct Node
{
	double value = 0;
	double bias = 0;
	double gradientBias = 0;
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
	double gradientValue = 0;
};

//	Test comment

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

struct NetworkStruct
{
	int numNodeLayers = 0;
	vector<NodeLayer> nodeLayers;
	//NodeLayer* nodeLayers;
	int numWeightsLayers = 0;
	vector<WeightLayer> weightLayers;
	//WeightLayer* weightLayers;
	//bool NetworkStruct::operator==(const NetworkStruct& net);
};

/*bool NetworkStruct::operator==(const NetworkStruct& net)
{
	if (net.numNodeLayers == numNodeLayers &&
		net.numWeightsLayers == numWeightsLayers)
		return true;
	return false;
}*/

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

void StartWatch()
{
	startT = high_resolution_clock::now();
	//Log("start Watch");
}

void StopWatch()
{
	stopT = high_resolution_clock::now();
	duration<double, milli> timeDif = duration_cast<duration<double, milli>>(stopT - startT);
	//Log("stop Watch");
	//Log("Duration: " + to_string(timeDif.count()) + " milliseconds");
}

void InitNetwork(NetworkStruct& network, NetworkInfo netInfo)
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

double ActivationFunc(double value)
{
	return 1 / (1 + pow(EULER, -value));
}

double ActivationFuncDervivative(double value)
{
	double activation = ActivationFunc(value);
	return activation * (1 - activation);
}

double CostFunc(double input, double value)
{
	double res = input - value;
	return (res * res);
}

double CostFuncDerivative(double input, double value)
{
	double res = input - value;
	return 2 * res;
}

NetworkInfo InitNetworkInfo(int layers, ...)
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

void EvaluateNetwork(NetworkStruct& network, vector<double> input)
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
				newTotalValue += network.nodeLayers[layer].nodes[node].bias;
				network.nodeLayers[layer].nodes[node].value = ActivationFunc(newTotalValue);
			}
			/*if (layer == network.numNodeLayers - 1)
			{
				for (int node = 0; node < network.nodeLayers[layer].numNodes; node++)
				{
					output[node] = network.nodeLayers[layer].nodes[node].value;
				}
			}*/
		}
	}
}

void OpenTrainingSet(string path)
{
	trainingSet.open(path);
}

void CloseTrainingSet()
{
	trainingSet.close();
}

void GetNetDigit(int& number, vector<double>& output)
{
	if (!trainingSet.is_open())
	{
		cout << "Abort. Trainingset not opend" << endl;
		return;
	}
	output.resize(784);

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
			int arg = stoi(argument);
			double result = (double)arg / 255.0;
			output[index] = result;
			//cout << argument << endl;
		//	printf("res: %f\n", result);
			//cout << result << endl;
			//cout << output[index] << endl;
		//	printf("out: %f\n", output[index]);
			//output.push_back(stod(argument));
			index++;
		}

	}
	//currentLineTrainingSet = currentLineTrainingSet < 50000 ? currentLineTrainingSet + 1 : 0;
}

void PlotNumber()
{
	vector<double> vec;
	int num;
	GetNetDigit(num, vec);
	string name = "testNum" + to_string(num) + ".png";
	pngwriter png(28, 28, 0, name.c_str());
	cout << num << endl;
	int c = 0;
	for (int y = 27; y >= 0; y--)
	{
		for (int x = 0; x < 28; x++)
		{
			png.plot(x, y, vec[c], vec[c], vec[c]);
			//cout << c << ", " << vec[c] << endl;
			//cout << c << ", " << vec2[c] << endl;
			c++;
		}
	}
	png.close();
	cout << endl;
}

string GetDataFormat(string mode, int layer, int row, double value, double bias)
{
	return mode + "," + to_string(layer) + "," + to_string(row) + "," + to_string(value) + "," + to_string(bias) + "\n";
}

string GetDataFormat(string mode, int layer, int row, int weight, double value)
{
	return mode + "," + to_string(layer) + "," + to_string(row) + "," + to_string(weight) + "," + to_string(value) + "\n";
}

string GetDataFormat(string mode, int layers, int row, int nodes)
{
	return mode + "," + to_string(layers) + "," + to_string(row) + "," + to_string(nodes) + "\n";
}

void SaveNetwork(NetworkStruct network, string path)
{
	ofstream outputFile;
	outputFile.open(path);
	cout << "writing Network to " << path << endl;
	for (int layer = 0; layer < network.numNodeLayers; layer++)
	{
		int layers = network.numNodeLayers;
		int rows = network.nodeLayers[layer].numNodes;
		//outputFile << "S:" << layers << "," << layer << "," << rows << endl;
		outputFile << GetDataFormat("s", layers, layer, rows);
	}
	for (int layer = 0; layer < network.numNodeLayers; layer++)
	{
		for (int node = 0; node < network.nodeLayers[layer].numNodes; node++)
		{
			int value = network.nodeLayers[layer].nodes[node].value;
			int bias = network.nodeLayers[layer].nodes[node].bias;
			//outputFile << "N:" << layer << "," << node << "," << value << "," << bias << endl;
			outputFile << GetDataFormat("n", layer, node, value, bias);
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
				outputFile << GetDataFormat("w", layer, row, weight, value);
			}
		}
	}
	outputFile.close();
	cout << "finnished writing Network to " << path << endl;
}

void ReadNetwork(NetworkStruct& network, string path)
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
		//DebugLog("Get SS: " + line);
		while (getline(ss, argument, ','))
		{
			switch (modeCounter)
			{
			case 0:
				//DebugLog("Case 0");
				if (argument == "s")
					mode = S_MODE;
				if (argument == "n")
					mode = N_MODE;
				if (argument == "w")
					mode = W_MODE;
				break;

				//	Case 1:
			case 1:
				//DebugLog("Case 1");
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
						InitNetwork(network, netInfo);
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
				//DebugLog("Case 2");
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
				//DebugLog("Case 3");
				switch (mode)
				{
				case S_MODE:
					//	DebugLog("Get Num");
					iNum3 = stoi(argument);
					//	DebugLog("success");
					//	DebugLog("Fill Row");
					//	DebugLog("Num2: " + to_string(iNum2) + ", Num3: " + to_string(iNum3));
					netInfo.numRows[iNum2] = iNum3;
					//	DebugLog("success");
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
				//				DebugLog("Case 4");
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
				//	DebugLog("Default");
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

void PrintNetwork(NetworkStruct network)
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
	//	Network network;
	//	NetworkInfo netInfo;

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
		/*
		Training Set CSV:
		https://drive.google.com/file/d/1w6guG9fW3jA1D4_VeadwZwf4G86HBgQ3/view?usp=drive_link
		*/
		//	OpenTrainingSet("mnist_train.csv");
		//	PlotNumber();
		//	PlotNumber();
		//	PlotNumber();
		//	CloseTrainingSet();

	//Network network(4, 2, 5, 3, 2);

	//DebugLog("After Read");

	NetUtils::LogStatus = Info | Debug | Warning | Error;

	//Network network(3, 2, 3, 2);
	vector<int> layers{ 3, 2, 3, 2 };
	Network network(layers);
	NetUtils::Log(Info, "Created Network");
	vector<double> input{ 1.0, 2.0 };
	vector<double> output = network.EvaluateLayers(input);
	NetUtils::Log(Info, "Evaluated Network");

	for (auto& itr : input)
	{
		NetUtils::Log(Debug, "Inputs: " + to_string(itr));
	}
	for (auto& itr : output)
	{
		NetUtils::Log(Warning, "Output: " + to_string(itr));
	}

	//string path = "networkSave.txt";
	//Network::SaveNetwork(path, network);
	//Network loadedNetwork = Network::LoadNetwork(path);

	//output = loadedNetwork.EvaluateLayers(input);

	/*for (auto& itr : output)
	{
		NetUtils::Log(Warning, "Output: " + to_string(itr));
	}*/

	NetUtils::Log(Info, "\nPress key to exit");

	char ch;
	cin >> ch;
}

/*
using static System.Math;

public class Layer
{
	public readonly int numNodesIn;
	public readonly int numNodesOut;

	public readonly double[] weights;
	public readonly double[] biases;

	// Cost gradient with respect to weights and with respect to biases
	public readonly double[] costGradientW;
	public readonly double[] costGradientB;

	// Used for adding momentum to gradient descent
	public readonly double[] weightVelocities;
	public readonly double[] biasVelocities;

	public IActivation activation;

	// Create the layer
	public Layer(int numNodesIn, int numNodesOut, System.Random rng)
	{
		this.numNodesIn = numNodesIn;
		this.numNodesOut = numNodesOut;
		activation = new Activation.Sigmoid();

		weights = new double[numNodesIn * numNodesOut];
		costGradientW = new double[weights.Length];
		biases = new double[numNodesOut];
		costGradientB = new double[biases.Length];

		weightVelocities = new double[weights.Length];
		biasVelocities = new double[biases.Length];

		InitializeRandomWeights(rng);
	}

	// Calculate layer output activations
	public double[] CalculateOutputs(double[] inputs)
	{
		double[] weightedInputs = new double[numNodesOut];

		for (int nodeOut = 0; nodeOut < numNodesOut; nodeOut++)
		{
			double weightedInput = biases[nodeOut];

			for (int nodeIn = 0; nodeIn < numNodesIn; nodeIn++)
			{
				weightedInput += inputs[nodeIn] * GetWeight(nodeIn, nodeOut);
			}
			weightedInputs[nodeOut] = weightedInput;
		}

		// Apply activation function
		double[] activations = new double[numNodesOut];
		for (int outputNode = 0; outputNode < numNodesOut; outputNode++)
		{
			activations[outputNode] = activation.Activate(weightedInputs, outputNode);
		}

		return activations;
	}

	// Calculate layer output activations and store inputs/weightedInputs/activations in the given learnData object
	public double[] CalculateOutputs(double[] inputs, LayerLearnData learnData)
	{
		learnData.inputs = inputs;

		for (int nodeOut = 0; nodeOut < numNodesOut; nodeOut++)
		{
			double weightedInput = biases[nodeOut];
			for (int nodeIn = 0; nodeIn < numNodesIn; nodeIn++)
			{
				weightedInput += inputs[nodeIn] * GetWeight(nodeIn, nodeOut);
			}
			learnData.weightedInputs[nodeOut] = weightedInput;
		}

		// Apply activation function
		for (int i = 0; i < learnData.activations.Length; i++)
		{
			learnData.activations[i] = activation.Activate(learnData.weightedInputs, i);
		}

		return learnData.activations;
	}

	// Update weights and biases based on previously calculated gradients.
	// Also resets the gradients to zero.
	public void ApplyGradients(double learnRate, double regularization, double momentum)
	{
		double weightDecay = (1 - regularization * learnRate);

		for (int i = 0; i < weights.Length; i++)
		{
			double weight = weights[i];
			double velocity = weightVelocities[i] * momentum - costGradientW[i] * learnRate;
			weightVelocities[i] = velocity;
			weights[i] = weight * weightDecay + velocity;
			costGradientW[i] = 0;
		}


		for (int i = 0; i < biases.Length; i++)
		{
			double velocity = biasVelocities[i] * momentum - costGradientB[i] * learnRate;
			biasVelocities[i] = velocity;
			biases[i] += velocity;
			costGradientB[i] = 0;
		}
	}

	// Calculate the "node values" for the output layer. This is an array containing for each node:
	// the partial derivative of the cost with respect to the weighted input
	public void CalculateOutputLayerNodeValues(LayerLearnData layerLearnData, double[] expectedOutputs, ICost cost)
	{
		for (int i = 0; i < layerLearnData.nodeValues.Length; i++)
		{
			// Evaluate partial derivatives for current node: cost/activation & activation/weightedInput
			double costDerivative = cost.CostDerivative(layerLearnData.activations[i], expectedOutputs[i]);
			double activationDerivative = activation.Derivative(layerLearnData.weightedInputs, i);
			layerLearnData.nodeValues[i] = costDerivative * activationDerivative;
		}
	}

	// Calculate the "node values" for a hidden layer. This is an array containing for each node:
	// the partial derivative of the cost with respect to the weighted input
	public void CalculateHiddenLayerNodeValues(LayerLearnData layerLearnData, Layer oldLayer, double[] oldNodeValues)
	{
		for (int newNodeIndex = 0; newNodeIndex < numNodesOut; newNodeIndex++)
		{
			double newNodeValue = 0;
			for (int oldNodeIndex = 0; oldNodeIndex < oldNodeValues.Length; oldNodeIndex++)
			{
				// Partial derivative of the weighted input with respect to the input
				double weightedInputDerivative = oldLayer.GetWeight(newNodeIndex, oldNodeIndex);
				newNodeValue += weightedInputDerivative * oldNodeValues[oldNodeIndex];
			}
			newNodeValue *= activation.Derivative(layerLearnData.weightedInputs, newNodeIndex);
			layerLearnData.nodeValues[newNodeIndex] = newNodeValue;
		}

	}

	public void UpdateGradients(LayerLearnData layerLearnData)
	{
		// Update cost gradient with respect to weights (lock for multithreading)
		lock (costGradientW)
		{
			for (int nodeOut = 0; nodeOut < numNodesOut; nodeOut++)
			{
				double nodeValue = layerLearnData.nodeValues[nodeOut];
				for (int nodeIn = 0; nodeIn < numNodesIn; nodeIn++)
				{
					// Evaluate the partial derivative: cost / weight of current connection
					double derivativeCostWrtWeight = layerLearnData.inputs[nodeIn] * nodeValue;
					// The costGradientW array stores these partial derivatives for each weight.
					// Note: the derivative is being added to the array here because ultimately we want
					// to calculate the average gradient across all the data in the training batch
					costGradientW[GetFlatWeightIndex(nodeIn, nodeOut)] += derivativeCostWrtWeight;
				}
			}
		}

		// Update cost gradient with respect to biases (lock for multithreading)
		lock (costGradientB)
		{
			for (int nodeOut = 0; nodeOut < numNodesOut; nodeOut++)
			{
				// Evaluate partial derivative: cost / bias
				double derivativeCostWrtBias = 1 * layerLearnData.nodeValues[nodeOut];
				costGradientB[nodeOut] += derivativeCostWrtBias;
			}
		}
	}

	public double GetWeight(int nodeIn, int nodeOut)
	{
		int flatIndex = nodeOut * numNodesIn + nodeIn;
		return weights[flatIndex];
	}

	public int GetFlatWeightIndex(int inputNeuronIndex, int outputNeuronIndex)
	{
		return outputNeuronIndex * numNodesIn + inputNeuronIndex;
	}

	public void SetActivationFunction(IActivation activation)
	{
		this.activation = activation;
	}

	public void InitializeRandomWeights(System.Random rng)
	{
		for (int i = 0; i < weights.Length; i++)
		{
			weights[i] = RandomInNormalDistribution(rng, 0, 1) / Sqrt(numNodesIn);
		}

		double RandomInNormalDistribution(System.Random rng, double mean, double standardDeviation)
		{
			double x1 = 1 - rng.NextDouble();
			double x2 = 1 - rng.NextDouble();

			double y1 = Sqrt(-2.0 * Log(x1)) * Cos(2.0 * PI * x2);
			return y1 * standardDeviation + mean;
		}
	}
}


	Next:



public class NeuralNetwork
{
	public readonly Layer[] layers;
	public readonly int[] layerSizes;

	public ICost cost;
	System.Random rng;
	NetworkLearnData[] batchLearnData;

	// Create the neural network
	public NeuralNetwork(params int[] layerSizes)
	{
		this.layerSizes = layerSizes;
		rng = new System.Random();

		layers = new Layer[layerSizes.Length - 1];
		for (int i = 0; i < layers.Length; i++)
		{
			layers[i] = new Layer(layerSizes[i], layerSizes[i + 1], rng);
		}

		cost = new Cost.MeanSquaredError();
	}

	// Run the inputs through the network to predict which class they belong to.
	// Also returns the activations from the output layer.
	public (int predictedClass, double[] outputs) Classify(double[] inputs)
	{
		var outputs = CalculateOutputs(inputs);
		int predictedClass = MaxValueIndex(outputs);
		return (predictedClass, outputs);
	}

	// Run the inputs through the network to calculate the outputs
	public double[] CalculateOutputs(double[] inputs)
	{
		foreach (Layer layer in layers)
		{
			inputs = layer.CalculateOutputs(inputs);
		}
		return inputs;
	}


	public void Learn(DataPoint[] trainingData, double learnRate, double regularization = 0, double momentum = 0)
	{

		if (batchLearnData == null || batchLearnData.Length != trainingData.Length)
		{
			batchLearnData = new NetworkLearnData[trainingData.Length];
			for (int i = 0; i < batchLearnData.Length; i++)
			{
				batchLearnData[i] = new NetworkLearnData(layers);
			}
		}

		System.Threading.Tasks.Parallel.For(0, trainingData.Length, (i) =>
		{
			UpdateGradients(trainingData[i], batchLearnData[i]);
		});


		// Update weights and biases based on the calculated gradients
		for (int i = 0; i < layers.Length; i++)
		{
			layers[i].ApplyGradients(learnRate / trainingData.Length, regularization, momentum);
		}
	}


	void UpdateGradients(DataPoint data, NetworkLearnData learnData)
	{
		// Feed data through the network to calculate outputs.
		// Save all inputs/weightedinputs/activations along the way to use for backpropagation.
		double[] inputsToNextLayer = data.inputs;

		for (int i = 0; i < layers.Length; i++)
		{
			inputsToNextLayer = layers[i].CalculateOutputs(inputsToNextLayer, learnData.layerData[i]);
		}

		// -- Backpropagation --
		int outputLayerIndex = layers.Length - 1;
		Layer outputLayer = layers[outputLayerIndex];
		LayerLearnData outputLearnData = learnData.layerData[outputLayerIndex];

		// Update output layer gradients
		outputLayer.CalculateOutputLayerNodeValues(outputLearnData, data.expectedOutputs, cost);
		outputLayer.UpdateGradients(outputLearnData);

		// Update all hidden layer gradients
		for (int i = outputLayerIndex - 1; i >= 0; i--)
		{
			LayerLearnData layerLearnData = learnData.layerData[i];
			Layer hiddenLayer = layers[i];

			hiddenLayer.CalculateHiddenLayerNodeValues(layerLearnData, layers[i + 1], learnData.layerData[i + 1].nodeValues);
			hiddenLayer.UpdateGradients(layerLearnData);
		}

	}

	public void SetCostFunction(ICost costFunction)
	{
		this.cost = costFunction;
	}

	public void SetActivationFunction(IActivation activation)
	{
		SetActivationFunction(activation, activation);
	}

	public void SetActivationFunction(IActivation activation, IActivation outputLayerActivation)
	{
		for (int i = 0; i < layers.Length - 1; i++)
		{
			layers[i].SetActivationFunction(activation);
		}
		layers[layers.Length - 1].SetActivationFunction(outputLayerActivation);
	}


	public int MaxValueIndex(double[] values)
	{
		double maxValue = double.MinValue;
		int index = 0;
		for (int i = 0; i < values.Length; i++)
		{
			if (values[i] > maxValue)
			{
				maxValue = values[i];
				index = i;
			}
		}

		return index;
	}
}


public class NetworkLearnData
{
	public LayerLearnData[] layerData;

	public NetworkLearnData(Layer[] layers)
	{
		layerData = new LayerLearnData[layers.Length];
		for (int i = 0; i < layers.Length; i++)
		{
			layerData[i] = new LayerLearnData(layers[i]);
		}
	}
}

public class LayerLearnData
{
	public double[] inputs;
	public double[] weightedInputs;
	public double[] activations;
	public double[] nodeValues;

	public LayerLearnData(Layer layer)
	{
		weightedInputs = new double[layer.numNodesOut];
		activations = new double[layer.numNodesOut];
		nodeValues = new double[layer.numNodesOut];
	}
}


	0 = Schwarz       8 = Grau
	1 = Blau        9 = Hellblau
	2 = Grün       A = Hellgrün
	3 = Türkis        B = Helltürkis
	4 = Rot         C = Hellrot
	5 = Lila      D = Helllila
	6 = Gelb      E = Hellgelb
	7 = Hellgrau       F = Weiß


*/