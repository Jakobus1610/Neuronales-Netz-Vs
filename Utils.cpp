#include "Utils.h"

int NNUtils::LogStatus = Info | Debug | Warning | Error;
ifstream NNUtils::trainingSet = ifstream();
high_resolution_clock::time_point NNUtils::startT = high_resolution_clock::now();
high_resolution_clock::time_point NNUtils::stopT = high_resolution_clock::now();

void NNUtils::Log(Status stat, string str)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (stat & LogStatus)
	{
		switch (stat)
		{	
		case Info:
			SetConsoleTextAttribute(hConsole, 0x0F);
			break;
		case Debug:
			SetConsoleTextAttribute(hConsole, 0x02);
			break;
		case Warning:
			SetConsoleTextAttribute(hConsole, 0x6F);
			break;
		case Error:
			SetConsoleTextAttribute(hConsole, 0x4F);
			break;
		default:
			SetConsoleTextAttribute(hConsole, 0x0F);
			break;
		}
		if(stat & Debug)
			cout << str << endl;
		else
			cout << str << endl;

	}
}

void NNUtils::StartWatch()
{
	startT = high_resolution_clock::now();
	Log(Info, "Start Watch");
}

void NNUtils::StopWatch()
{
	stopT = high_resolution_clock::now();
	duration<double, milli> timeDif = duration_cast<duration<double, milli>>(stopT - startT);
	Log(Info, "Stop Watch");
	Log(Info, "Duration: " + to_string(timeDif.count()) + " milliseconds");
}

double NNUtils::ActivationFunc(double value)
{
	return 1 / (1 + exp(-value));
}

double NNUtils::ActivationFuncDervivative(double value)
{
	double activation = ActivationFunc(value);
	return activation * (1 - activation);
}

double NNUtils::CostFunc(double input, double value)
{
	double res = input - value;
	return (res * res);
}

double NNUtils::CostFuncDerivative(double input, double value)
{
	double res = input - value;
	return 2 * res;
}

double NNUtils::RandomNumer(double min, double max)
{
	random_device rd;
	mt19937 rng(rd());
	uniform_real_distribution<double> dist(min, max);
	return dist(rng);
}

void NNUtils::OpenTrainingSet(string path)
{
	trainingSet.open(path);
}

void NNUtils::CloseTrainingSet()
{
	trainingSet.close();
}

void NNUtils::GetNetDigit(int& number, vector<double>& output)
{
	if (!trainingSet.is_open())
	{
		cout << "Abort. Trainingset not opend" << endl;
		return;
	}
	output.resize(784);

	string line, argument;

	getline(trainingSet, line);
	stringstream ss(line);
	bool is_first_num = true;
	int index = 0;

	number = -1;

	while (getline(ss, argument, ','))
	{
		if (is_first_num)
		{
			number = stoi(argument);
			is_first_num = false;
		}
		else
		{
			int arg = stoi(argument);
			double result = (double)arg / 255.0;
			output[index] = result;
			index++;
		}
	}
}

