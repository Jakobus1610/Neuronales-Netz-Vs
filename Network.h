//#ifndef NETWORK_INCLUDE
//#define NETWORK_INCLUDE 1
//#include "Includes.h"
#pragma once
#include "pch.h"
#include "Layer.h"

class Network
{
public:
	int numLayers;
	vector<int> layerSizes;
	vector<Layer> layers;
	Network();
	Network(int numLayers_in, ...);
	Network(vector<int> numInputs);
	vector<double> EvaluateLayers(vector<double> inputs);
	void initLayerWeightsRnd();
	static void SaveNetwork(string path, Network network);
	static Network LoadNetwork(string path);
};
//#endif