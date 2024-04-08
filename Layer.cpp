#include "Layer.h"
//#include "pch.h"

Layer::Layer()
{
	nodesIn = 0;
	nodesOut = 0;
	weights = vector<double>(nodesIn * nodesOut);
	grdWeighs = vector<double>(weights.size());
	biases = vector<double>(nodesOut);
	grdBiases = vector<double>(biases.size());
}

Layer::Layer(int nodesIn_in, int nodesOut_in, bool initWeights)
{
	nodesIn = nodesIn_in;
	nodesOut = nodesOut_in;
	NetUtils::Log(Debug, "Assinged values");
	weights = vector<double>(nodesIn * nodesOut);
	grdWeighs = vector<double>(weights.size());
	biases = vector<double>(nodesOut);
	grdBiases = vector<double>(biases.size());
	NetUtils::Log(Debug, "Assinged vectors");
	if (initWeights) { InitRndWeights(); }
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

string Layer::saveLayer()
{
	//string output;
	stringstream strStream;
	//output += "i," + ts(nodesIn) + ",";
	//output += "o," + ts(nodesOut) + ",";
	//output += "b,";
	strStream << "b,";
	for (int biasIndex = 0; biasIndex < biases.size(); biasIndex++)
	{
		//output += ts(biases[biasIndex]) + ",";
		//strStream << biases[biasIndex] << ",";
		strStream << format("{}", biases[biasIndex]) << ",";
	}
	strStream << "t,w,";
	//output += "t,w,";
	for (int weightIndex = 0; weightIndex < weights.size(); weightIndex++)
	{
		//output += ts(weights[weightIndex]) + ",";
		//std::format("{}", std::numbers::pi_v<double>)
		strStream << format("{}", weights[weightIndex]) << ",";
		//strStream << weights[weightIndex] << ",";
	}
	//output += "t,";
	strStream << "t,";
	return strStream.str();
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
		//NetUtils::Log(Debug, "weightIndex: " + ts(weightIndex));
		weights[weightIndex] = NetUtils::RandomNumer(0.0, 1.0);
	}
	NetUtils::Log(Debug, "Finished with the Weights");
}
