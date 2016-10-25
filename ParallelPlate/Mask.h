#pragma once

#include "Constants.h"
#include "Eigen\Eigen"
#include <iostream>

using namespace Eigen;
using namespace constants;

class Mask
{
public:
	int cellNumberTotal;
	int cellNumberMasked;
	char maskCharacter;
	MatrixXd smallMask;
	MatrixXd smallMaskRow;
	MatrixXd smallMaskCol;
	MatrixXd smallMaskStartRow;
	MatrixXd smallMaskStartCol;
	MatrixXd bigMask;
	RowVectorXd	vectorBigMask;

	Mask(char character);

};