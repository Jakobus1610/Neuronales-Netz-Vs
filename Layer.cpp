#include "Layer.h"

Layer::Layer()
{
	nodesIn = 0;
	nodesOut = 0;
	weights = vector<double>(nodesIn * nodesOut);
	grdWeighs = vector<double>(weights.size());
	biases = vector<double>(nodesOut);
	grdBiases = vector<double>(biases.size());
}

Layer::Layer(int nodesIn_in, int nodesOut_in)
{
	nodesIn = nodesIn_in;
	nodesOut = nodesOut_in;
	NetUtils::Log(Debug, "Assinged values");
	weights = vector<double>(nodesIn * nodesOut);
	grdWeighs = vector<double>(weights.size());
	biases = vector<double>(nodesOut);
	grdBiases = vector<double>(biases.size());
	NetUtils::Log(Debug, "Assinged vectors");
	InitRndWeights();
	NetUtils::Log(Debug, "Assinged rng weights");
}

vector<double> Layer::EvaluateLayer(vector<double> inputs)
{
	vector<double> activations(nodesOut);
	for (int nodeOut = 0; nodeOut < nodesOut; nodeOut++)
	{
		for (int nodeIn = 0; nodeIn < nodesIn; nodeIn++)
		{
			double weightedInput = inputs[nodeIn] * GetWeight(nodeIn, nodeOut) + biases[nodeOut];
			activations[nodeOut] = NetUtils::ActivationFunc(weightedInput);
		}
	}
	return activations;
}

void Layer::UpdateGradients()
{
}

void Layer::ApplyGradients()
{
}

double Layer::GetWeight(int nodeIn, int nodeOut)
{
	int flatIndex = nodeOut * nodesIn + nodeIn;
	return weights[flatIndex];
}

void Layer::InitRndWeights()
{
	NetUtils::Log(Debug, "weights.size(): " + ts(weights.size()));
	for (int weightIndex = 0; weightIndex < weights.size(); weightIndex++)
	{
		NetUtils::Log(Debug, "weightIndex: " + ts(weightIndex));
		weights[weightIndex] = NetUtils::RandomNumer(0.0, 1.0);
	}
}
