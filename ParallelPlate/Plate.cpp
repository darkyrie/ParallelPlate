#include "Plate.h"

Plate::Plate(double distance, double potential)
{
	plateDistance = distance;
	platePotential = potential;
	cellNumberTotal = CELL_NUMBER_HEIGHT*CELL_NUMBER_WIDTH;
	cellSize = PLATE_WIDTH / CELL_NUMBER_WIDTH;

	MatrixXd centerOfMassWidth =  cellSize * RowVectorXd::LinSpaced(CELL_NUMBER_WIDTH, -((double)CELL_NUMBER_WIDTH - 1) / 2, ((double)CELL_NUMBER_WIDTH - 1) / 2).replicate(CELL_NUMBER_HEIGHT, 1);
	vectorCenterOfMassWidth = Map<RowVectorXd>(centerOfMassWidth.data(), 1, cellNumberTotal);

	MatrixXd centerOfMassHeight = cellSize * VectorXd::LinSpaced(CELL_NUMBER_HEIGHT, ((double)CELL_NUMBER_HEIGHT - 1) / 2, -((double)CELL_NUMBER_HEIGHT - 1) / 2).replicate(1, CELL_NUMBER_WIDTH);;
	vectorCenterOfMassHeight = Map<RowVectorXd>(centerOfMassHeight.data(), 1, cellNumberTotal);

	vectorCenterOfMassDistance.resize(cellNumberTotal);
	vectorCenterOfMassDistance.setConstant(plateDistance);
}

void Plate::applyMask(Mask amask)
{
	cellNumberMasked = amask.cellNumberMasked;

	RowVectorXd maskedVectorCenterOfMassWidth = vectorCenterOfMassWidth.array() * amask.vectorBigMask.array();
	RowVectorXd maskedVectorCenterOfMassHeight = vectorCenterOfMassHeight.array() * amask.vectorBigMask.array();
	RowVectorXd maskedVectorCenterOfMassDistance = vectorCenterOfMassDistance.array() * amask.vectorBigMask.array();

	droppedVectorCenterOfMassWidth.resize(1, amask.cellNumberMasked);
	droppedVectorCenterOfMassHeight.resize(1, amask.cellNumberMasked);
	droppedVectorCenterOfMassDistance.resize(1, amask.cellNumberMasked);

	int position = 0;
	for (int i = 0; i < amask.vectorBigMask.cols(); i++)
	{
		if (amask.vectorBigMask(i) != INFINITY)
		{
			droppedVectorCenterOfMassWidth(position) = maskedVectorCenterOfMassWidth(i);
			droppedVectorCenterOfMassHeight(position) = maskedVectorCenterOfMassHeight(i);
			droppedVectorCenterOfMassDistance(position) = maskedVectorCenterOfMassDistance(i);
			position++;
		}
	}

}
;