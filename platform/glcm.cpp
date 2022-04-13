#include "GLCM.h"

using namespace std;

GLCM::GLCM() {

}
GLCM::~GLCM() {

}
void GLCM::clearVariable() {

	vector1DofOriPixelsInROI.clear();
	vector<short>().swap(vector1DofOriPixelsInROI); // size & capacity ��� 0���� �ʱ�ȭ

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
	//meanRowProb = NAN;
	//meanColProb = NAN;
	//stdRowProb = NAN;
	//stdColProb = NAN;
	correlation = NAN;
	autoCorrelation = NAN;
	clusterTendency = NAN;
	clusterShade = NAN;
	clusterProminence = NAN;
	firstMCorrelation = NAN;
	secondMCorrelation = NAN;

}
void GLCM::clearVector() {

	if (!final2DVec.empty()) {
		final2DVec.clear();
		vector<vector<float>>().swap(final2DVec); // size & capacity ��� 0���� �ʱ�ȭ
	}

	if (!final1DVec.empty()) {
		final1DVec.clear();
		vector<float>().swap(final1DVec);
	}

}

vector<short> GLCM::get1DVectorOfPixels(short* psImage, unsigned char* pucMask, int nHeight, int nWidth) {
	
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
vector<vector<unsigned short>> GLCM::get2DVectorOfDiscretizedPixels_nBins(short* psImage, unsigned char* pucMask, int nHeight, int nWidth) {

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
	// ROI �ȿ� ���ϴ� �̿��ָ� push�ϱ�!
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
				// �� �̿� �ȼ� ��� ROI�� ���� ��
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
	getXYDirections(directionX, directionY, angle); // ���� �� 1ȸ����
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

void GLCM::calcFeature(int FEATURE_IDX, vector<float> &temp1DirVals1DVec, vector<vector<float>> sumGLCM) {

	switch (FEATURE_IDX)
	{
	case JOINTMAXIMUM:
		calcJointMaximum(sumGLCM);
		temp1DirVals1DVec.push_back(jointMaximum);
		break;


	default:
		//cout << "error : Unknown GLCM Feature!" << endl;
		break;
	}
}
void GLCM::featureExtraction(short* psImage, unsigned char* pucMask, int nHeight_, int nWidth_) {

	// claer all values
	clearVariable(); // �����̽����� �ʱ�ȭ

	// get discretized 2D input matrix (2d vector)
	nHeight = nHeight_;
	nWidth = nWidth_;
	vector1DofOriPixels = get1DVectorOfPixels(psImage, pucMask, nHeight, nWidth); // **nHeight, nWidth ���ڷ� �ȳ־��൵ �� => ����**
	vector2DofDiscretizedPixels = get2DVectorOfDiscretizedPixels_nBins(psImage, pucMask, nHeight, nWidth); // �����̽����� �ʱ�ȭ
	
	vector<vector<float>> temp4DirVals2DVec;	// �����̽����� �ʱ�ȭ (for. 4���� 1d vec ��ճ���)
	vector<float> tempValues1DVec;				// �����̽����� �ʱ�ȭ (for. final2DVec�� ����)

	// calculate checked feature (4 directions)
	int ang;
	sizeMatrix = nBins;
	float sumMatrElement;
	for (int dir = 0; dir < 4; dir++) {

		clearVariable();					// ���⸶�� �ʱ�ȭ
		vector<float> temp1DirVals1DVec;	// ���⸶�� �ʱ�ȭ

		// calculate GLCM matrix for each dir
		ang = 180 - dir * 45;
		vector<vector<float>> sum; 
		vector<vector<float>> inverseMatrix(sizeMatrix, vector<float>(sizeMatrix, 0)); // �̸� ���� �Ҵ� ����� ���� �Լ����� index�� ���� ����! (out of index error)
		vector<vector<float>> GLCMatrix(sizeMatrix, vector<float>(sizeMatrix, 0));
		fill2DGLCMatrix(GLCMatrix, pucMask, ang);

		sum = GLCMatrix;
		inverse(GLCMatrix, inverseMatrix);	// get inverseMatrix
		matrixSum(sum, inverseMatrix);		// get sum (GLCM + inverseGLCM)

		//calculate the sum of all matrix elements (= the number of neighbor-pairs in the matrix
		sumMatrElement = getSumOfElements(sum);
		
		//divide the whole matrix by the sum to obtain matrix elements representing the probabilities of the occurence of a neighbor pair
		if (sumMatrElement != 0) divideMatrix(sum, sumMatrElement);


		// calculate feature for each dir
		for (int i = 0; i < FEATURE_COUNT; i++) {
			if (isCheckedFeature[i]) calcFeature(i, temp1DirVals1DVec, sum);
		}
		temp4DirVals2DVec.push_back(temp1DirVals1DVec); // 4��
	}

	average4DirValues(temp4DirVals2DVec, tempValues1DVec); // 4���� ��� Ư¡��

	final2DVec.push_back(tempValues1DVec); // ��� ROI �����̽� ���� ������ ���� (�ø���� �ʱ�ȭ)
	
}

void GLCM::averageAllValues() {

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

void GLCM::defineFeatureNames(vector<string> &features) {
	// �� 25����
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
void GLCM::extractFeatureValues(vector<float> &glcmValues) {
	// platform.cpp�� writeCSVCheckedValue()���� ���� ����. But, �ӵ� ������ ��� x 
	/*
	intensityHistogramValues[MEAN] = meanValue;
	intensityHistogramValues[VARIANCE] = varianceValue;
	intensityHistogramValues[SKEWNESS] = skewnessValue;
	intensityHistogramValues[KURTOSIS] = kurtosisValue;
	intensityHistogramValues[MEDIAN] = medianValue;
	intensityHistogramValues[MINIMUM] = minimumValue;
	intensityHistogramValues[PERCENTILE10] = percentile10;
	intensityHistogramValues[PERCENTILE90] = percentile90;
	intensityHistogramValues[MAXIMUM] = maximumValue;
	intensityHistogramValues[INTERQUARTILERANGE] = interquartileRange;
	intensityHistogramValues[MODE] = mode;
	intensityHistogramValues[RANGE] = rangeValue;
	intensityHistogramValues[MEANABSDEV] = meanAbsDev;
	intensityHistogramValues[ROBUSTMEANABSDEV] = robustMeanAbsDev;
	intensityHistogramValues[MEDIANABSDEV] = medianAbsDev;
	intensityHistogramValues[COEFFOFVAR] = coeffOfVar;
	intensityHistogramValues[QUARTILECOEFF] = quartileCoeff;
	intensityHistogramValues[ENTROPY] = entropy;
	intensityHistogramValues[UNIFORMITY] = uniformity;
	intensityHistogramValues[MAXHISTGRADIENT] = maxHistGradient;
	intensityHistogramValues[MAXHISTGRADGREY] = maxHistGradGreyValue;
	intensityHistogramValues[MINHISTGRADIENT] = minHistGradient;
	intensityHistogramValues[MINHISTGRADGREY] = minHistGradGreyValue;
	*/
}

