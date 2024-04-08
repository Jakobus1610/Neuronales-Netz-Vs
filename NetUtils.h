//#ifndef NETUTILS_INCLUDE
//#define NETUTILS_INCLUDE 1
//#include "Includes.h"
#pragma once
#include "pch.h"
#include "pngwriter.h"

enum Status {
	Info = 0b1000,
	Debug = 0b0100,
	Warning = 0b0010,
	Error = 0b0001
};

struct DataPoint
{
	vector<double> values;
	vector<double> expectedValues;
	int number;
};

class NetUtils
{
public:
	static int logStatus;
	static int debugCount;
	static int warningCount;
	static int errorCount;
	static int logCount;
	static ifstream trainingSet;
	static high_resolution_clock::time_point startT;
	static high_resolution_clock::time_point stopT;

	static void Log(Status stat, string str);
	static void PrintLogCounts();
	static void StartWatch();
	static void StopWatch();
	static double ActivationFunc(double value);
	static double ActivationFuncDervivative(double value);
	static double CostFunc(double input, double value);
	static double CostFuncDerivative(double input, double value);
	static double RandomNumer(double min, double max);
	static void OpenTrainingSet(string path);
	static void CloseTrainingSet();
	static DataPoint GetDataPoint();
	static void PlotNumber(DataPoint dataPoint);
	NetUtils() = delete;
};
//#endif