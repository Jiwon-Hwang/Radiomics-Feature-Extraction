#include "NGLDM.h"

using namespace std;

NGLDM::NGLDM() {

}
NGLDM::~NGLDM() {

}
void NGLDM::clearVariable() {
	// 슬라이스마다 초기화
	vector1DofOriPixelsInROI.clear();
	vector<short>().swap(vector1DofOriPixelsInROI); // size & capacity 모두 0으로 초기화
	rowSums.clear();
	vector<float>().swap(rowSums);
	colSums.clear();
	vector<float>().swap(colSums);

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
	energy = NAN;

}
void NGLDM::clearVector() {

	if (!final2DVec.empty()) {
		final2DVec.clear();
		vector<vector<float>>().swap(final2DVec); // size & capacity 모두 0으로 초기화
	}

	if (!final1DVec.empty()) {
		final1DVec.clear();
		vector<float>().swap(final1DVec);
	}

}

vector<short> NGLDM::get1DVectorOfPixels(short* psImage, unsigned char* pucMask) {
	
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
	nPixelsInROI = vector1DofOriPixelsInROI.size();

	return vector1DofOriPixels;
}
vector<vector<unsigned short>> NGLDM::get2DVectorOfDiscretizedPixels_FBN(short* psImage, unsigned char* pucMask) {

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

	// clear diffGreyLevels (슬라이스마다 초기화)
	diffGreyLevels.clear();
	vector<unsigned short>().swap(diffGreyLevels);

	//do rounding(ceil), type casting(unsigned short) and asigning to 2D Vec (with ROI)
	vector<vector<unsigned short>> vector2DofDiscretizedPixels(nHeight, vector<unsigned short>(nWidth, 0)); // 양자화한 최종 결과 픽셀값들 담을 벡터
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			unsigned short imageValue_discretized = (unsigned short)ceil(tempFloatVec[index]); // psImage (ori) -> tempFloatVec (discretized)

			// ROI 밖(0)과 안(discretized) 픽셀값들 2D Vec에 할당
			if (maskValue >(unsigned char)0) {
				vector2DofDiscretizedPixels[row][col] = imageValue_discretized;
				// diffGreyLevels에 담기
				diffGreyLevels.push_back(imageValue_discretized);
			}
			else {
				vector2DofDiscretizedPixels[row][col] = 0;
			}
		}
	}

	// get diffGreyLevels (vector containing the different grey levels of the matrix -> extract every element only once)
	sort(diffGreyLevels.begin(), diffGreyLevels.end());
	diffGreyLevels.erase(unique(diffGreyLevels.begin(), diffGreyLevels.end()), diffGreyLevels.end()); // 중복 제거

	return vector2DofDiscretizedPixels;
}
vector<vector<unsigned short>> NGLDM::get2DVectorOfDiscretizedPixels_FBS(short* psImage, unsigned char* pucMask) {

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

	// clear diffGreyLevels (슬라이스마다 초기화)
	diffGreyLevels.clear();
	vector<unsigned short>().swap(diffGreyLevels);

	//do rounding(floor) +1, type casting(unsigned short) and asigning to 2D Vec (with ROI)
	vector<vector<unsigned short>> vector2DofDiscretizedPixels(nHeight, vector<unsigned short>(nWidth, 0));
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			unsigned short imageValue_discretized = (unsigned short)floor(tempFloatVec[index]) + 1;

			// ROI 밖(0)과 안(discretized) 픽셀값들 2D Vec에 할당
			if (maskValue > (unsigned char)0) {
				vector2DofDiscretizedPixels[row][col] = imageValue_discretized;
				// diffGreyLevels에 담기
				diffGreyLevels.push_back(imageValue_discretized);
			}
			else {
				vector2DofDiscretizedPixels[row][col] = 0;
			}
		}
	}

	// get diffGreyLevels (vector containing the different grey levels of the matrix -> extract every element only once)
	sort(diffGreyLevels.begin(), diffGreyLevels.end());
	diffGreyLevels.erase(unique(diffGreyLevels.begin(), diffGreyLevels.end()), diffGreyLevels.end()); 

	return vector2DofDiscretizedPixels;
}
int NGLDM::findIndex(vector<unsigned short> diffGreyLevels, int size, unsigned short target) {
	int i = 0;
	while ((i < size) && (diffGreyLevels[i] != target)) i++; 
	return (i < size) ? (i) : (-1);
}
int NGLDM::getNeighborGreyLevels(vector<vector<unsigned short>> vector2DofDiscretizedPixels, vector<int> actualIndex) {

	int actualX = actualIndex[0];
	int actualY = actualIndex[1];
	int ngldmnr = 0;
	unsigned short tempElement;
	int dist = 1; // 임의로 지정 (distance = 1이면, 8인접 => Chebyshev distance)
	//get the actual grey value
	unsigned short actualElement = vector2DofDiscretizedPixels[actualX][actualY];
	//leftBorder
	int leftBorderY = actualY - dist;
	if (leftBorderY<0) {
		leftBorderY = 0;
	}
	//go from the left to the right border
	int leftBorderX;
	while (leftBorderY < 1 + actualY + dist && leftBorderY < nWidth) {
		leftBorderX = actualX - dist;
		if (leftBorderX<0) {
			leftBorderX = 0;
		}
		while (leftBorderX < 1 + actualX + dist && leftBorderX < nHeight) {
			tempElement = vector2DofDiscretizedPixels[leftBorderX][leftBorderY];
			if (actualElement == tempElement) {
				ngldmnr += 1;
			}
			leftBorderX += 1;
		}
		leftBorderY += 1;
	}
	//because with this method I also counted the actual Element, I have to subtract 1
	ngldmnr -= 1;

	return ngldmnr;
}
void NGLDM::fill2DNGLDMatrix(vector<vector<unsigned short>> vector2DofDiscretizedPixels, vector<vector<float>> &NGLDMatrix) {

	// vector2DofDiscretizedPixels : 멤버변수 copy (매개변수로 새 벡터 할당)
		
	vector<int> actualIndex;
	unsigned short actualElement;
	//here the position of the grey level in the different grey level-array will be stored
	int actualGreyIndex;
	int ngldmnr;
	//check every element of the ROI
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			actualElement = vector2DofDiscretizedPixels[row][col];
			//if the actual matrix element is not 0(NAN) (=is in ROI)
			if (actualElement) {
				actualGreyIndex = findIndex(diffGreyLevels, sizeMatrix, actualElement);
				actualIndex.push_back(row);
				actualIndex.push_back(col);
				ngldmnr = getNeighborGreyLevels(vector2DofDiscretizedPixels, actualIndex);
				actualIndex.clear();
				//update NGLDM
				NGLDMatrix[actualGreyIndex][ngldmnr] += 1;
			}
		}
	}

}
void NGLDM::fill2DprobMatrix(vector<vector<float>> NGLDMatrix, vector<vector<float>> &probMatrix) {
	
	probMatrix = NGLDMatrix; // 깊은 복사
	for (int i = 0; i < probMatrix.size(); i++) {
		transform(probMatrix[i].begin(), probMatrix[i].end(), probMatrix[i].begin(), bind2nd(divides<float>(), int(totalSum)));
	}

}

// common calculation functions
float NGLDM::getTotalSum(vector<vector<float>> matrix) {
	// calculate the sum of all matrix elements
	float sum = 0;
	for (int i = 0; i<matrix.size(); i++) {
		for (int j = 0; j<matrix[0].size(); j++) {
			sum += matrix[i][j];
		}
	}

	return sum;
}
vector<float> NGLDM::getRowSums(vector<vector<float>> matrix) {
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
vector<float> NGLDM::getColSums(vector<vector<float>> matrix) {
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
float NGLDM::getMeanProbGrey(vector<vector<float>> probMatrix) {
	// calculate the mean probability of the appearance of every grey level
	float mean = 0;
	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			mean += diffGreyLevels[i] * probMatrix[i][j];
		}
	}

	return mean;
}
float NGLDM::getMeanProbRun(vector<vector<float>> probMatrix) {
	// calculate the mean probability of the runlength
	float mean = 0;
	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			mean += j * probMatrix[i][j];
		}
	}

	return mean;
}

void NGLDM::calcShortRunEmph() {
	
	shortRunEmph = 0;

	if (totalSum != 0) {
		for (int j = 0; j < rowSums.size(); j++) {
			shortRunEmph += rowSums[j] / pow(j + 1, 2);
		}
		shortRunEmph /= totalSum;
	}
	
}
void NGLDM::calcLongRunEmph() {

	longRunEmph = 0;

	if (totalSum != 0) {
		for (int j = 0; j < rowSums.size(); j++) {
			longRunEmph += rowSums[j] * pow(j + 1, 2);
		}
		longRunEmph /= totalSum;
	}

}
void NGLDM::calcLowGreyRunEmph() {

	lowGreyRunEmph = 0;

	if (totalSum != 0) {
		for (int i = 0; i < colSums.size(); i++) {
			lowGreyRunEmph += colSums[i] / pow(i + 1, 2);
		}
		lowGreyRunEmph /= totalSum;
	}

}
void NGLDM::calcHighGreyRunEmph() {
	
	highGreyRunEmph = 0;

	if (totalSum != 0) {
		for (int i = 0; i < colSums.size(); i++) {
			highGreyRunEmph += colSums[i] * pow(i + 1, 2);
		}
		highGreyRunEmph /= totalSum;
	}

}
void NGLDM::calcShortRunLowEmph(vector<vector<float>> NGLDMatrix) {

	shortRunLowEmph = 0;

	if (totalSum != 0) {
		for (int row = 0; row < NGLDMatrix.size(); row++) {
			for (int col = 1; col < NGLDMatrix[0].size() + 1; col++) {
				shortRunLowEmph += NGLDMatrix[row][col - 1] / (pow(row+1, 2) * pow(col, 2));
			}
		}
		shortRunLowEmph /= totalSum;
	}

}
void NGLDM::calcShortRunHighEmph(vector<vector<float>> NGLDMatrix) {

	shortRunHighEmph = 0;

	if (totalSum != 0) {
		for (int row = 0; row < NGLDMatrix.size(); row++) {
			for (int col = 1; col < NGLDMatrix[0].size() + 1; col++) {
				shortRunHighEmph += pow(row+1, 2) * NGLDMatrix[row][col - 1] / pow(col, 2);
			}
		}
		shortRunHighEmph /= totalSum;
	}

}
void NGLDM::calcLongRunLowEmph(vector<vector<float>> NGLDMatrix) {

	longRunLowEmph = 0;

	if (totalSum != 0) {
		for (int row = 0; row < NGLDMatrix.size(); row++) {
			for (int col = 1; col < NGLDMatrix[0].size() + 1; col++) {
				longRunLowEmph += pow(col, 2) * NGLDMatrix[row][col - 1] / pow(row+1, 2);
			}
		}
		longRunLowEmph /= totalSum;
	}

}
void NGLDM::calcLongRunHighEmph(vector<vector<float>> NGLDMatrix) {

	longRunHighEmph = 0;

	if (totalSum != 0) {
		for (int row = 0; row < NGLDMatrix.size(); row++) {
			for (int col = 1; col < NGLDMatrix[0].size() + 1; col++) {
				longRunHighEmph += pow(col, 2) * pow(row+1, 2) * NGLDMatrix[row][col - 1];
			}
		}
		longRunHighEmph /= totalSum;
	}

}
void NGLDM::calcGreyNonUnimformity() {

	greyNonUnimformity = 0;

	if (totalSum != 0) {
		for (int i = 0; i < colSums.size(); i++) {
			greyNonUnimformity += pow(colSums[i], 2);
		}
		greyNonUnimformity /= totalSum;
	}

}
void NGLDM::calcGreyNonUnimformityNorm() {

	greyNonUnimformityNorm = 0;

	if (totalSum != 0) {
		for (int i = 0; i < colSums.size(); i++) {
			greyNonUnimformityNorm += pow(colSums[i], 2);
		}
		greyNonUnimformityNorm /= pow(totalSum, 2);
	}

}
void NGLDM::calcRunLengthNonUniformity() {
	
	runLengthNonUniformity = 0;

	if (totalSum != 0) {
		for (int i = 0; i < rowSums.size(); i++) {
			runLengthNonUniformity += pow(rowSums[i], 2);
		}
		runLengthNonUniformity /= totalSum;
	}
	
}
void NGLDM::calcRunLengthNonUniformityNorm() {

	runLengthNonUniformityNorm = 0;

	if (totalSum != 0) {
		for (int i = 0; i < rowSums.size(); i++) {
			runLengthNonUniformityNorm += pow(rowSums[i], 2);
		}
		runLengthNonUniformityNorm /= pow(totalSum, 2);
	}

}
void NGLDM::calcRunPercentage() {

	if (nPixelsInROI != 0) {
		runPercentage = totalSum / nPixelsInROI;
	}
	else {
		runPercentage = 0;
	}
	
}
void NGLDM::calcGreyLevelVar(vector<vector<float>> probMatrix) {

	greyLevelVar = 0;

	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			if (!isnan(pow((diffGreyLevels[i] - meanGrey), 2)*probMatrix[i][j])) {
				greyLevelVar += pow((diffGreyLevels[i] - meanGrey), 2)*probMatrix[i][j];
			}
		}
	}
}
void NGLDM::calcRunLengthVar(vector<vector<float>> probMatrix) {

	runLengthVar = 0;

	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			runLengthVar += pow((j - meanRun), 2)*probMatrix[i][j];
		}
	}
}
void NGLDM::calcRunEntropy(vector<vector<float>> probMatrix) {

	runEntropy = 0;

	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			if (probMatrix[i][j] > 0) {
				runEntropy -= probMatrix[i][j] * log2(probMatrix[i][j]);
			}
		}
	}
}
void NGLDM::calcEnergy(vector<vector<float>> probMatrix) {

	energy = 0;

	for (int row = 0; row<probMatrix.size(); row++) {
		for (int col = 0; col<probMatrix[0].size(); col++) {
			energy += pow(probMatrix[row][col], 2);
		}
	}
}

void NGLDM::calcFeature(int FEATURE_IDX, vector<float> &tempValues1DVec, vector<vector<float>> NGLDMatrix, vector<vector<float>> probMatrix) {

	switch (FEATURE_IDX)
	{
	case SRE:
		calcShortRunEmph();
		tempValues1DVec.push_back(shortRunEmph);
		break;

	case LRE:
		calcLongRunEmph();
		tempValues1DVec.push_back(longRunEmph);
		break;

	case LGRE:
		calcLowGreyRunEmph();
		tempValues1DVec.push_back(lowGreyRunEmph);
		break;

	case HGRE:
		calcHighGreyRunEmph();
		tempValues1DVec.push_back(highGreyRunEmph);
		break; 

	case SRLE:
		calcShortRunLowEmph(NGLDMatrix);
		tempValues1DVec.push_back(shortRunLowEmph);
		break;

	case SRHE:
		calcShortRunHighEmph(NGLDMatrix);
		tempValues1DVec.push_back(shortRunHighEmph);
		break;

	case LRLE:
		calcLongRunLowEmph(NGLDMatrix);
		tempValues1DVec.push_back(longRunLowEmph);
		break;

	case LRHE:
		calcLongRunHighEmph(NGLDMatrix);
		tempValues1DVec.push_back(longRunHighEmph);
		break;

	case GNU:
		calcGreyNonUnimformity();
		tempValues1DVec.push_back(greyNonUnimformity);
		break;

	case GNUN:
		calcGreyNonUnimformityNorm();
		tempValues1DVec.push_back(greyNonUnimformityNorm);
		break;

	case RLNU:
		calcRunLengthNonUniformity();
		tempValues1DVec.push_back(runLengthNonUniformity);
		break;

	case RLNUN:
		calcRunLengthNonUniformityNorm();
		tempValues1DVec.push_back(runLengthNonUniformityNorm);
		break;

	case RP:
		calcRunPercentage();
		tempValues1DVec.push_back(runPercentage);
		break;

	case GLV:
		calcGreyLevelVar(probMatrix);
		tempValues1DVec.push_back(greyLevelVar);
		break;

	case RLV:
		calcRunLengthVar(probMatrix);
		tempValues1DVec.push_back(runLengthVar);
		break;

	case RE:
		calcRunEntropy(probMatrix);
		tempValues1DVec.push_back(runEntropy);
		break;

	case ENERGY:
		calcEnergy(probMatrix);
		tempValues1DVec.push_back(energy);
		break;

	default:
		break;
	}
}
void NGLDM::featureExtraction(short* psImage, unsigned char* pucMask, int nHeight_, int nWidth_) {

	// claer all values
	clearVariable(); // 슬라이스마다 초기화

	// get discretized 2D input matrix (2d vector)
	nHeight = nHeight_;
	nWidth = nWidth_;
	vector1DofOriPixels = get1DVectorOfPixels(psImage, pucMask);  // 슬라이스마다 초기화, nPixelsInROI 산출
	vector2DofDiscretizedPixels = isFBN? get2DVectorOfDiscretizedPixels_FBN(psImage, pucMask) : get2DVectorOfDiscretizedPixels_FBS(psImage, pucMask); // 슬라이스마다 초기화
	
	// calculate checked feature
	sizeMatrix = diffGreyLevels.size();		// ***NGLDM에서는 Matrix의 row 크기인 sizeMatrix가 maxIntensity가 아닌 diffGreyLevels의 size가 정확!!***
		
	// calculate NGLDM matrix
	vector<vector<float>> NGLDMatrix(sizeMatrix, vector<float>(9, 0));
	fill2DNGLDMatrix(vector2DofDiscretizedPixels, NGLDMatrix); 

	totalSum = getTotalSum(NGLDMatrix);
	rowSums = getRowSums(NGLDMatrix);
	colSums = getColSums(NGLDMatrix);
		
	vector<vector<float>> probMatrix(sizeMatrix, vector<float>(9, 0));
	fill2DprobMatrix(NGLDMatrix, probMatrix);

	meanGrey = getMeanProbGrey(probMatrix);
	meanRun = getMeanProbRun(probMatrix);

	vector<float> tempValues1DVec;			// 슬라이스마다 초기화 (for. final2DVec에 누적)
	
	// calculate feature
	for (int i = 0; i < FEATURE_COUNT; i++) {
		if (isCheckedFeature[i]) calcFeature(i, tempValues1DVec, NGLDMatrix, probMatrix);
	}

	final2DVec.push_back(tempValues1DVec);	// 모든 ROI 슬라이스 들어올 때까지 누적 (시리즈마다 초기화)
	
}

void NGLDM::averageAllValues() {

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

void NGLDM::defineFeatureNames(vector<string> &features) {
	features[SRE] = "LDE";
	features[LRE] = "HDE";
	features[LGRE] = "LGCE";
	features[HGRE] = "HGCE";
	features[SRLE] = "LDLE";
	features[SRHE] = "LDHE";
	features[LRLE] = "HDLE";
	features[LRHE] = "HDHE";
	features[GNU] = "GNU";
	features[GNUN] = "GNUN";
	features[RLNU] = "DCNU";
	features[RLNUN] = "DCNUN";
	features[RP] = "DCPercentage";
	features[GLV] = "GLV";
	features[RLV] = "DCV";
	features[RE] = "DCEntropy";
	features[ENERGY] = "DCEnergy";
}

