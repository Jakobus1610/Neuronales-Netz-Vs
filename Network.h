#ifndef NETWORK_INCLUDE
#define NETWORK_INCLUDE 1
#include "Includes.h"
#include "Layer.h"

class Network
{
public:
	int numLayers;
	vector<int> layerSizes;
	vector<Layer> layers;
	Network(int numLayers_in, ...);
	Network(vector<int> numInputs);
	vector<double> EvaluateLayers(vector<double> inputs);
	static void SaveNetwork(string path, Network network);
	static Network LoadNetwork(string path);
};
#endif