#pragma once
#include <winrt/base.h>

//#include "pch.h"

#include <stdarg.h>
#include <cmath>
#include <iostream>
#include <format>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <random>

#include "Windows.h"

using namespace std;
using namespace std::chrono;

//#define ts(x) to_string(x)
#define	ts(x) format("{}", x)

//#include "pngwriter.h"
//#include "Layer.h"
//#include "Network.h"
#include "NetUtils.h"