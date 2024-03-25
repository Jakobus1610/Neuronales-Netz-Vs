#ifndef LAYER_INCLUDE
#define LAYER_INCLUDE 1
#include "Includes.h"

class Layer
{
public:
	int nodesIn;
	int nodesOut;
	vector<double> weights;
	vector<double> biases;
	vector<double> grdBiases;
	vector<double> grdWeighs;

	Layer();
	Layer(int nodesIn_in, int nodesOut_in);
	vector<double> EvaluateLayer(vector<double> input);
	void UpdateGradients();
	void ApplyGradients();
	double GetWeight(int nodeIn, int nodeOut);
	void InitRndWeights();
};
#endif