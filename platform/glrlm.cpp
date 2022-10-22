#include "GLRLM.h"

using namespace std;

GLRLM::GLRLM() {

}
GLRLM::~GLRLM() {

}
void GLRLM::clearVariable() {
	// �����̽�����, ���⸶�� �ʱ�ȭ
	vector1DofOriPixelsInROI.clear();
	vector<short>().swap(vector1DofOriPixelsInROI); // size & capacity ��� 0���� �ʱ�ȭ
	rowSums.clear();
	vector<float>().swap(rowSums);
	colSums.clear();
	vector<float>().swap(colSums);

	maxIntensity = -1;

	shortRunEmph = NAN;
	longRunEmph = NAN;
	lowGreyRunEmph = NAN;
	highGreyRunEmph = NAN;
	shortRunLowEmph = NAN;
	shortRunHighEmph = NAN;
	longRunLowEmph = NAN;
	longRunHighEmph = NAN;
	greyNonUnimformity = NAN;
	greyNonUnimformityNorm = NAN;
	runLengthNonUniformity = NAN;
	runLengthNonUniformityNorm = NAN;
	runPercentage = NAN;
	greyLevelVar = NAN;
	runLengthVar = NAN;
	runEntropy = NAN;

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
	nPixelsInROI = vector1DofOriPixelsInROI.size();

	return vector1DofOriPixels;
}
vector<vector<unsigned short>> GLRLM::get2DVectorOfDiscretizedPixels_FBN(short* psImage, unsigned char* pucMask) {

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

	// clear diffGreyLevels (�����̽����� �ʱ�ȭ)
	diffGreyLevels.clear();
	vector<unsigned short>().swap(diffGreyLevels);

	//do rounding(ceil), type casting(unsigned short) and asigning to 2D Vec (with ROI)
	vector<vector<unsigned short>> vector2DofDiscretizedPixels(nHeight, vector<unsigned short>(nWidth, 0)); // ����ȭ�� ���� ��� �ȼ����� ���� ����
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			unsigned short imageValue_discretized = (unsigned short)ceil(tempFloatVec[index]); // psImage (ori) -> tempFloatVec (discretized)

			// ROI ��(0)�� ��(discretized) �ȼ����� 2D Vec�� �Ҵ�
			if (maskValue >(unsigned char)0) {
				vector2DofDiscretizedPixels[row][col] = imageValue_discretized;
				// diffGreyLevels�� ���
				diffGreyLevels.push_back(imageValue_discretized);
			}
			else {
				vector2DofDiscretizedPixels[row][col] = 0;
			}
		}
	}

	// maxIntensity(�ִ�) ���ϱ� => for. sizeMatrix (FBN�� ��쿣 nBins)
	maxIntensity = nBins;

	// get diffGreyLevels (vector containing the different grey levels of the matrix -> extract every element only once)
	sort(diffGreyLevels.begin(), diffGreyLevels.end());
	diffGreyLevels.erase(unique(diffGreyLevels.begin(), diffGreyLevels.end()), diffGreyLevels.end()); // �ߺ� ����

	return vector2DofDiscretizedPixels;
}
vector<vector<unsigned short>> GLRLM::get2DVectorOfDiscretizedPixels_FBS(short* psImage, unsigned char* pucMask) {

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

	// clear diffGreyLevels (�����̽����� �ʱ�ȭ)
	diffGreyLevels.clear();
	vector<unsigned short>().swap(diffGreyLevels);

	//do rounding(floor) +1, type casting(unsigned short) and asigning to 2D Vec (with ROI)
	vector<vector<unsigned short>> vector2DofDiscretizedPixels(nHeight, vector<unsigned short>(nWidth, 0));
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			unsigned short imageValue_discretized = (unsigned short)floor(tempFloatVec[index]) + 1;

			// ROI ��(0)�� ��(discretized) �ȼ����� 2D Vec�� �Ҵ�
			if (maskValue > (unsigned char)0) {
				vector2DofDiscretizedPixels[row][col] = imageValue_discretized;
				// diffGreyLevels�� ���
				diffGreyLevels.push_back(imageValue_discretized);
			}
			else {
				vector2DofDiscretizedPixels[row][col] = 0;
			}

			// maxIntensity(�ִ�) ���ϱ� => for. sizeMatrix
			maxIntensity = maxIntensity < (int)vector2DofDiscretizedPixels[row][col] ? (int)vector2DofDiscretizedPixels[row][col] : maxIntensity;
		}
	}

	// get diffGreyLevels (vector containing the different grey levels of the matrix -> extract every element only once)
	sort(diffGreyLevels.begin(), diffGreyLevels.end());
	diffGreyLevels.erase(unique(diffGreyLevels.begin(), diffGreyLevels.end()), diffGreyLevels.end()); 

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
int GLRLM::findIndex(vector<unsigned short> diffGreyLevels, int size, unsigned short target) {
	int i = 0;
	while ((i < size) && (diffGreyLevels[i] != target)) i++; 
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
			actGreyIndex = findIndex(diffGreyLevels, diffGreyLevels.size(), actElement); // in ROI : 0~(nBins-1), not in ROI : -1 return
			//if the actual matrix element is not 0(NAN) (=is in ROI)
			if (actElement) {
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
void GLRLM::fill2DprobMatrix(vector<vector<float>> GLRLMatrix, vector<vector<float>> &probMatrix) {
	
	probMatrix = GLRLMatrix; // ���� ����
	
	for (int i = 0; i < probMatrix.size(); i++) {
		transform(probMatrix[i].begin(), probMatrix[i].end(), probMatrix[i].begin(), bind2nd(divides<float>(), int(totalSum)));
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
float GLRLM::getMeanProbGrey(vector<vector<float>> probMatrix) {
	// calculate the mean probability of the appearance of every grey level
	float mean = 0;
	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			mean += (i+1) * probMatrix[i][j];
		}
	}

	return mean;
}
float GLRLM::getMeanProbRun(vector<vector<float>> probMatrix) {
	// calculate the mean probability of the runlength
	float mean = 0;
	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			mean += j * probMatrix[i][j];
		}
	}

	return mean;
}

void GLRLM::calcShortRunEmph() {
	
	shortRunEmph = 0;

	if (totalSum != 0) {
		for (int j = 0; j < rowSums.size(); j++) {
			shortRunEmph += rowSums[j] / pow(j + 1, 2);
		}
		shortRunEmph /= totalSum;
	}
	
}
void GLRLM::calcLongRunEmph() {

	longRunEmph = 0;

	if (totalSum != 0) {
		for (int j = 0; j < rowSums.size(); j++) {
			longRunEmph += rowSums[j] * pow(j + 1, 2);
		}
		longRunEmph /= totalSum;
	}

}
void GLRLM::calcLowGreyRunEmph() {

	lowGreyRunEmph = 0;

	if (totalSum != 0) {
		for (int i = 0; i < colSums.size(); i++) {
			lowGreyRunEmph += colSums[i] / pow(i + 1, 2);
		}
		lowGreyRunEmph /= totalSum;
	}

}
void GLRLM::calcHighGreyRunEmph() {
	
	highGreyRunEmph = 0;

	if (totalSum != 0) {
		for (int i = 0; i < colSums.size(); i++) {
			highGreyRunEmph += colSums[i] * pow(i + 1, 2);
		}
		highGreyRunEmph /= totalSum;
	}

}
void GLRLM::calcShortRunLowEmph(vector<vector<float>> GLRLMatrix) {

	shortRunLowEmph = 0;

	if (totalSum != 0) {
		for (int row = 0; row < GLRLMatrix.size(); row++) {
			for (int col = 1; col < GLRLMatrix[0].size() + 1; col++) {
				shortRunLowEmph += GLRLMatrix[row][col - 1] / (pow(row+1, 2) * pow(col, 2));
			}
		}
		shortRunLowEmph /= totalSum;
	}

}
void GLRLM::calcShortRunHighEmph(vector<vector<float>> GLRLMatrix) {

	shortRunHighEmph = 0;

	if (totalSum != 0) {
		for (int row = 0; row < GLRLMatrix.size(); row++) {
			for (int col = 1; col < GLRLMatrix[0].size() + 1; col++) {
				shortRunHighEmph += pow(row+1, 2) * GLRLMatrix[row][col - 1] / pow(col, 2);
			}
		}
		shortRunHighEmph /= totalSum;
	}

}
void GLRLM::calcLongRunLowEmph(vector<vector<float>> GLRLMatrix) {

	longRunLowEmph = 0;

	if (totalSum != 0) {
		for (int row = 0; row < GLRLMatrix.size(); row++) {
			for (int col = 1; col < GLRLMatrix[0].size() + 1; col++) {
				longRunLowEmph += pow(col, 2) * GLRLMatrix[row][col - 1] / pow(row+1, 2);
			}
		}
		longRunLowEmph /= totalSum;
	}

}
void GLRLM::calcLongRunHighEmph(vector<vector<float>> GLRLMatrix) {

	longRunHighEmph = 0;

	if (totalSum != 0) {
		for (int row = 0; row < GLRLMatrix.size(); row++) {
			for (int col = 1; col < GLRLMatrix[0].size() + 1; col++) {
				longRunHighEmph += pow(col, 2) * pow(row+1, 2) * GLRLMatrix[row][col - 1];
			}
		}
		longRunHighEmph /= totalSum;
	}

}
void GLRLM::calcGreyNonUnimformity() {

	greyNonUnimformity = 0;

	if (totalSum != 0) {
		for (int i = 0; i < colSums.size(); i++) {
			greyNonUnimformity += pow(colSums[i], 2);
		}
		greyNonUnimformity /= totalSum;
	}

}
void GLRLM::calcGreyNonUnimformityNorm() {

	greyNonUnimformityNorm = 0;

	if (totalSum != 0) {
		for (int i = 0; i < colSums.size(); i++) {
			greyNonUnimformityNorm += pow(colSums[i], 2);
		}
		greyNonUnimformityNorm /= pow(totalSum, 2);
	}

}
void GLRLM::calcRunLengthNonUniformity() {
	
	runLengthNonUniformity = 0;

	if (totalSum != 0) {
		for (int i = 0; i < rowSums.size(); i++) {
			runLengthNonUniformity += pow(rowSums[i], 2);
		}
		runLengthNonUniformity /= totalSum;
	}
	
}
void GLRLM::calcRunLengthNonUniformityNorm() {

	runLengthNonUniformityNorm = 0;

	if (totalSum != 0) {
		for (int i = 0; i < rowSums.size(); i++) {
			runLengthNonUniformityNorm += pow(rowSums[i], 2);
		}
		runLengthNonUniformityNorm /= pow(totalSum, 2);
	}

}
void GLRLM::calcRunPercentage() {

	if (nPixelsInROI != 0) {
		runPercentage = totalSum / nPixelsInROI;
	}
	else {
		runPercentage = 0;
	}
	
}
void GLRLM::calcGreyLevelVar(vector<vector<float>> probMatrix) {

	greyLevelVar = 0;

	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			if (!isnan(pow((diffGreyLevels[i] - meanGrey), 2)*probMatrix[i][j])) {
				greyLevelVar += pow((diffGreyLevels[i] - meanGrey), 2)*probMatrix[i][j];
			}
		}
	}
}
void GLRLM::calcRunLengthVar(vector<vector<float>> probMatrix) {

	runLengthVar = 0;

	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			runLengthVar += pow((j - meanRun), 2)*probMatrix[i][j];
		}
	}
}
void GLRLM::calcRunEntropy(vector<vector<float>> probMatrix) {

	runEntropy = 0;

	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			if (probMatrix[i][j] > 0) {
				runEntropy -= probMatrix[i][j] * log2(probMatrix[i][j]);
			}
		}
	}
}

void GLRLM::calcFeature(int FEATURE_IDX, vector<float> &temp1DirVals1DVec, vector<vector<float>> GLRLMatrix, vector<vector<float>> probMatrix) {

	switch (FEATURE_IDX)
	{
	case SRE:
		calcShortRunEmph();
		temp1DirVals1DVec.push_back(shortRunEmph);
		break;

	case LRE:
		calcLongRunEmph();
		temp1DirVals1DVec.push_back(longRunEmph);
		break;

	case LGRE:
		calcLowGreyRunEmph();
		temp1DirVals1DVec.push_back(lowGreyRunEmph);
		break;

	case HGRE:
		calcHighGreyRunEmph();
		temp1DirVals1DVec.push_back(highGreyRunEmph);
		break; 

	case SRLE:
		calcShortRunLowEmph(GLRLMatrix);
		temp1DirVals1DVec.push_back(shortRunLowEmph);
		break;

	case SRHE:
		calcShortRunHighEmph(GLRLMatrix);
		temp1DirVals1DVec.push_back(shortRunHighEmph);
		break;

	case LRLE:
		calcLongRunLowEmph(GLRLMatrix);
		temp1DirVals1DVec.push_back(longRunLowEmph);
		break;

	case LRHE:
		calcLongRunHighEmph(GLRLMatrix);
		temp1DirVals1DVec.push_back(longRunHighEmph);
		break;

	case GNU:
		calcGreyNonUnimformity();
		temp1DirVals1DVec.push_back(greyNonUnimformity);
		break;

	case GNUN:
		calcGreyNonUnimformityNorm();
		temp1DirVals1DVec.push_back(greyNonUnimformityNorm);
		break;

	case RLNU:
		calcRunLengthNonUniformity();
		temp1DirVals1DVec.push_back(runLengthNonUniformity);
		break;

	case RLNUN:
		calcRunLengthNonUniformityNorm();
		temp1DirVals1DVec.push_back(runLengthNonUniformityNorm);
		break;

	case RP:
		calcRunPercentage();
		temp1DirVals1DVec.push_back(runPercentage);
		break;

	case GLV:
		calcGreyLevelVar(probMatrix);
		temp1DirVals1DVec.push_back(greyLevelVar);
		break;

	case RLV:
		calcRunLengthVar(probMatrix);
		temp1DirVals1DVec.push_back(runLengthVar);
		break;

	case RE:
		calcRunEntropy(probMatrix);
		temp1DirVals1DVec.push_back(runEntropy);
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
	vector1DofOriPixels = get1DVectorOfPixels(psImage, pucMask);  // �����̽����� �ʱ�ȭ, nPixelsInROI ����
	vector2DofDiscretizedPixels = isFBN? get2DVectorOfDiscretizedPixels_FBN(psImage, pucMask) : get2DVectorOfDiscretizedPixels_FBS(psImage, pucMask); // �����̽����� �ʱ�ȭ, calc maxIntensity
	
	vector<vector<float>> temp4DirVals2DVec;	// �����̽����� �ʱ�ȭ (for. 4���� 1d vec ��ճ���)
	vector<float> tempValues1DVec;				// �����̽����� �ʱ�ȭ (for. final2DVec�� ����)

	// calculate checked feature (4 directions)
	int ang;
	sizeMatrix = maxIntensity; // maxIntensity�� clearVariable()�� ���� �Ź� �ʱ�ȭ
	maxRunLength = max(nHeight, nWidth);
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
		
		vector<vector<float>> probMatrix(sizeMatrix, vector<float>(maxRunLength, 0));
		fill2DprobMatrix(GLRLMatrix, probMatrix);

		meanGrey = getMeanProbGrey(probMatrix);
		meanRun = getMeanProbRun(probMatrix);

		// calculate feature for each dir
		for (int i = 0; i < FEATURE_COUNT; i++) {
			if (isCheckedFeature[i]) calcFeature(i, temp1DirVals1DVec, GLRLMatrix, probMatrix);
		}
		temp4DirVals2DVec.push_back(temp1DirVals1DVec); // 4��
	}

	average4DirValues(temp4DirVals2DVec, tempValues1DVec); // 4���� ��� Ư¡��

	final2DVec.push_back(tempValues1DVec); // ��� ROI �����̽� ���� ������ ���� (�ø���� �ʱ�ȭ)
	
}

void GLRLM::averageAllValues() {

	// get final mean vector
	if (!final2DVec.empty()) {
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
}

void GLRLM::defineFeatureNames(vector<string> &features) {
	/*
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
	*/
	features[SRE] = "SRE";
	features[LRE] = "LRE";
	features[LGRE] = "LGRE";
	features[HGRE] = "HGRE";
	features[SRLE] = "SRLE";
	features[SRHE] = "SRHE";
	features[LRLE] = "LRLE";
	features[LRHE] = "LRHE";
	features[GNU] = "GNU";
	features[GNUN] = "GNUN";
	features[RLNU] = "RLNU";
	features[RLNUN] = "RLNUN";
	features[RP] = "RP";
	features[GLV] = "GLV";
	features[RLV] = "RLV";
	features[RE] = "RE";
}

