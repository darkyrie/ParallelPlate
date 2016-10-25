#include "ParallelPlate.h"

int main()
{
	
	/////////////////////////////////////////
	/////        Create structure
	/////////////////////////////////////////

	double distance(0.01);
	cout << "Enter distance: " << endl;
	cin >> distance;

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	Plate plateTop(distance, PLATE_TOP_POTENTIAL);
	Plate plateBot(0, PLATTE_BOT_POTENTIAL);
	Mask aMask(MASK_CHARACTER);

	plateTop.applyMask(aMask);

	/////////////////////////////////////////
	/////    Calculate coeff matrices
	/////////////////////////////////////////
	MatrixXd matrixCouplingCoeff = createMatrix(aMask, plateBot, plateTop);

	/////////////////////////////////////////
	/////      Calculate gM vector
	/////////////////////////////////////////
	VectorXd vectorGM(CELL_NUMBER_HEIGHT*CELL_NUMBER_WIDTH + aMask.cellNumberMasked);
	vectorGM << VectorXd::Constant(CELL_NUMBER_HEIGHT*CELL_NUMBER_WIDTH, PLATTE_BOT_POTENTIAL),
		VectorXd::Constant(aMask.cellNumberMasked, PLATE_TOP_POTENTIAL);

	/////////////////////////////////////////
	/////     Calculate vector Alpha
	/////////////////////////////////////////
	VectorXd vectorAlpha = matrixCouplingCoeff.llt().solve(vectorGM);
	double totalCharge = vectorAlpha.cwiseAbs().sum()*plateBot.cellSize*plateBot.cellSize;

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(t2 - t1).count();

	/////////////////////////////////////////
	/////         Write to file
	/////////////////////////////////////////
	std::ofstream file("vectoralpha.txt");
	if (file.is_open())
	{
		file.clear();
		file << vectorAlpha << '\n';
		file << "Consists of " << vectorAlpha.rows() << " rows and " << vectorAlpha.cols() << " columns" << endl;
		file << "Total charge is " << totalCharge << endl;
		file << "Calculation duration is " << duration/1000000.0 << "s" << endl;
		file << "List of constants: " << endl;
		file << "const double PLATE_HEIGHT(0.16);" << endl
			<< "const double PLATE_WIDTH(0.1);" << endl
			<< "const int CELL_NUMBER_HEIGHT(80);" << endl
			<< "const int CELL_NUMBER_WIDTH(50);" << endl
			<< "const int MASK_OUTER_COL_WIDTH(15);" << endl
			<< "const int MASK_CENTER_COL_WIDTH(20);" << endl
			<< "const int MASK_ROW_HEIGHT(20);" << endl
			<< "const char MASK_CHARACTER('L');" << endl
			<< "const double PLATE_TOP_POTENTIAL(1.0);" << endl
			<< "const double PLATTE_BOT_POTENTIAL(-1.0);" << endl
			<< "const double PI(3.14159265358979323846);" << endl
			<< "const double EPS(8.854187817e-12);";
		file.close();
	}

	return 0;
}

MatrixXd createMatrix(Mask& aMask, Plate& plateBot, Plate& plateTop)
{

	int length = CELL_NUMBER_HEIGHT*CELL_NUMBER_WIDTH + aMask.cellNumberMasked;
	RowVectorXd vectorBothWidth(length);
	RowVectorXd vectorBothHeight(length);
	RowVectorXd vectorBothDistance(length);
	vectorBothWidth << plateBot.vectorCenterOfMassWidth, plateTop.droppedVectorCenterOfMassWidth;
	vectorBothHeight << plateBot.vectorCenterOfMassHeight, plateTop.droppedVectorCenterOfMassHeight;
	vectorBothDistance << plateBot.vectorCenterOfMassDistance, plateTop.droppedVectorCenterOfMassDistance;

	MatrixXd matrixDistance = (((vectorBothWidth.replicate(length, 1) - vectorBothWidth.transpose().replicate(1, length)).array().square())
		+ ((vectorBothHeight.replicate(length, 1) - vectorBothHeight.transpose().replicate(1, length)).array().square())
		+ ((vectorBothDistance.replicate(length, 1) - vectorBothDistance.transpose().replicate(1, length)).array().square())).array().sqrt();

	MatrixXd matrixCouplingCoeff = (plateBot.cellSize*plateBot.cellSize) / (4 * PI*EPS*matrixDistance.array());
	matrixCouplingCoeff.diagonal() = ArrayXd::Constant(matrixCouplingCoeff.diagonalSize(), 1, plateBot.cellSize*0.8816736 / (PI*EPS));

	return matrixCouplingCoeff;
}