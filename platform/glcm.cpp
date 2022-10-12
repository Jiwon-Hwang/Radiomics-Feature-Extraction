#include "GLCM.h"

using namespace std;

GLCM::GLCM() {

}
GLCM::~GLCM() {

}
void GLCM::clearVariable() {
	// 슬라이스마다, 방향마다 초기화
	vector1DofOriPixelsInROI.clear();
	vector<short>().swap(vector1DofOriPixelsInROI); // size & capacity 모두 0으로 초기화
	diagonalProbabilities.clear();
	vector<float>().swap(diagonalProbabilities);
	crossProbabilities.clear();
	vector<float>().swap(crossProbabilities);
	sumProbCols.clear();
	vector<float>().swap(sumProbCols);
	sumProbRows.clear();
	vector<float>().swap(sumProbRows);

	maxIntensity = -1;

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
void GLCM::clearVector() {

	if (!final2DVec.empty()) {
		final2DVec.clear();
		vector<vector<float>>().swap(final2DVec); // size & capacity 모두 0으로 초기화
	}

	if (!final1DVec.empty()) {
		final1DVec.clear();
		vector<float>().swap(final1DVec);
	}

}

vector<short> GLCM::get1DVectorOfPixels(short* psImage, unsigned char* pucMask) {
	
	vector<short> vector1DofOriPixels(nHeight*nWidth, 0); // 0으로 초기화 후, ROI 바깥 영역은 그대로 0. 미리 크기 할당해 두는게 훨씬 안정적
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			short imageValue = psImage[index];

			// ROI 밖(0)과 안 픽셀값들 모두 할당 (in 1d vector => for fast discretising!)
			vector1DofOriPixels[index] = (maskValue > (unsigned char)0) ? imageValue : 0;
			// ROI 내부값 push (ROI가 없으면 계속 pass => for min, max)
			if (maskValue >(unsigned char)0) {
				vector1DofOriPixelsInROI.push_back(imageValue); // 슬라이스마다 초기화 해줘야 함! (clearVariable)
			}
		}
	}

	return vector1DofOriPixels;
}
vector<vector<unsigned short>> GLCM::get2DVectorOfDiscretizedPixels_FBN(short* psImage, unsigned char* pucMask) {

	// min(front) : -1, max(back) : 180
	float min = (float)*min_element(vector1DofOriPixelsInROI.begin(), vector1DofOriPixelsInROI.end()); // min_element() : pointer return
	float max = (float)*max_element(vector1DofOriPixelsInROI.begin(), vector1DofOriPixelsInROI.end());
	vector<float> tempFloatVec(vector1DofOriPixels.begin(), vector1DofOriPixels.end()); // short -> float 형변환

	if (min == 0 && min == max) {
		cout << "error in calculating discretization, VOI contains only 0" << endl;
		exit(0);
	}
	else if (min > 0 && min == max) {
		cout << "error in calculating discretization, VOI contains only one intensity value, minimum value is set to 0" << endl;
		min = 0;
	}


	// 우선 ROI 밖의 부분까지 모두 양자화 후, 맨 마지막에 if문으로 ROI 밖 영역은 0 처리

	//subtract minimum value from every matrix element
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(minus<float>(), min)); // minus<T> : 이항 연산 함수 객체 (-) / bind2nd : 2번째 인수를 고정해 함수 객체로 변환 / transform : 일괄 연산

	//get the range
	float range = (max - min) / nBins; // range : 몫 (width of a bin) => ***float이 들어가면 / 는 몫이 아니라 진짜 "나누기" 연산!!!***

	//divide every element of the matrix by the range
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(divides<float>(), range));

	//replace 0 to 1 => ibsi 핑크색 마킹 부분! (min - min = 0이 되므로 최소인 1로 바꿔줘야 뒤에 ceil에서 min value of each bin : 1 유지)
	replace(tempFloatVec.begin(), tempFloatVec.end(), 0, 1);

	//do rounding(ceil), type casting(unsigned short) and asigning to 2D Vec (with ROI)
	vector<vector<unsigned short>> vector2DofDiscretizedPixels(nHeight, vector<unsigned short>(nWidth, 0)); // 양자화한 최종 결과 픽셀값들 담을 벡터
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			unsigned short imageValue_discretized = (unsigned short)ceil(tempFloatVec[index]); // psImage (ori) -> tempFloatVec (discretized)

			// ROI 밖(0)과 안(discretized) 픽셀값들 2D Vec에 할당
			vector2DofDiscretizedPixels[row][col] = (maskValue >(unsigned char)0) ? imageValue_discretized : 0;
		}
	}

	// maxIntensity(최댓값) 구하기 => for. sizeMatrix (FBN의 경우엔 nBins)
	maxIntensity = nBins;

	return vector2DofDiscretizedPixels;
}
vector<vector<unsigned short>> GLCM::get2DVectorOfDiscretizedPixels_FBS(short* psImage, unsigned char* pucMask) {

	float min = (float)*min_element(vector1DofOriPixelsInROI.begin(), vector1DofOriPixelsInROI.end());
	float max = (float)*max_element(vector1DofOriPixelsInROI.begin(), vector1DofOriPixelsInROI.end());
	vector<float> tempFloatVec(vector1DofOriPixels.begin(), vector1DofOriPixels.end()); 

	//subtract minimum value from every matrix element
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(minus<float>(), min));

	//divide every element of the matrix by the sBin (width of bin)
	if (isnan(sBin)) {
		cout << "invalid value error : sBin! It will be replaced with nBins=32..." << endl;
		sBin = (max - min) / 32; // range
	}
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(divides<float>(), sBin));
	
	//do rounding(floor) +1, type casting(unsigned short) and asigning to 2D Vec (with ROI)
	vector<vector<unsigned short>> vector2DofDiscretizedPixels(nHeight, vector<unsigned short>(nWidth, 0)); 
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			unsigned short imageValue_discretized = (unsigned short)floor(tempFloatVec[index]) + 1; 

			// ROI 밖(0)과 안(discretized) 픽셀값들 2D Vec에 할당
			vector2DofDiscretizedPixels[row][col] = (maskValue >(unsigned char)0) ? imageValue_discretized : 0;

			// maxIntensity(최댓값) 구하기 => for. sizeMatrix
			maxIntensity = maxIntensity < (int)vector2DofDiscretizedPixels[row][col] ? (int)vector2DofDiscretizedPixels[row][col] : maxIntensity;
		}
	}

	return vector2DofDiscretizedPixels;
}
void GLCM::getXYDirections(int &directionX, int &directionY, int angle) {

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
vector<pair<unsigned short, unsigned short>> GLCM::getNeighbours2D(unsigned char* pucMask, int directionX, int directionY) {
	// ROI 안에 속하는 이웃쌍만 push하기!
	vector<pair<unsigned short, unsigned short>> neighbours;
	unsigned short neighbour1;
	unsigned short neighbour2;

	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth - directionX*(directionX + 1) / 2; col++) {
			// index out of range check
			if (row - directionY > -1 && col + directionX > -1) {
				// isInROI check
				int index1 = row * nWidth + col;
				int index2 = (row - directionY) * nWidth + (col + directionX);
				unsigned char maskValue1 = pucMask[index1];
				unsigned char maskValue2 = pucMask[index2];
				// 두 이웃 픽셀 모두 ROI에 있을 때
				if (maskValue1 >(unsigned char)0 && maskValue2 >(unsigned char)0) {
					neighbour1 = vector2DofDiscretizedPixels[row][col];
					neighbour2 = vector2DofDiscretizedPixels[row - directionY][col + directionX];

					neighbours.push_back(make_pair(neighbour1, neighbour2));
				}
			}
		}
	}

	return neighbours;

}
void GLCM::fill2DGLCMatrix(vector<vector<float>> &GLCMatrix, unsigned char* pucMask, int angle) {

	//vector in which all neihbor pairs are stored
	vector<pair<unsigned short, unsigned short>> neighbours;
	int directionX;
	int directionY;

	//define in which direction you have to look for a neighbor
	getXYDirections(directionX, directionY, angle); // 방향 당 1회씩만
	neighbours = getNeighbours2D(pucMask, directionX, directionY);
	pair<unsigned short, unsigned short> actNeighbour;

	//iterate over the neighbor-vector
	for (int neighbourNr = 0; neighbourNr<neighbours.size(); neighbourNr++) {
		actNeighbour = neighbours[neighbourNr];

		if (!isnan(float(actNeighbour.first)) && !isnan(float(actNeighbour.second))) {
			GLCMatrix[actNeighbour.first - 1][actNeighbour.second - 1] += 1;
		}
	}
}
void GLCM::calcDiagonalProbabilities(vector<vector<float>> GLCMatrix) {

	float diagProbability;

	for (int k = 0; k < sizeMatrix; k++) {
		diagProbability = 0;
		//get the diagonal elements
		for (int row = 0; row < sizeMatrix; row++) {
			for (int col = 0; col < sizeMatrix; col++) {
				//Kronecker delta
				if (k == abs(row - col)) {
					diagProbability += GLCMatrix[row][col];
				}
			}
		}
		//store the diagonal probabilities in a vector
		diagonalProbabilities.push_back(diagProbability);
	}
}
void GLCM::calcCrossProbabilities(vector<vector<float>> GLCMatrix) {

	float crossProbability;

	for (int k = 0; k < 2 * sizeMatrix; k++) {
		crossProbability = 0;
		for (int row = 0; row < sizeMatrix; row++) {
			for (int col = 0; col < sizeMatrix; col++) {
				//Kronecker delta
				if (k == abs(row + col)) {
					crossProbability += GLCMatrix[row][col];
				}
			}
		}
		crossProbabilities.push_back(crossProbability);
	}
}
void GLCM::average4DirValues(vector<vector<float>> temp4DirVals2DVec, vector<float> &tempValues1DVec) {

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
void inverse(vector<vector<float>> matrix, vector<vector<float>> &inverseMatrix) {

	for (int i = 0; i<matrix.size(); i++) {
		for (int j = 0; j<matrix[0].size(); j++) {
			inverseMatrix[j][i] = matrix[i][j];
		}
	}

}
void matrixSum(vector<vector<float>> &matrix1, vector<vector<float>> matrix2) {

	for (int i = 0; i<matrix1.size(); i++) {
		for (int j = 0; j<matrix1[0].size(); j++) {
			matrix1[i][j] += matrix2[i][j];
		}
	}

}
float getSumOfElements(vector<vector<float>> matrix) {
	
	float sum = 0;
	for (int i = 0; i<matrix.size(); i++) {
		for (int j = 0; j<matrix[0].size(); j++) {
			sum += matrix[i][j];
		}
	}

	return sum;
}
void divideMatrix(vector<vector<float>> &matrix, float divisor) {

	for (int i = 0; i < matrix.size(); i++) {
		transform(matrix[i].begin(), matrix[i].end(), matrix[i].begin(), bind2nd(divides<float>(), (int)divisor));
	}

}

void GLCM::calcJointMaximum(vector<vector<float>> GLCMatrix) {
	
	jointMaximum = 0;

	for (int row = 0; row < sizeMatrix; row++) {
		for (int col = 0; col < sizeMatrix; col++) {
			jointMaximum = (GLCMatrix[row][col] > jointMaximum) ? GLCMatrix[row][col] : jointMaximum;
		}
	}
}
void GLCM::calcJointAverage(vector<vector<float>> GLCMatrix) {

	jointAverage = 0;

	for (int row = 0; row < sizeMatrix; row++) {
		for (int col = 0; col < sizeMatrix; col++) {
			jointAverage += (row + 1) * GLCMatrix[row][col];
		}
	}
}
void GLCM::calcJointVariance(vector<vector<float>> GLCMatrix) {

	jointVariance = 0;

	if (isnan(jointAverage)) {
		calcJointAverage(GLCMatrix);
	}

	for (int row = 0; row < sizeMatrix; row++) {
		for (int col = 0; col < sizeMatrix; col++) {
			jointVariance += pow((row + 1 - jointAverage), 2) * GLCMatrix[row][col];
		}
	}
}
void GLCM::calcJointEntropy(vector<vector<float>> GLCMatrix) {
	
	jointEntropy = 0;

	float tempElement;
	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			tempElement = GLCMatrix[i][j];
			if (tempElement != 0) {
				jointEntropy -= tempElement * log2(tempElement);
			}
		}
	}
}
void GLCM::calcDiffAverage(vector<vector<float>> GLCMatrix) {

	diffAverage = 0;

	for (int diagElement = 0; diagElement < diagonalProbabilities.size(); diagElement++) {
		diffAverage += diagonalProbabilities[diagElement] * diagElement;
	}
}
void GLCM::calcDiffVariance(vector<vector<float>> GLCMatrix) {

	diffVariance = 0;

	if (isnan(diffAverage)) {
		calcDiffAverage(GLCMatrix);
	}

	for (int diagElement = 0; diagElement < diagonalProbabilities.size(); diagElement++) {
		diffVariance += diagonalProbabilities[diagElement] * pow((diagElement - diffAverage), 2);
	}
}
void GLCM::calcDiffEntropy(vector<vector<float>> GLCMatrix) {

	diffEntropy = 0;

	float tempElement = 0;
	for (int k = 0; k < diagonalProbabilities.size(); k++) {
		tempElement = diagonalProbabilities[k];
		if (tempElement != 0) {
			diffEntropy -= tempElement*log2(tempElement);
		}
	}
}
void GLCM::calcSumAverage(vector<vector<float>> GLCMatrix) {

	sumAverage = 0;

	for (int crossElement = 1; crossElement < crossProbabilities.size() + 1; crossElement++) {
		sumAverage += crossProbabilities[crossElement - 1] * (crossElement + 1);
	}
}
void GLCM::calcSumVariance(vector<vector<float>> GLCMatrix) {

	sumVariance = 0;

	if (isnan(sumAverage)) {
		calcSumAverage(GLCMatrix);
	}

	for (int crossElement = 1; crossElement < crossProbabilities.size() + 1; crossElement++) {
		sumVariance += crossProbabilities[crossElement - 1] * pow((crossElement + 1 - sumAverage), 2);
	}
}
void GLCM::calcSumEntropy(vector<vector<float>> GLCMatrix) {

	sumEntropy = 0;

	float tempElement = 0;
	for (int k = 0; k < crossProbabilities.size(); k++) {
		tempElement = crossProbabilities[k];
		if (tempElement != 0 && !isnan(tempElement)) {
			sumEntropy -= tempElement*log2(tempElement);
		}
	}
}
void GLCM::calcAngSecMoment(vector<vector<float>> GLCMatrix) {
	
	// Angular Second Moment (== Energy, Uniformity)
	angSecMoment = 0;

	for (int row = 0; row < sizeMatrix; row++) {
		for (int col = 0; col < sizeMatrix; col++) {
			angSecMoment += pow(GLCMatrix[row][col], 2);
		}
	}

}
void GLCM::calcContrast(vector<vector<float>> GLCMatrix) {

	contrast = 0;

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			contrast += pow((i - j), 2)*GLCMatrix[i][j];
		}
	}
}
void GLCM::calcDissimilarity(vector<vector<float>> GLCMatrix) {

	dissimilarity = 0;

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			dissimilarity += abs(i - j)*GLCMatrix[i][j];
		}
	}
}
void GLCM::calcInverseDiff(vector<vector<float>> GLCMatrix) {

	inverseDiff = 0;

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			inverseDiff += GLCMatrix[i][j] / (1 + abs(i - j));
		}
	}
}
void GLCM::calcInverseDiffNorm(vector<vector<float>> GLCMatrix) {

	inverseDiffNorm = 0;

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			inverseDiffNorm += GLCMatrix[i][j] / (1 + float(abs(i - j)) / float(sizeMatrix));
		}
	}
}
void GLCM::calcInverseDiffMom(vector<vector<float>> GLCMatrix) {

	inverseDiffMom = 0;

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			inverseDiffMom += GLCMatrix[i][j] / (1 + pow((i - j), 2));
		}
	}
}
void GLCM::calcInverseDiffMomNorm(vector<vector<float>> GLCMatrix) {

	inverseDiffMomNorm = 0;

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			inverseDiffMomNorm += GLCMatrix[i][j] / (1 + pow((i - j), 2) / pow(sizeMatrix, 2));
		}
	}
}
void GLCM::calcInverseVar(vector<vector<float>> GLCMatrix) {

	inverseVar = 0;

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = i + 1; j < sizeMatrix; j++) {
			inverseVar += 2 * GLCMatrix[i][j] / pow((i - j), 2);
		}
	}
}
void GLCM::calcColProb(vector<vector<float>> GLCMatrix) {

	float colProb;
	sumProbCols.clear();

	for (int j = 0; j < sizeMatrix; j++) {
		colProb = 0;
		for (int i = 0; i < sizeMatrix; i++) {
			colProb += GLCMatrix[i][j];
		}
		sumProbCols.push_back(colProb);
	}
}
void GLCM::calcRowProb(vector<vector<float>> GLCMatrix) {

	float rowProb;
	sumProbRows.clear();

	for (int i = 0; i < sizeMatrix; i++) {
		rowProb = 0;
		for (int j = 0; j < sizeMatrix; j++) {
			rowProb += GLCMatrix[i][j];
		}
		sumProbRows.push_back(rowProb);
	}
}
void GLCM::calcMeanColProb(vector<vector<float>> GLCMatrix) {

	calcColProb(GLCMatrix);

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
void GLCM::calcMeanRowProb(vector<vector<float>> GLCMatrix) {

	calcRowProb(GLCMatrix);

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
void GLCM::calcCorrelation(vector<vector<float>> GLCMatrix) {

	calcMeanColProb(GLCMatrix); 
	calcMeanRowProb(GLCMatrix); 

	correlation = 0;

	for (int row = 1; row < sizeMatrix + 1; row++) {
		for (int col = 1; col < sizeMatrix + 1; col++) {
			correlation += (row - meanRowProb)*(col - meanRowProb)*GLCMatrix[row - 1][col - 1];
		}
	}

	if (!isnan(correlation) && stdRowProb != 0 && !isnan(stdRowProb)) {
		correlation = correlation / pow(stdRowProb, 2);
	}
}
void GLCM::calcAutoCorrelation(vector<vector<float>> GLCMatrix) {

	autoCorrelation = 0;

	for (int row = 1; row < sizeMatrix + 1; row++) {
		for (int col = 1; col < sizeMatrix + 1; col++) {
			autoCorrelation += GLCMatrix[row - 1][col - 1] * (row)*(col);
		}
	}
}
void GLCM::calcClusterTendency(vector<vector<float>> GLCMatrix) {

	clusterTendency = 0;

	if (isnan(meanRowProb)) {
		calcMeanRowProb(GLCMatrix);
	}

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			clusterTendency += pow((i + j + 2 - 2 * meanRowProb), 2)*GLCMatrix[i][j];
		}
	}

}
void GLCM::calcClusterShade(vector<vector<float>> GLCMatrix) {

	clusterShade = 0;

	if (isnan(meanRowProb)) {
		calcMeanRowProb(GLCMatrix);
	}

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			clusterShade += pow((i + j + 2 - 2 * meanRowProb), 3)*GLCMatrix[i][j];
		}
	}

}
void GLCM::calcClusterProminence(vector<vector<float>> GLCMatrix) {

	clusterProminence = 0;

	if (isnan(meanRowProb)) {
		calcMeanRowProb(GLCMatrix);
	}

	for (int i = 0; i < sizeMatrix; i++) {
		for (int j = 0; j < sizeMatrix; j++) {
			clusterProminence += pow((i + j + 2 - 2 * meanRowProb), 4)*GLCMatrix[i][j];
		}
	}
}
void GLCM::calcFirstMCorrelation(vector<vector<float>> GLCMatrix) {

	calcColProb(GLCMatrix); // for. sumProbCols
	calcRowProb(GLCMatrix); // for. sumProbRows
	
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
				if (actualProbCols != 0 && GLCMatrix[i][j] != 0 && !isnan(GLCMatrix[i][j]) && actualProbRows != 0) {
					HXY -= GLCMatrix[i][j] * log2(GLCMatrix[i][j]);
					HXY1 -= GLCMatrix[i][j] * log2(actualProbRows*actualProbCols);
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
void GLCM::calcSecondMCorrelation(vector<vector<float>> GLCMatrix) {

	if (isnan(HXY)) {
		calcFirstMCorrelation(GLCMatrix);
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

void GLCM::calcFeature(int FEATURE_IDX, vector<float> &temp1DirVals1DVec, vector<vector<float>> sumGLCM) {

	switch (FEATURE_IDX)
	{
	case JOINTMAXIMUM:
		calcJointMaximum(sumGLCM);
		temp1DirVals1DVec.push_back(jointMaximum);
		break;

	case JOINTAVERAGE:
		calcJointAverage(sumGLCM);
		temp1DirVals1DVec.push_back(jointAverage);
		break;

	case JOINTVARIANCE:
		calcJointVariance(sumGLCM);
		temp1DirVals1DVec.push_back(jointVariance);
		break;

	case JOINTENTROPY:
		calcJointEntropy(sumGLCM);
		temp1DirVals1DVec.push_back(jointEntropy);
		break; 

	case DIFFAVERAGE:
		calcDiffAverage(sumGLCM);
		temp1DirVals1DVec.push_back(diffAverage);
		break;

	case DIFFVARIANCE:
		calcDiffVariance(sumGLCM);
		temp1DirVals1DVec.push_back(diffVariance);
		break;

	case DIFFENTROPY:
		calcDiffEntropy(sumGLCM);
		temp1DirVals1DVec.push_back(diffEntropy);
		break;

	case SUMAVERAGE:
		calcSumAverage(sumGLCM);
		temp1DirVals1DVec.push_back(sumAverage);
		break;

	case SUMVARIANCE:
		calcSumVariance(sumGLCM);
		temp1DirVals1DVec.push_back(sumVariance);
		break;

	case SUMENTROPY:
		calcSumEntropy(sumGLCM);
		temp1DirVals1DVec.push_back(sumEntropy);
		break;

	case ANGSECMOMENT:
		calcAngSecMoment(sumGLCM);
		temp1DirVals1DVec.push_back(angSecMoment);
		break;

	case CONTRAST:
		calcContrast(sumGLCM);
		temp1DirVals1DVec.push_back(contrast);
		break;

	case DISSIMILARITY:
		calcDissimilarity(sumGLCM);
		temp1DirVals1DVec.push_back(dissimilarity);
		break;

	case INVERSEDIFF:
		calcInverseDiff(sumGLCM);
		temp1DirVals1DVec.push_back(inverseDiff);
		break;

	case INVERSEDIFFNORM:
		calcInverseDiffNorm(sumGLCM);
		temp1DirVals1DVec.push_back(inverseDiffNorm);
		break;

	case INVERSEDIFFMOM:
		calcInverseDiffMom(sumGLCM);
		temp1DirVals1DVec.push_back(inverseDiffMom);
		break;

	case INVERSEDIFFMOMNORM:
		calcInverseDiffMomNorm(sumGLCM);
		temp1DirVals1DVec.push_back(inverseDiffMomNorm);
		break;

	case INVERSEVAR:
		calcInverseVar(sumGLCM);
		temp1DirVals1DVec.push_back(inverseVar);
		break;

	case CORRELATION:
		calcCorrelation(sumGLCM);
		temp1DirVals1DVec.push_back(correlation);
		break;

	case AUTOCORRELATION:
		calcAutoCorrelation(sumGLCM);
		temp1DirVals1DVec.push_back(autoCorrelation);
		break;

	case CLUSTERTENDENCY:
		calcClusterTendency(sumGLCM);
		temp1DirVals1DVec.push_back(clusterTendency);
		break;

	case CLUSTERSHADE:
		calcClusterShade(sumGLCM);
		temp1DirVals1DVec.push_back(clusterShade);
		break;

	case CLUSTERPROMINENCE:
		calcClusterProminence(sumGLCM);
		temp1DirVals1DVec.push_back(clusterProminence);
		break;

	case FIRSTMCORRELATION:
		calcFirstMCorrelation(sumGLCM);
		temp1DirVals1DVec.push_back(firstMCorrelation);
		break;

	case SECONDMCORRELATION:
		calcSecondMCorrelation(sumGLCM);
		temp1DirVals1DVec.push_back(secondMCorrelation);
		break;


	default:
		//cout << "error : Unknown GLCM Feature!" << endl;
		break;
	}
}
void GLCM::featureExtraction(short* psImage, unsigned char* pucMask, int nHeight_, int nWidth_) {

	// claer all values
	clearVariable(); // 슬라이스마다 초기화

	// get discretized 2D input matrix (2d vector)
	nHeight = nHeight_;
	nWidth = nWidth_;
	vector1DofOriPixels = get1DVectorOfPixels(psImage, pucMask); 
	vector2DofDiscretizedPixels = isFBN? get2DVectorOfDiscretizedPixels_FBN(psImage, pucMask) : get2DVectorOfDiscretizedPixels_FBS(psImage, pucMask); // 슬라이스마다 초기화, calc maxIntensity

	vector<vector<float>> temp4DirVals2DVec;	// 슬라이스마다 초기화 (for. 4방향 1d vec 평균내기)
	vector<float> tempValues1DVec;				// 슬라이스마다 초기화 (for. final2DVec에 누적)

	// calculate checked feature (4 directions)
	int ang;
	sizeMatrix = maxIntensity; // maxIntensity만 clearVariable()을 통해 매번 초기화
	float sumMatrElement;
	for (int dir = 0; dir < 4; dir++) {

		clearVariable();					// 방향마다 초기화
		vector<float> temp1DirVals1DVec;	// 방향마다 초기화

		// calculate GLCM matrix for each dir
		ang = 180 - dir * 45;
		vector<vector<float>> sum; 
		vector<vector<float>> inverseMatrix(sizeMatrix, vector<float>(sizeMatrix, 0)); // 미리 공간 할당 해줘야 추후 함수에서 index로 접근 가능! (out of index error)
		vector<vector<float>> GLCMatrix(sizeMatrix, vector<float>(sizeMatrix, 0));
		fill2DGLCMatrix(GLCMatrix, pucMask, ang);

		sum = GLCMatrix;
		inverse(GLCMatrix, inverseMatrix);	// get inverseMatrix
		matrixSum(sum, inverseMatrix);		// get sum (GLCM + inverseGLCM)

		//calculate the sum of all matrix elements (= the number of neighbor-pairs in the matrix
		sumMatrElement = getSumOfElements(sum);
		
		//divide the whole matrix by the sum to obtain matrix elements representing the probabilities of the occurence of a neighbor pair
		if (sumMatrElement != 0) divideMatrix(sum, sumMatrElement);
		
		calcDiagonalProbabilities(sum);
		calcCrossProbabilities(sum);

		// calculate feature for each dir
		for (int i = 0; i < FEATURE_COUNT; i++) {
			if (isCheckedFeature[i]) calcFeature(i, temp1DirVals1DVec, sum);
		}
		temp4DirVals2DVec.push_back(temp1DirVals1DVec); // 4행
	}

	average4DirValues(temp4DirVals2DVec, tempValues1DVec); // 4방향 평균 특징값

	final2DVec.push_back(tempValues1DVec); // 모든 ROI 슬라이스 들어올 때까지 누적 (시리즈마다 초기화)
	
}

void GLCM::averageAllValues() {

	// get final mean vector
	for (int col = 0, nCols = final2DVec[0].size(); col < nCols; col++) {
		float colSum = 0;
		float colMean;

		for (int row = 0, nRows = final2DVec.size(); row < nRows; row++) {
			colSum += final2DVec[row][col];
		}
		colMean = colSum / final2DVec.size();
		final1DVec.push_back(colMean);
	}

}

void GLCM::defineFeatureNames(vector<string> &features) {
	// 총 25가지
	features[JOINTMAXIMUM] = "joint maximum";
	features[JOINTAVERAGE] = "joint average";
	features[JOINTVARIANCE] = "joint variance";
	features[JOINTENTROPY] = "joint entropy";
	features[DIFFAVERAGE] = "difference average";
	features[DIFFVARIANCE] = "difference variance";
	features[DIFFENTROPY] = "difference entropy";
	features[SUMAVERAGE] = "sum average";
	features[SUMVARIANCE] = "sum variance";
	features[SUMENTROPY] = "sum entropy";
	features[ANGSECMOMENT] = "angular second moment";
	features[CONTRAST] = "contrast";
	features[DISSIMILARITY] = "dissimilarity";
	features[INVERSEDIFF] = "inverse difference";
	features[INVERSEDIFFNORM] = "inverse difference normalised";
	features[INVERSEDIFFMOM] = "inverse difference moment";
	features[INVERSEDIFFMOMNORM] = "inverse difference moment normalised";
	features[INVERSEVAR] = "inverse variance";
	features[CORRELATION] = "correlation";
	features[AUTOCORRELATION] = "autocorrelation";
	features[CLUSTERTENDENCY] = "cluster tendency";
	features[CLUSTERSHADE] = "cluster shade";
	features[CLUSTERPROMINENCE] = "cluster prominence";
	features[FIRSTMCORRELATION] = "first measure of information correlation";
	features[SECONDMCORRELATION] = "second measure of information correlation";

}

