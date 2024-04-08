#include "Network.h"
#include "pch.h"

Network::Network()
{
	numLayers = 0;
	NetUtils::Log(Error, "Created an emtpy Network");
}

Network::Network(int numLayers_in, ...)
{
	//	Argumentlist
	va_list vl;

	// Fill the list with the Arguments
	va_start(vl, numLayers_in);

	//	Set the numder of Layers
	//	Structure:
	//	The inputs are for example 3 Layers. 2, 3, 2.
	//	The Network dosnt save all Layers because the input Layer (the first 2) are only inputs
	//	So it is important to get the diffrence between actual save Layers and ALL Layers
	//	Also the Layer itself stores the weights going in to it.
	numLayers = numLayers_in - 1;

	//	Resize to dont cause issues
	//	We are using numLayers_in because we have a size stored for ALL Layers, so dont just the actual stored ones.
	layerSizes.resize(numLayers_in);

	//	Looping throug layerSizes to assing the size to each Layer
	for (int i = 0; i < numLayers_in; i++)
	{
		int value = va_arg(vl, int);
		NetUtils::Log(Debug, "Value at index " + ts(i) + ": " + ts(value));
		layerSizes[i] = value;
	}

	//	Close the Argumentlist
	va_end(vl);

	NetUtils::Log(Debug, "Finished with filling layerSizes in Network()");

	//	Rezise the stored Layers to the number of real Layers
	layers.resize(numLayers);

	//	Going throug all real Layers and create them
	for (int indexLayer = 1; indexLayer < numLayers + 1; indexLayer++)
	{
		int subIndex = indexLayer - 1;
		NetUtils::Log(Debug, "subIndex: " + ts(subIndex));
		layers[subIndex] = Layer(layerSizes[subIndex], layerSizes[indexLayer], true);
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
		int indexMinusOne = index - 1;
		layerSizes[indexMinusOne] = numInputs[index];
	}

	NetUtils::Log(Debug, "Finished with filling layerSizes in Network()");
	layers.resize(numLayers);
	for (int indexLayer = 1; indexLayer < numLayers + 1; indexLayer++)
	{
		int subIndex = indexLayer - 1;
		NetUtils::Log(Debug, "subIndex: " + ts(subIndex));
		layers[subIndex] = Layer(layerSizes[subIndex], layerSizes[indexLayer], false);
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

void Network::initLayerWeightsRnd()
{
	for (int layerIndex = 0; layerIndex < layers.size(); layerIndex++)
	{
		layers[layerIndex].InitRndWeights();
	}
}

void Network::SaveNetwork(string path, Network network)
{
	NetUtils::Log(Info, "Saving Network to " + path);
	ofstream outputFile;
	NetUtils::Log(Info, "Saving Network step 2");
	outputFile.open(path);
	outputFile << "s," << ts(network.numLayers + 1);
	for (int layerIndex = 0; layerIndex < network.layerSizes.size(); layerIndex++)
	{
		NetUtils::Log(Info, "Saving Network step 3");
		outputFile << "," << ts(network.layerSizes[layerIndex]);
		NetUtils::Log(Info, "Saving Network step 3.1");
	}
	outputFile << ",t" << endl;
	NetUtils::Log(Info, "Saving Network step 4");
	for (int layerIndex = 0; layerIndex < network.layers.size(); layerIndex++)
	{
		NetUtils::Log(Info, "Saving Network step 5");
		outputFile << "l," << layerIndex << "," << network.layers[layerIndex].saveLayer() << "t" << endl;
		NetUtils::Log(Info, "Saving Network step 5.1");
	}
	outputFile.close();
	NetUtils::Log(Info, "Finished Saving Network");
}

Network Network::LoadNetwork(string path)
{
	enum ReadMode {
		STRUCTURE,
		LAYER,
		NODES_IN,
		NODES_OUT,
		BIASES,
		WEIGHTS,
		TERMINATE,
		NONE
	};

	NetUtils::Log(Info, "Loading Network from " + path);

	ifstream inputFile;
	string line, argument;
	stringstream strStream;
	vector<int> numLayersRead;
	int currentLayer = 0;
	int counter = 0;
	Network network;
	//Network network;

	inputFile.open(path);
	if (!inputFile.is_open())
	{
		NetUtils::Log(Error, "Couldt open Network from " + path);
		return Network();
	}
	while (getline(inputFile, line))
	{
		ReadMode currentReadMode = NONE;
		ReadMode previousReadMode = NONE;
		strStream = stringstream(line);
		NetUtils::Log(Debug, "Read a Layer");
		while (getline(strStream, argument, ','))
		{
			if (argument == "s") { currentReadMode = STRUCTURE; continue; }
			if (argument == "l") { currentReadMode = LAYER; continue; }
			if (argument == "b") { currentReadMode = BIASES; continue; }
			if (argument == "w") { currentReadMode = WEIGHTS; continue; }
			if (argument == "t") { previousReadMode = currentReadMode; currentReadMode = TERMINATE; }

			switch (currentReadMode)
			{
			case STRUCTURE:
				numLayersRead.push_back(stoi(argument));
				break;
			case LAYER:
				currentLayer = stoi(argument);
				break;
			case BIASES:
				network.layers[currentLayer].biases[counter] = stod(argument);
				counter++;
				break;
			case WEIGHTS:
				network.layers[currentLayer].weights[counter] = stod(argument);
				counter++;
				break;
			case TERMINATE:
				switch (previousReadMode)
				{
				case STRUCTURE:
					NetUtils::Log(Warning, "Got the following structure for the Network: ");
					for (auto& itr : numLayersRead) { NetUtils::Log(Debug, to_string(itr)); }
					network = Network(numLayersRead);
					break;
				case LAYER:
					break;
				case BIASES:
					counter = 0;
					break;
				case WEIGHTS:
					counter = 0;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}

		}
	}
	NetUtils::Log(Info, "Finished Loading Network");
	return network;
}