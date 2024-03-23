#pragma once
#include "Utils.h"
#include "Layer.h"

class Network
{
public:
	int numLayers;
	vector<int> layerSizes;
	vector<Layer> layers;
	Network(int numLayers_in, ...);
	vector<double> EvaluateLayers(vector<double> inputs);
};