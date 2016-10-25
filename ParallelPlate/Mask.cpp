#include "Mask.h"

Mask::Mask(char character)
{
	cellNumberTotal = CELL_NUMBER_WIDTH*CELL_NUMBER_HEIGHT;

	MatrixXd smallMask(4, 3);
	switch (character)
	{
	case 'L':
	{
		smallMask << 1, INFINITY, INFINITY,
			1, INFINITY, INFINITY,
			1, INFINITY, INFINITY,
			1, 1, 1;
		break;
	}
	case 'I':
	{
		smallMask << 1, 1, 1,
			INFINITY, 1, INFINITY,
			INFINITY, 1, INFINITY,
			1, 1, 1;
		break;
	}
	case 'O':
	{
		smallMask << 1, 1, 1,
			1, INFINITY, 1,
			1, INFINITY, 1,
			1, 1, 1;
		break;
	}
		
	default:
	{
		smallMask << 1, INFINITY, INFINITY,
			1, INFINITY, INFINITY,
			1, INFINITY, INFINITY,
			1, 1, 1;
	}
	}

	MatrixXd smc(1, 3);
	smc << MASK_OUTER_COL_WIDTH, MASK_CENTER_COL_WIDTH, MASK_OUTER_COL_WIDTH;
	smallMaskCol = smc.replicate(4, 1);

	smallMaskRow.resize(4, 3);
	smallMaskRow.setConstant(MASK_ROW_HEIGHT);

	MatrixXd smsc(1,3);
	smsc << 0, MASK_OUTER_COL_WIDTH, MASK_OUTER_COL_WIDTH+MASK_CENTER_COL_WIDTH;
	smallMaskStartCol = smsc.replicate(4, 1);

	MatrixXd smsr(4,1);
	smsr << 0,
		MASK_CENTER_COL_WIDTH,
		MASK_CENTER_COL_WIDTH*2,
		MASK_CENTER_COL_WIDTH*3;
	smallMaskStartRow = smsr.replicate(1, 3);

	bigMask.resize(CELL_NUMBER_HEIGHT, CELL_NUMBER_WIDTH);
	bigMask.setConstant(INFINITY);
	
	for (int i = 0; i < smallMask.rows(); i++)
	{
		for (int j = 0; j < smallMask.cols(); j++)
		{
			if (smallMask(i, j) == 1)
			{
				bigMask.block(smallMaskStartRow(i, j), smallMaskStartCol(i, j), smallMaskRow(i, j), smallMaskCol(i, j)) = 
					MatrixXd::Ones(smallMaskRow(i, j), smallMaskCol(i, j));
			}
		}
	}

	vectorBigMask = Map<RowVectorXd>(bigMask.data(), 1, cellNumberTotal);

	int count(0);
	for (int i = 0; i < vectorBigMask.cols(); i++)
	{
		if (vectorBigMask(i) != INFINITY)
			count++;
	}

	cellNumberMasked = count;
}