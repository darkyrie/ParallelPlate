#pragma once

#include "Eigen\Eigen"
#include <iostream>
#include "Mask.h"

using namespace Eigen;

class Plate
{
public:
	double plateDistance;
	double platePotential;
	int cellNumberTotal;
	int cellNumberMasked;
	double cellSize;
	RowVectorXd vectorCenterOfMassHeight;
	RowVectorXd vectorCenterOfMassWidth;
	RowVectorXd vectorCenterOfMassDistance;
	RowVectorXd droppedVectorCenterOfMassWidth;
	RowVectorXd droppedVectorCenterOfMassHeight;
	RowVectorXd droppedVectorCenterOfMassDistance;

	Plate(double distance, double potential);

	void applyMask(Mask amask);
};