#include <stdarg.h>
#include <cmath>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <random>

#include "Windows.h"

using namespace std;
using namespace std::chrono;

#define ts(x) to_string(x)

enum Status {
	Info = 0b1000,
	Debug = 0b0100,
	Warning = 0b0010,
	Error = 0b0001
};

//	Test 16:36

#pragma once
class NNUtils
{
public:
	static int LogStatus;
	static ifstream trainingSet;
	static high_resolution_clock::time_point startT;
	static high_resolution_clock::time_point stopT;

	static void Log(Status stat, string str);
	static void StartWatch();
	static void StopWatch();
	static double ActivationFunc(double value);
	static double ActivationFuncDervivative(double value);
	static double CostFunc(double input, double value);
	static double CostFuncDerivative(double input, double value);
	static double RandomNumer(double min, double max);
	static void OpenTrainingSet(string path);
	static void CloseTrainingSet();
	static void GetNetDigit(int& number, vector<double>& output);
	NNUtils() = delete;
};