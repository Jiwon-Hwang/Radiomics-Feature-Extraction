#include "GLRLM.h"

using namespace std;

GLRLM::GLRLM() {

}
GLRLM::~GLRLM() {

}
void GLRLM::clearVariable() {

	vector1DofOriPixelsInROI.clear();
	vector<short>().swap(vector1DofOriPixelsInROI); // size & capacity ��� 0���� �ʱ�ȭ
	diagonalProbabilities.clear();
	vector<float>().swap(diagonalProbabilities);
	crossProbabilities.clear();
	vector<float>().swap(crossProbabilities);
	sumProbCols.clear();
	vector<float>().swap(sumProbCols);
	sumProbRows.clear();
	vector<float>().swap(sumProbRows);

	jointMaximum = NAN;
	jointAverage = NAN;
	jointVariance = NAN;
	jointEntropy = NAN;
	diffAverage = NAN;
	diffVariance = NAN;
	diffEntropy = NAN;
	sumAverage = NAN;
	sumVariance = NAN;
	sumEntropy = NAN;
	angSecMoment = NAN;
	contrast = NAN;
	dissimilarity = NAN;
	inverseDiff = NAN;
	inverseDiffNorm = NAN;
	inverseDiffMom = NAN;
	inverseDiffMomNorm = NAN;
	inverseVar = NAN;
	meanRowProb = NAN;
	meanColProb = NAN;
	stdRowProb = NAN;
	stdColProb = NAN;
	correlation = NAN;
	autoCorrelation = NAN;
	clusterTendency = NAN;
	clusterShade = NAN;
	clusterProminence = NAN;
	HXY = NAN;			
	HX = NAN;				
	HXY1 = NAN;			
	HXY2 = NAN;
	firstMCorrelation = NAN;
	secondMCorrelation = NAN;

}
void GLRLM::clearVector() {

	if (!final2DVec.empty()) {
		final2DVec.clear();
		vector<vector<float>>().swap(final2DVec); // size & capacity ��� 0���� �ʱ�ȭ
	}

	if (!final1DVec.empty()) {
		final1DVec.clear();
		vector<float>().swap(final1DVec);
	}

}

vector<short> GLRLM::get1DVectorOfPixels(short* psImage, unsigned char* pucMask) {
	
	vector<short> vector1DofOriPixels(nHeight*nWidth, 0); // 0���� �ʱ�ȭ ��, ROI �ٱ� ������ �״�� 0. �̸� ũ�� �Ҵ��� �δ°� �ξ� ������
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			short imageValue = psImage[index];

			// ROI ��(0)�� �� �ȼ����� ��� �Ҵ� (in 1d vector => for fast discretising!)
			vector1DofOriPixels[index] = (maskValue > (unsigned char)0) ? imageValue : 0;
			// ROI ���ΰ� push (ROI�� ������ ��� pass => for min, max)
			if (maskValue >(unsigned char)0) {
				vector1DofOriPixelsInROI.push_back(imageValue); // �����̽����� �ʱ�ȭ ����� ��! (clearVariable)
			}
		}
	}

	return vector1DofOriPixels;
}
vector<vector<unsigned short>> GLRLM::get2DVectorOfDiscretizedPixels_nBins(short* psImage, unsigned char* pucMask) {

	// min(front) : -1, max(back) : 180
	float min = (float)*min_element(vector1DofOriPixelsInROI.begin(), vector1DofOriPixelsInROI.end()); // min_element() : pointer return
	float max = (float)*max_element(vector1DofOriPixelsInROI.begin(), vector1DofOriPixelsInROI.end());
	vector<float> tempFloatVec(vector1DofOriPixels.begin(), vector1DofOriPixels.end()); // short -> float ����ȯ

	if (min == 0 && min == max) {
		cout << "error in calculating discretization, VOI contains only 0" << endl;
		exit(0);
	}
	else if (min > 0 && min == max) {
		cout << "error in calculating discretization, VOI contains only one intensity value, minimum value is set to 0" << endl;
		min = 0;
	}


	// �켱 ROI ���� �κб��� ��� ����ȭ ��, �� �������� if������ ROI �� ������ 0 ó��

	//subtract minimum value from every matrix element
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(minus<float>(), min)); // minus<T> : ���� ���� �Լ� ��ü (-) / bind2nd : 2��° �μ��� ������ �Լ� ��ü�� ��ȯ / transform : �ϰ� ����

																											 //get the range
	float range = (max - min) / nBins; // range : �� (width of a bin) => ***float�� ���� / �� ���� �ƴ϶� ��¥ "������" ����!!!***

	//divide every element of the matrix by the range
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(divides<float>(), range));

	//replace 0 to 1 => ibsi ��ũ�� ��ŷ �κ�! (min - min = 0�� �ǹǷ� �ּ��� 1�� �ٲ���� �ڿ� ceil���� min value of each bin : 1 ����)
	replace(tempFloatVec.begin(), tempFloatVec.end(), 0, 1);

	//do rounding(ceil), type casting(unsigned short) and asigning to 2D Vec (with ROI)
	vector<vector<unsigned short>> vector2DofDiscretizedPixels(nHeight, vector<unsigned short>(nWidth, 0)); // ����ȭ�� ���� ��� �ȼ����� ���� ����
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			unsigned short imageValue_discretized = (unsigned short)ceil(tempFloatVec[index]); // psImage (ori) -> tempFloatVec (discretized)

			// ROI ��(0)�� ��(discretized) �ȼ����� 2D Vec�� �Ҵ�
			vector2DofDiscretizedPixels[row][col] = (maskValue >(unsigned char)0) ? imageValue_discretized : 0;
		}
	}

	return vector2DofDiscretizedPixels;
}
void GLRLM::getXYDirections(int &directionX, int &directionY, int angle) {

	if (angle == 0) {
		directionX = 0;
		directionY = 0;
	}
	if (angle == 180) {
		directionX = 1;
		directionY = 0;
	}
	else if (angle == 90) {
		directionX = 0;
		directionY = 1;
	}
	else if (angle == 45) {
		directionX = 1;
		directionY = 1;
	}
	else if (angle == 135) {
		directionX = -1;
		directionY = 1;
	}

}
int GLRLM::findIndex(int size, unsigned short target) {
	int i = 0;
	while ((i < size) && (i+1 != target)) i++; // i+1 == diffGreyLevels[i]
	return (i < size) ? (i) : (-1);
}
void GLRLM::fill2DGLRLMatrix(vector<vector<unsigned short>> vector2DofDiscretizedPixels, vector<vector<float>> &GLRLMatrix, int angle) {

	// vector2DofDiscretizedPixels : ������� copy (�Ű������� �� ���� �Ҵ�)

	unsigned short actElement = 0;
	int runLength = 0;
	int directionX;
	int directionY;

	//define in which direction you have to look for a neighbor
	getXYDirections(directionX, directionY, angle); // ���� �� 1ȸ����

	//get the grey level we are interested at the moment
	int actGreyIndex;

	for (int row = 0; row<nHeight; row++) {
		for (int col = 0; col<nWidth; col++) {
			//at the beginning the run length =0
			runLength = 0;
			//get the actual matrix element
			actElement = vector2DofDiscretizedPixels[nHeight - row - 1][col];
			actGreyIndex = findIndex(nBins, actElement); // in ROI : 0~(nBins-1), not in ROI : -1 return
			//if the actual matrix element is not 0 (=is in ROI)
			if (!actElement) {
				//set the run length to 1
				runLength = 1;
				//to avoid to take an element more than once, set the element to 0 => �ٽ� üũ ���ϵ��� ROI �� �ȼ�ó�� 0���� �ٲٱ�
				vector2DofDiscretizedPixels[nHeight - row - 1][col] = 0;
				//now look at the matrix element in the actual direction (depends on the
				//angle we are interested at the moment)
				int colValue = col + directionX;
				int rowValue = nHeight - 1 - (row + directionY);
				//now have a look at the following elements in the desired direction
				//stop as soon as we look at an element diifferent from our actual element
				while (colValue<nWidth && rowValue>-1 && colValue>-1 && vector2DofDiscretizedPixels[rowValue][colValue] == actElement) {
					//for every element we find, count the runLength
					runLength += 1;
					vector2DofDiscretizedPixels[rowValue][colValue] = 0;
					//go further in the desired direction
					colValue += 1 * directionX;
					rowValue -= 1 * directionY;
				}
			}
			//as soon as we cannot find an element in the desired direction, count one up in the desired
			//position of the glrl-matrix
			// if not in ROI, runLengh = 0 (actGreyIndex = -1)
			if (runLength > 0 && runLength < maxRunLength + 1) {

				GLRLMatrix[actGreyIndex][runLength - 1] += 1;
			}

		}
	}
	

}
void GLRLM::calcDiagonalProbabilities(vector<vector<float>> GLRLMatrix) {

	float diagProbability;

	for (int k = 0; k < sizeMatrix; k++) {
		diagProbability = 0;
		//get the diagonal elements
		for (int row = 0; row < sizeMatrix; row++) {
			for (int col = 0; col < sizeMatrix; col++) {
				//Kronecker delta
				if (k == abs(row - col)) {
					diagProbability += GLRLMatrix[row][col];
				}
			}
		}
		//store the diagonal probabilities in a vector
		diagonalProbabilities.push_back(diagProbability);
	}
}
void GLRLM::calcCrossProbabilities(vector<vector<float>> GLRLMatrix) {

	float crossProbability;

	for (int k = 0; k < 2 * sizeMatrix; k++) {
		crossProbability = 0;
		for (int row = 0; row < sizeMatrix; row++) {
			for (int col = 0; col < sizeMatrix; col++) {
				//Kronecker delta
				if (k == abs(row + col)) {
					crossProbability += GLRLMatrix[row][col];
				}
			}
		}
		crossProbabilities.push_back(crossProbability);
	}
}
void GLRLM::average4DirValues(vector<vector<float>> temp4DirVals2DVec, vector<float> &tempValues1DVec) {

	for (int col = 0; col < temp4DirVals2DVec[0].size(); col++) {
		float colSum = 0;
		float colMean;

		for (int row = 0; row < 4; row++) {
			colSum += temp4DirVals2DVec[row][col];
		}
		colMean = colSum / 4;
		tempValues1DVec.push_back(colMean);
	}
}

// common calculation functions
float GLRLM::getTotalSum(vector<vector<float>> matrix) {
	// calculate the sum of all matrix elements
	float sum = 0;
	for (int i = 0; i<matrix.size(); i++) {
		for (int j = 0; j<matrix[0].size(); j++) {
			sum += matrix[i][j];
		}
	}

	return sum;
}
vector<float> GLRLM::getRowSums(vector<vector<float>> matrix) {
	// calculates the sum of rows and stores them in the vector rowSums
	vector<float> rowSums;
	rowSums.clear();
	int sum = 0;
	for (int col = 0; col < matrix[0].size(); col++) {
		sum = 0;
		for (int row = 0; row < matrix.size(); row++) {
			sum += matrix[row][col];

		}
		rowSums.push_back(sum);

	}

	return rowSums;
}
vector<float> GLRLM::getColSums(vector<vector<float>> matrix) {
	// calculates the sum of columns and stores them in the vector colSums
	int sum = 0;

	vector<float> colSums;
	colSums.clear();
	for (int row = 0; row < matrix.size(); row++) {
		sum = 0;
		for (int col = 0; col < matrix[0].size(); col++) {
			sum += matrix[row][col];
		}
		colSums.push_back(sum);
	}
	return colSums;
}

void GLRLM::calcJointMaximum(vector<vector<float>> GLRLMatrix) {
	
	jointMaximum = 0;

	for (int row = 0; row < sizeMatrix; row++) {
		for (int col = 0; col < sizeMatrix; col++) {
			jointMaximum = (GLRLMatrix[row][col] > jointMaximum) ? GLRLMatrix[row][col] : jointMaximum;
		}
	}
}
void GLRLM::calcJointAverage(vector<vector<float>> GLRLMatrix) {

	jointAverage = 0;

	for (int row = 0; row < sizeMatrix; row++) {
		for (int col = 0; col < sizeMatrix; col++) {
			jointAverage += (row + 1) * GLRLMatrix[row][col];
		}
	}
}
void GLRLM::calcJointVariance(vector<vector<float>> GLRLMatrix) {

	jointVariance = 0;

	if (isnan(jointAverage)) {
		calcJointAverage(GLRLMatrix);
	}

	for (int row = 0; row < sizeMatrix; row++) {
		for (int col = 0; col < sizeMatrix; col++) {
			jointVariance += pow((row + 1 - jointAverage), 2) * GLRLMatrix[row][col];
		}
	}
}
void GLRLM::calcJointEntropy(vector<vector<float>> GLRLMatrix) {
	
	jointEntropy = 0;

	float tempElement;
	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			tempElement = GLRLMatrix[i][j];
			if (tempElement != 0) {
				jointEntropy -= tempElement * log2(tempElement);
			}
		}
	}
}
void GLRLM::calcDiffAverage(vector<vector<float>> GLRLMatrix) {

	diffAverage = 0;

	for (int diagElement = 0; diagElement < diagonalProbabilities.size(); diagElement++) {
		diffAverage += diagonalProbabilities[diagElement] * diagElement;
	}
}
void GLRLM::calcDiffVariance(vector<vector<float>> GLRLMatrix) {

	diffVariance = 0;

	if (isnan(diffAverage)) {
		calcDiffAverage(GLRLMatrix);
	}

	for (int diagElement = 0; diagElement < diagonalProbabilities.size(); diagElement++) {
		diffVariance += diagonalProbabilities[diagElement] * pow((diagElement - diffAverage), 2);
	}
}
void GLRLM::calcDiffEntropy(vector<vector<float>> GLRLMatrix) {

	diffEntropy = 0;

	float tempElement = 0;
	for (int k = 0; k < diagonalProbabilities.size(); k++) {
		tempElement = diagonalProbabilities[k];
		if (tempElement != 0) {
			diffEntropy -= tempElement*log2(tempElement);
		}
	}
}
void GLRLM::calcSumAverage(vector<vector<float>> GLRLMatrix) {

	sumAverage = 0;

	for (int crossElement = 1; crossElement < crossProbabilities.size() + 1; crossElement++) {
		sumAverage += crossProbabilities[crossElement - 1] * (crossElement + 1);
	}
}
void GLRLM::calcSumVariance(vector<vector<float>> GLRLMatrix) {

	sumVariance = 0;

	if (isnan(sumAverage)) {
		calcSumAverage(GLRLMatrix);
	}

	for (int crossElement = 1; crossElement < crossProbabilities.size() + 1; crossElement++) {
		sumVariance += crossProbabilities[crossElement - 1] * pow((crossElement + 1 - sumAverage), 2);
	}
}
void GLRLM::calcSumEntropy(vector<vector<float>> GLRLMatrix) {

	sumEntropy = 0;

	float tempElement = 0;
	for (int k = 0; k < crossProbabilities.size(); k++) {
		tempElement = crossProbabilities[k];
		if (tempElement != 0 && !isnan(tempElement)) {
			sumEntropy -= tempElement*log2(tempElement);
		}
	}
}
void GLRLM::calcAngSecMoment(vector<vector<float>> GLRLMatrix) {
	
	// Angular Second Moment (== Energy, Uniformity)
	angSecMoment = 0;

	for (int row = 0; row < sizeMatrix; row++) {
		for (int col = 0; col < sizeMatrix; col++) {
			angSecMoment += pow(GLRLMatrix[row][col], 2);
		}
	}

}
void GLRLM::calcContrast(vector<vector<float>> GLRLMatrix) {

	contrast = 0;

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			contrast += pow((i - j), 2)*GLRLMatrix[i][j];
		}
	}
}
void GLRLM::calcDissimilarity(vector<vector<float>> GLRLMatrix) {

	dissimilarity = 0;

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			dissimilarity += abs(i - j)*GLRLMatrix[i][j];
		}
	}
}
void GLRLM::calcInverseDiff(vector<vector<float>> GLRLMatrix) {

	inverseDiff = 0;

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			inverseDiff += GLRLMatrix[i][j] / (1 + abs(i - j));
		}
	}
}
void GLRLM::calcInverseDiffNorm(vector<vector<float>> GLRLMatrix) {

	inverseDiffNorm = 0;

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			inverseDiffNorm += GLRLMatrix[i][j] / (1 + float(abs(i - j)) / float(sizeMatrix));
		}
	}
}
void GLRLM::calcInverseDiffMom(vector<vector<float>> GLRLMatrix) {

	inverseDiffMom = 0;

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			inverseDiffMom += GLRLMatrix[i][j] / (1 + pow((i - j), 2));
		}
	}
}
void GLRLM::calcInverseDiffMomNorm(vector<vector<float>> GLRLMatrix) {

	inverseDiffMomNorm = 0;

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			inverseDiffMomNorm += GLRLMatrix[i][j] / (1 + pow((i - j), 2) / pow(sizeMatrix, 2));
		}
	}
}
void GLRLM::calcInverseVar(vector<vector<float>> GLRLMatrix) {

	inverseVar = 0;

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = i + 1; j < sizeMatrix; j++) {
			inverseVar += 2 * GLRLMatrix[i][j] / pow((i - j), 2);
		}
	}
}
void GLRLM::calcColProb(vector<vector<float>> GLRLMatrix) {

	float colProb;
	sumProbCols.clear();

	for (int j = 0; j < sizeMatrix; j++) {
		colProb = 0;
		for (int i = 0; i < sizeMatrix; i++) {
			colProb += GLRLMatrix[i][j];
		}
		sumProbCols.push_back(colProb);
	}
}
void GLRLM::calcRowProb(vector<vector<float>> GLRLMatrix) {

	float rowProb;
	sumProbRows.clear();

	for (int i = 0; i < sizeMatrix; i++) {
		rowProb = 0;
		for (int j = 0; j < sizeMatrix; j++) {
			rowProb += GLRLMatrix[i][j];
		}
		sumProbRows.push_back(rowProb);
	}
}
void GLRLM::calcMeanColProb(vector<vector<float>> GLRLMatrix) {

	calcColProb(GLRLMatrix);

	meanColProb = 0;
	stdColProb = 0;

	for (int k = 0; k < sizeMatrix; k++) {
		meanColProb += (k + 1) * sumProbCols[k];
	}

	for (int k = 0; k < sizeMatrix; k++) {
		stdColProb += pow(((k + 1) - meanColProb), 2)*sumProbCols[k];
	}

	stdColProb = sqrt(stdColProb);

}
void GLRLM::calcMeanRowProb(vector<vector<float>> GLRLMatrix) {

	calcRowProb(GLRLMatrix);

	meanRowProb = 0;
	stdRowProb = 0;

	for (int k = 0; k < sizeMatrix; k++) {
		meanRowProb += (k + 1) * sumProbRows[k];
	}

	for (int k = 0; k < sizeMatrix; k++) {
		stdRowProb += pow(((k + 1) - meanRowProb), 2)*sumProbRows[k];
	}

	stdRowProb = sqrt(stdRowProb);

}
void GLRLM::calcCorrelation(vector<vector<float>> GLRLMatrix) {

	calcMeanColProb(GLRLMatrix); 
	calcMeanRowProb(GLRLMatrix); 

	correlation = 0;

	for (int row = 1; row < sizeMatrix + 1; row++) {
		for (int col = 1; col < sizeMatrix + 1; col++) {
			correlation += (row - meanRowProb)*(col - meanRowProb)*GLRLMatrix[row - 1][col - 1];
		}
	}

	if (!isnan(correlation) && stdRowProb != 0 && !isnan(stdRowProb)) {
		correlation = correlation / pow(stdRowProb, 2);
	}
}
void GLRLM::calcAutoCorrelation(vector<vector<float>> GLRLMatrix) {

	autoCorrelation = 0;

	for (int row = 1; row < sizeMatrix + 1; row++) {
		for (int col = 1; col < sizeMatrix + 1; col++) {
			autoCorrelation += GLRLMatrix[row - 1][col - 1] * (row)*(col);
		}
	}
}
void GLRLM::calcClusterTendency(vector<vector<float>> GLRLMatrix) {

	clusterTendency = 0;

	if (isnan(meanRowProb)) {
		calcMeanRowProb(GLRLMatrix);
	}

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			clusterTendency += pow((i + j + 2 - 2 * meanRowProb), 2)*GLRLMatrix[i][j];
		}
	}

}
void GLRLM::calcClusterShade(vector<vector<float>> GLRLMatrix) {

	clusterShade = 0;

	if (isnan(meanRowProb)) {
		calcMeanRowProb(GLRLMatrix);
	}

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			clusterShade += pow((i + j + 2 - 2 * meanRowProb), 3)*GLRLMatrix[i][j];
		}
	}

}
void GLRLM::calcClusterProminence(vector<vector<float>> GLRLMatrix) {

	clusterProminence = 0;

	if (isnan(meanRowProb)) {
		calcMeanRowProb(GLRLMatrix);
	}

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			clusterProminence += pow((i + j + 2 - 2 * meanRowProb), 4)*GLRLMatrix[i][j];
		}
	}
}
void GLRLM::calcFirstMCorrelation(vector<vector<float>> GLRLMatrix) {

	calcColProb(GLRLMatrix); // for. sumProbCols
	calcRowProb(GLRLMatrix); // for. sumProbRows
	
	HXY = 0;
	HX = 0;
	HXY1 = 0;
	float actualProbRows = 0;
	float actualProbCols = 0;

	for (int i = 0; i < sizeMatrix; i++) {
		actualProbRows = sumProbRows[i];
		if (actualProbRows != 0) {
			HX -= actualProbRows*log2(actualProbRows);

			for (int j = 0; j < sizeMatrix; j++) {
				actualProbCols = sumProbCols[j];
				if (actualProbCols != 0 && GLRLMatrix[i][j] != 0 && !isnan(GLRLMatrix[i][j]) && actualProbRows != 0) {
					HXY -= GLRLMatrix[i][j] * log2(GLRLMatrix[i][j]);
					HXY1 -= GLRLMatrix[i][j] * log2(actualProbRows*actualProbCols);
				}
			}
		}
	}
	if (HX > 0) {
		firstMCorrelation = (HXY - HXY1) / HX;
	}
	else {
		firstMCorrelation = 0;
	}
}
void GLRLM::calcSecondMCorrelation(vector<vector<float>> GLRLMatrix) {

	if (isnan(HXY)) {
		calcFirstMCorrelation(GLRLMatrix);
	}

	HXY2 = 0;

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			if (sumProbRows[i] * sumProbCols[j] != 0) {
				HXY2 -= sumProbRows[i] * sumProbCols[j] * log2(sumProbRows[i] * sumProbCols[j]);
			}
		}
	}
	secondMCorrelation = pow((1 - exp(-2 * (HXY2 - HXY))), 0.5);
}

void GLRLM::calcFeature(int FEATURE_IDX, vector<float> &temp1DirVals1DVec, vector<vector<float>> sumGLRLM) {

	switch (FEATURE_IDX)
	{
	case SRE:
		calcJointMaximum(sumGLRLM);
		temp1DirVals1DVec.push_back(jointMaximum);
		break;

	case LRE:
		calcJointAverage(sumGLRLM);
		temp1DirVals1DVec.push_back(jointAverage);
		break;

	case LGE:
		calcJointVariance(sumGLRLM);
		temp1DirVals1DVec.push_back(jointVariance);
		break;

	case HGE:
		calcJointEntropy(sumGLRLM);
		temp1DirVals1DVec.push_back(jointEntropy);
		break; 

	case SRLE:
		calcDiffAverage(sumGLRLM);
		temp1DirVals1DVec.push_back(diffAverage);
		break;

	case SRHE:
		calcDiffVariance(sumGLRLM);
		temp1DirVals1DVec.push_back(diffVariance);
		break;

	case LRLE:
		calcDiffEntropy(sumGLRLM);
		temp1DirVals1DVec.push_back(diffEntropy);
		break;

	case LRHE:
		calcSumAverage(sumGLRLM);
		temp1DirVals1DVec.push_back(sumAverage);
		break;

	case GNU:
		calcSumVariance(sumGLRLM);
		temp1DirVals1DVec.push_back(sumVariance);
		break;

	case GNUN:
		calcSumEntropy(sumGLRLM);
		temp1DirVals1DVec.push_back(sumEntropy);
		break;

	case RLNU:
		calcAngSecMoment(sumGLRLM);
		temp1DirVals1DVec.push_back(angSecMoment);
		break;

	case RLNUN:
		calcContrast(sumGLRLM);
		temp1DirVals1DVec.push_back(contrast);
		break;

	case RP:
		calcDissimilarity(sumGLRLM);
		temp1DirVals1DVec.push_back(dissimilarity);
		break;

	case GLV:
		calcInverseDiff(sumGLRLM);
		temp1DirVals1DVec.push_back(inverseDiff);
		break;

	case RLV:
		calcInverseDiffNorm(sumGLRLM);
		temp1DirVals1DVec.push_back(inverseDiffNorm);
		break;

	case RE:
		calcInverseDiffMom(sumGLRLM);
		temp1DirVals1DVec.push_back(inverseDiffMom);
		break;

	default:
		break;
	}
}
void GLRLM::featureExtraction(short* psImage, unsigned char* pucMask, int nHeight_, int nWidth_) {

	// claer all values
	clearVariable(); // �����̽����� �ʱ�ȭ

	// get discretized 2D input matrix (2d vector)
	nHeight = nHeight_;
	nWidth = nWidth_;
	vector1DofOriPixels = get1DVectorOfPixels(psImage, pucMask); 
	vector2DofDiscretizedPixels = get2DVectorOfDiscretizedPixels_nBins(psImage, pucMask); // �����̽����� �ʱ�ȭ
	
	vector<vector<float>> temp4DirVals2DVec;	// �����̽����� �ʱ�ȭ (for. 4���� 1d vec ��ճ���)
	vector<float> tempValues1DVec;				// �����̽����� �ʱ�ȭ (for. final2DVec�� ����)

	// calculate checked feature (4 directions)
	sizeMatrix = nBins;
	maxRunLength = max(nHeight, nWidth);
	int ang;
	float totalSum;
	vector<float> rowSums;
	vector<float> colSums;

	for (int dir = 0; dir < 4; dir++) {

		clearVariable();					// ���⸶�� �ʱ�ȭ
		vector<float> temp1DirVals1DVec;	// ���⸶�� �ʱ�ȭ

		// calculate GLRLM matrix for each dir
		ang = 180 - dir * 45;
		vector<vector<float>> GLRLMatrix(sizeMatrix, vector<float>(maxRunLength, 0));
		fill2DGLRLMatrix(vector2DofDiscretizedPixels, GLRLMatrix, ang); 

		totalSum = getTotalSum(GLRLMatrix);
		rowSums = getRowSums(GLRLMatrix);
		colSums = getColSums(GLRLMatrix);
		
		//calcDiagonalProbabilities(sum);
		//calcCrossProbabilities(sum);

		// calculate feature for each dir
		for (int i = 0; i < FEATURE_COUNT; i++) {
			if (isCheckedFeature[i]) calcFeature(i, temp1DirVals1DVec, GLRLMatrix);
		}
		temp4DirVals2DVec.push_back(temp1DirVals1DVec); // 4��
	}

	average4DirValues(temp4DirVals2DVec, tempValues1DVec); // 4���� ��� Ư¡��

	final2DVec.push_back(tempValues1DVec); // ��� ROI �����̽� ���� ������ ���� (�ø���� �ʱ�ȭ)
	
}

void GLRLM::averageAllValues() {

	// get final mean vector
	for (int col = 0; col < final2DVec[0].size(); col++) {
		float colSum = 0;
		float colMean;

		for (int row = 0; row < final2DVec.size(); row++) {
			colSum += final2DVec[row][col];
		}
		colMean = colSum / final2DVec.size();
		final1DVec.push_back(colMean);
	}

}

void GLRLM::defineFeatureNames(vector<string> &features) {
	
	features[SRE] = "short run emphasis";
	features[LRE] = "long run emphasis";
	features[LGE] = "Low grey level run emphasis";
	features[HGE] = "High grey level run emphasis";
	features[SRLE] = "Short run low grey level emphasis";
	features[SRHE] = "Short run high grey level emphasis";
	features[LRLE] = "Long run low grey level emphasis";
	features[LRHE] = "Long run high grey level emphasis";
	features[GNU] = "Grey level non uniformity";
	features[GNUN] = "Grey level non uniformity normalized";
	features[RLNU] = "Run length non uniformity";
	features[RLNUN] = "Run length non uniformity normalized";
	features[RP] = "Run percentage";
	features[GLV] = "Grey level variance";
	features[RLV] = "Run length variance";
	features[RE] = "Run entropy";
	
}

