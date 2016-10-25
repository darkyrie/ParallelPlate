#pragma once
#define EIGEN_USE_MKL_ALL
#include "Constants.h"
#include "Mask.h"
#include "Plate.h"
#include "Eigen\Eigen"
#include <iostream>
#include <fstream>
#include <chrono>

using namespace Eigen;
using namespace constants;
using namespace std;
using namespace std::chrono;

MatrixXd createMatrix(Mask& aMask, Plate& plateBot, Plate& plateTop);