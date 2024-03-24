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

vector<double> Network::EvaluateLayers(vector<double> inputs)
{
	for (int indexLayer = 0; indexLayer < numLayers; indexLayer++)
	{
		inputs = layers[indexLayer].EvaluateLayer(inputs);
	}
	return inputs;
}
