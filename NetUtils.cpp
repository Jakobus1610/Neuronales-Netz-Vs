#include "NetUtils.h"
#include "pch.h"

int NetUtils::logStatus = Info | Debug | Warning | Error;
int NetUtils::debugCount = 0;
int NetUtils::warningCount = 0;
int NetUtils::errorCount = 0;
int NetUtils::logCount = 0;
ifstream NetUtils::trainingSet = ifstream();
high_resolution_clock::time_point NetUtils::startT = high_resolution_clock::now();
high_resolution_clock::time_point NetUtils::stopT = high_resolution_clock::now();

void NetUtils::Log(Status stat, string str)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (stat & logStatus)
	{
		switch (stat)
		{
		case Info:
			SetConsoleTextAttribute(hConsole, 0x0F);
			logCount++;
			break;
		case Debug:
			SetConsoleTextAttribute(hConsole, 0x02);
			debugCount++;
			break;
		case Warning:
			SetConsoleTextAttribute(hConsole, 0x6F);
			warningCount++;
			break;
		case Error:
			SetConsoleTextAttribute(hConsole, 0x4F);
			errorCount++;
			break;
		default:
			SetConsoleTextAttribute(hConsole, 0x0F);
			break;
		}
		if (stat & Debug)
			cout << str << endl;
		else
			cout << str << endl;
	}
}

void NetUtils::PrintLogCounts()
{
	Log(Info, "\nAll log counts: \t" + ts(logCount + debugCount + warningCount + errorCount));
	Log(Info, "Info count: \t\t" + ts(logCount));
	Log(Info, "Debug count: \t\t" + ts(debugCount));
	Log(Info, "Warning count: \t\t" + ts(warningCount));
	Log(Info, "Error count: \t\t" + ts(errorCount));
}

void NetUtils::StartWatch()
{
	startT = high_resolution_clock::now();
	Log(Warning, "Start Watch");
}

void NetUtils::StopWatch()
{
	stopT = high_resolution_clock::now();
	duration<double, milli> timeDif = duration_cast<duration<double, milli>>(stopT - startT);
	Log(Warning, "Stop Watch");
	Log(Warning, "Duration: " + to_string(timeDif.count()) + " milliseconds");
}

double NetUtils::ActivationFunc(double value)
{
	return 1 / (1 + exp(-value));
}

double NetUtils::ActivationFuncDervivative(double value)
{
	double activation = ActivationFunc(value);
	return activation * (1 - activation);
}

double NetUtils::CostFunc(double input, double value)
{
	double res = input - value;
	return (res * res);
}

double NetUtils::CostFuncDerivative(double input, double value)
{
	double res = input - value;
	return 2 * res;
}

double NetUtils::RandomNumer(double min, double max)
{
	random_device rd;
	mt19937 rng(rd());
	uniform_real_distribution<double> dist(min, max);
	return dist(rng);
}

void NetUtils::OpenTrainingSet(string path)
{
	trainingSet.open(path);
}

void NetUtils::CloseTrainingSet()
{
	trainingSet.close();
}

DataPoint NetUtils::GetDataPoint()
{
	DataPoint dataPoint;
	dataPoint.values.resize(784);

	if (!trainingSet.is_open())
	{
		cout << "Abort. Trainingset not opend" << endl;
		NetUtils::Log(Error, "Trainingset isnt open!");
		return dataPoint;
	}

	string line, argument;

	getline(trainingSet, line);
	stringstream ss(line);
	bool is_first_num = true;
	int index = 0;

	dataPoint.number = -1;

	while (getline(ss, argument, ','))
	{
		if (is_first_num)
		{
			dataPoint.number = stoi(argument);
			is_first_num = false;
		}
		else
		{
			double result = (double)stoi(argument) / 255.0;
			dataPoint.values[index] = result;
			index++;
		}

	}
	dataPoint.expectedValues.resize(10);

	switch (dataPoint.number)
	{
	case 0:
		dataPoint.expectedValues = { 1,0,0,0,0,0,0,0,0,0 };
		break;
	case 1:
		dataPoint.expectedValues = { 0,1,0,0,0,0,0,0,0,0 };
		break;
	case 2:
		dataPoint.expectedValues = { 0,0,1,0,0,0,0,0,0,0 };
		break;
	case 3:
		dataPoint.expectedValues = { 0,0,0,1,0,0,0,0,0,0 };
		break;
	case 4:
		dataPoint.expectedValues = { 0,0,0,0,1,0,0,0,0,0 };
		break;
	case 5:
		dataPoint.expectedValues = { 0,0,0,0,0,1,0,0,0,0 };
		break;
	case 6:
		dataPoint.expectedValues = { 0,0,0,0,0,0,1,0,0,0 };
		break;
	case 7:
		dataPoint.expectedValues = { 0,0,0,0,0,0,0,1,0,0 };
		break;
	case 8:
		dataPoint.expectedValues = { 0,0,0,0,0,0,0,0,1,0 };
		break;
	case 9:
		dataPoint.expectedValues = { 0,0,0,0,0,0,0,0,0,1 };
		break;
	default:
		dataPoint.expectedValues = { 0,0,0,0,0,0,0,0,0,0 };
		NetUtils::Log(Error, "Error. getNumberFromSet switch number");
		break;
	}
	return dataPoint;
}

void NetUtils::PlotNumber(DataPoint dataPoint)
{
	string name = "PlotedNumberIs-" + ts(dataPoint.number) + ".png";
	pngwriter png(28, 28, 0, name.c_str());
	cout << dataPoint.number << endl;
	int grayScale = 0;
	for (int y = 27; y >= 0; y--)
	{
		for (int x = 0; x < 28; x++)
		{
			png.plot(x, y, dataPoint.values[grayScale], dataPoint.values[grayScale], dataPoint.values[grayScale]);
			grayScale++;
		}
	}
	png.close();
	cout << endl;
}


