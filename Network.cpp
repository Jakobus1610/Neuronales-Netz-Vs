#include "Network.h"

Network::Network(int numLayers_in, ...)
{
	va_list vl;
	va_start(vl, numLayers_in);
	numLayers = numLayers_in - 1;
	layerSizes.resize(numLayers_in);
	for (int i = 0; i < numLayers_in; i++)
	{
		int value = va_arg(vl, int);
		NetUtils::Log(Debug, "Value at index " + ts(i) + ": " + ts(value));
		//cout << "Value from index " << i << ": " << value << endl;
		layerSizes[i] = value;
	}
	va_end(vl);

	NetUtils::Log(Debug, "Finished with filling layerSizes in Network()");
	layers.resize(numLayers);
	for (int indexLayer = 1; indexLayer < numLayers + 1; indexLayer++)
	{
		int subIndex = indexLayer - 1;
		NetUtils::Log(Debug, "subIndex: " + ts(subIndex));
		layers[subIndex] = Layer(layerSizes[subIndex], layerSizes[indexLayer]);
	}
	NetUtils::Log(Debug, "Finished with filling layers in Network()");
}

Network::Network(vector<int> numInputs)
{
	numLayers = numInputs[0] - 1;
	if (numInputs.size() - 1 == numInputs[0])
		NetUtils::Log(Debug, "Ja: " + ts(numLayers));
	else
		NetUtils::Log(Debug, "Nein: " + ts(numLayers));
	layerSizes.resize(numInputs[0]);
	for (int index = 1; index < numInputs.size(); index++)
	{
		layerSizes[index - 1] = numInputs[index];
	}

	NetUtils::Log(Debug, "Finished with filling layerSizes in Network()");
	layers.resize(numLayers);
	for (int indexLayer = 1; indexLayer < numLayers + 1; indexLayer++)
	{
		int subIndex = indexLayer - 1;
		NetUtils::Log(Debug, "subIndex: " + ts(subIndex));
		layers[subIndex] = Layer(layerSizes[subIndex], layerSizes[indexLayer]);
	}
	NetUtils::Log(Debug, "Finished with filling layers in Network()");
}

vector<double> Network::EvaluateLayers(vector<double> inputs)
{
	for (int indexLayer = 0; indexLayer < numLayers; indexLayer++)
	{
		inputs = layers[indexLayer].EvaluateLayer(inputs);
	}
	return inputs;
}

void Network::SaveNetwork(string path, Network network)
{
	NetUtils::Log(Info, "Saving Network to " + path);
	ofstream outputFile;
	outputFile.open(path);
	outputFile << "s," << ts(network.numLayers + 1);
	for (int layerIndex = 0; layerIndex < network.layerSizes.size(); layerIndex++)
	{
		outputFile << "," << ts(network.layerSizes[layerIndex]);
	}
	outputFile << endl;
	for (int layerIndex = 0; layerIndex < network.layerSizes.size(); layerIndex++)
	{

	}
	//outputFile.write((char*)&numLayersWrite, sizeof(numLayersWrite));
	//outputFile.write((char*)&network, sizeof(network));
	outputFile.close();
	NetUtils::Log(Info, "Finished Loading Network");
}

Network Network::LoadNetwork(string path)
{
	NetUtils::Log(Info, "Loading Network from " + path);
	ifstream inputFile;
	inputFile.open(path);
	string line, argument;
	getline(inputFile, line);
	stringstream strStream(line);
	vector<int> numLayersRead;
	bool isFirstArg = true;
	int indexCounter = 0;
	while (getline(strStream, argument, ','))
	{
		if (isFirstArg)
		{
			isFirstArg = false;
			numLayersRead.resize(stoi(argument) + 1);
		}
		numLayersRead[indexCounter] = stoi(argument);
		indexCounter++;
	}
	for (auto& itr : numLayersRead)
	{
		NetUtils::Log(Debug, "numLayersRead: " + to_string(itr));
	}
	Network network(numLayersRead);
	inputFile.read((char*)&network, sizeof(network));
	inputFile.close();
	NetUtils::Log(Info, "Finished Loading Network");
	return network;
}