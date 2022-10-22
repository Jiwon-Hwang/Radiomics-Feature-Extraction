#include "GLSZM.h"

using namespace std;

GLSZM::GLSZM() {

}
GLSZM::~GLSZM() {

}
void GLSZM::clearVariable() {
	// 슬라이스마다 초기화
	vector1DofOriPixelsInROI.clear();
	vector<short>().swap(vector1DofOriPixelsInROI); // size & capacity 모두 0으로 초기화
	rowSums.clear();
	vector<float>().swap(rowSums);
	colSums.clear();
	vector<float>().swap(colSums);
	vector1DofOriPixels.clear();
	vector<short>().swap(vector1DofOriPixels);
	vector2DofDiscretizedPixels.clear();
	vector<vector<unsigned short>>().swap(vector2DofDiscretizedPixels);

	maxIntensity = -1;

	smallZoneEmph = NAN;
	largeZoneEmph = NAN;
	lowGreyZoneEmph = NAN;
	highGreyZoneEmph = NAN;
	smallZoneLowGreyEmph = NAN;
	smallZoneHighGreyEmph = NAN;
	largeZoneLowGreyEmph = NAN;
	largeZoneHighGreyEmph = NAN;
	greyNonUniformity = NAN;
	greyNonUniformityNorm = NAN;
	zoneSizeNonUniformity = NAN;
	zoneSizeNonUniformityNorm = NAN;
	zonePercentage = NAN;
	greyLevelVar = NAN;
	zoneSizeVar = NAN;
	zoneSizeEntropy = NAN;

}
void GLSZM::clearVector() {

	if (!final2DVec.empty()) {
		final2DVec.clear();
		vector<vector<float>>().swap(final2DVec); // size & capacity 모두 0으로 초기화
	}

	if (!final1DVec.empty()) {
		final1DVec.clear();
		vector<float>().swap(final1DVec);
	}

}

vector<short> GLSZM::get1DVectorOfPixels(short* psImage, unsigned char* pucMask) {
	
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
vector<vector<unsigned short>> GLSZM::get2DVectorOfDiscretizedPixels_FBN(short* psImage, unsigned char* pucMask) {

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

	// maxIntensity(최댓값) 구하기 => for. sizeMatrix (FBN의 경우엔 nBins)
	maxIntensity = nBins;

	// get diffGreyLevels (vector containing the different grey levels of the matrix -> extract every element only once)
	sort(diffGreyLevels.begin(), diffGreyLevels.end());
	diffGreyLevels.erase(unique(diffGreyLevels.begin(), diffGreyLevels.end()), diffGreyLevels.end()); // 중복 제거

	return vector2DofDiscretizedPixels;
}
vector<vector<unsigned short>> GLSZM::get2DVectorOfDiscretizedPixels_FBS(short* psImage, unsigned char* pucMask) {

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

	/// clear diffGreyLevels (슬라이스마다 초기화)
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
			if (maskValue >(unsigned char)0) {
				vector2DofDiscretizedPixels[row][col] = imageValue_discretized;
				// diffGreyLevels에 담기
				diffGreyLevels.push_back(imageValue_discretized);
			}
			else {
				vector2DofDiscretizedPixels[row][col] = 0;
			}

			// maxIntensity(최댓값) 구하기 => for. sizeMatrix
			maxIntensity = maxIntensity < (int)vector2DofDiscretizedPixels[row][col] ? (int)vector2DofDiscretizedPixels[row][col] : maxIntensity;
		}
	}

	// get diffGreyLevels (vector containing the different grey levels of the matrix -> extract every element only once)
	sort(diffGreyLevels.begin(), diffGreyLevels.end());
	diffGreyLevels.erase(unique(diffGreyLevels.begin(), diffGreyLevels.end()), diffGreyLevels.end()); // 중복 제거

	return vector2DofDiscretizedPixels;
}
int GLSZM::findIndex(vector<unsigned short> diffGreyLevels, int size, unsigned short target) {
	int i = 0;
	while ((i < size) && (diffGreyLevels[i] != target)) i++;
	return (i < size) ? (i) : (-1);
}
void GLSZM::get360XYDirections(int &directionX, int &directionY, int angle) {

	//if we only go in the depth (현재는 X)
	if (angle == 360) {
		directionX = 0;
		directionY = 1;
	}
	//if angle is 180 degrees, only look in x direction
	else if (angle == 315) {
		directionX = 1;
		directionY = 1;
	}
	//if angle is 90 degrees only look in y direction
	else if (angle == 270) {
		directionX = 1;
		directionY = 0;
	}
	//if angle is in 45 degrees direction look in x and y direction
	else if (angle == 225) {
		directionX = 1;
		directionY = -1;
	}
	else if (angle == 180) {
		directionX = 0;
		directionY = -1;
	}
	else if (angle == 135) {
		directionX = -1;
		directionY = -1;
	}
	else if (angle == 90) {
		directionX = -1;
		directionY = 0;
	}
	else if (angle == 45) {
		directionX = -1;
		directionY = 1;
	}
	else if (angle == 0) {
		directionX = 0;
		directionY = 0;
	}
	else {
		std::cout << " incorrect angle" << std::endl;
	}

}
void GLSZM::getNeighbours(vector<vector<unsigned short>> &vector2DofDiscretizedPixels, unsigned short actElement, vector<vector<int>> &matrixIndices) {

	vector<int> actIndex;
	vector<int> newIndex;
	vector<int> tempIndex;
	int neighborDist = 1;
	int directionX;
	int directionY;
	vector<vector<int>> tempMatrixIndices = matrixIndices;
	int ang;

	//look at all indices which are saved in the vector matrix indices
	while (tempMatrixIndices.size() > 0) {
		//get the last element of the vector and delete it
		actIndex = tempMatrixIndices.back();
		tempMatrixIndices.pop_back();
		tempIndex = actIndex;
		for (int i = 0; i < 8; i++) { // 8인접 연결요소 체크
			ang = 360 - i * 45;
			get360XYDirections(directionX, directionY, ang);
			actIndex = tempIndex;
			if (actIndex[0] + directionX>-1 && actIndex[0] + directionX<nHeight && actIndex[1] + directionY>-1 && actIndex[1] + directionY<nWidth && actElement == vector2DofDiscretizedPixels[actIndex[0] + directionX][actIndex[1] + directionY]) {
				vector2DofDiscretizedPixels[actIndex[0] + directionX][actIndex[1] + directionY] = NAN;
				actIndex[0] += directionX;
				actIndex[1] += directionY;
				tempMatrixIndices.push_back(actIndex);
				matrixIndices.push_back(actIndex);
			}
		}
	}
}
void GLSZM::fill2DGLSZMatrix(vector<vector<unsigned short>> vector2DofDiscretizedPixels, vector<vector<float>> &GLSZMatrix) {

	// vector2DofDiscretizedPixels : 멤버변수 copy (매개변수로 새 벡터 할당 후 값 조작)

	unsigned short actualElement;
	vector<vector<int>> matrixIndices;
	vector<int> actualIndex;
	int actualGreyIndex;

	//do this grey level by grey level
	maxZoneSize = 0; // 갱신 (가능한 최대 영역 넓이로)
	int tempZoneSize = 0;

	//go element by element through the image
	//look at the neighbors of every element and check if they have the same grey value
	//set every element already seen to NAN
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			actualElement = vector2DofDiscretizedPixels[row][col];
			if (!isnan((float)actualElement)) {
				actualGreyIndex = findIndex(diffGreyLevels, diffGreyLevels.size(), actualElement);
				vector2DofDiscretizedPixels[row][col] = NAN;
				actualIndex.push_back(row);
				actualIndex.push_back(col);
				matrixIndices.push_back(actualIndex);
				actualIndex.clear();
				getNeighbours(vector2DofDiscretizedPixels, actualElement, matrixIndices);
			}
			tempZoneSize = matrixIndices.size();
			if (tempZoneSize > maxZoneSize) {
				maxZoneSize = tempZoneSize;
			}
			if (matrixIndices.size() > 0) {
				GLSZMatrix[actualGreyIndex][matrixIndices.size() - 1] += 1;
				matrixIndices.clear();
			}
		}
	}
	

}
void GLSZM::fill2DprobMatrix(vector<vector<float>> GLSZMatrix, vector<vector<float>> &probMatrix) {
	
	probMatrix = GLSZMatrix; // 깊은 복사
	
	for (int i = 0; i < probMatrix.size(); i++) {
		transform(probMatrix[i].begin(), probMatrix[i].end(), probMatrix[i].begin(), bind2nd(divides<float>(), int(totalSum)));
	}

}

// common calculation functions
float GLSZM::getTotalSum(vector<vector<float>> matrix) {
	// calculate the sum of all matrix elements
	float sum = 0;
	for (int i = 0; i<matrix.size(); i++) {
		for (int j = 0; j<matrix[0].size(); j++) {
			sum += matrix[i][j];
		}
	}

	return sum;
}
vector<float> GLSZM::getRowSums(vector<vector<float>> matrix) {
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
vector<float> GLSZM::getColSums(vector<vector<float>> matrix) {
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
float GLSZM::getMeanProbGrey(vector<vector<float>> probMatrix) {
	// calculate the mean probability of the appearance of every grey level
	float mean = 0;
	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			mean += (i+1) * probMatrix[i][j];
		}
	}

	return mean;
}
float GLSZM::getMeanProbRun(vector<vector<float>> probMatrix) {
	// calculate the mean probability of the runlength
	float mean = 0;
	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			mean += j * probMatrix[i][j];
		}
	}

	return mean;
}

// GLRLM과 동일 => 추후 상속으로 수정
void GLSZM::calcSmallZoneEmph() { 
	
	smallZoneEmph = 0;

	if (totalSum != 0) {
		for (int j = 0; j < rowSums.size(); j++) {
			smallZoneEmph += rowSums[j] / pow(j + 1, 2);
		}
		smallZoneEmph /= totalSum;
	}
	
}
void GLSZM::calcLargeZoneEmph() {

	largeZoneEmph = 0;

	if (totalSum != 0) {
		for (int j = 0; j < rowSums.size(); j++) {
			largeZoneEmph += rowSums[j] * pow(j + 1, 2);
		}
		largeZoneEmph /= totalSum;
	}

}
void GLSZM::calcLowGreyZoneEmph() {

	lowGreyZoneEmph = 0;

	if (totalSum != 0) {
		for (int i = 0; i < colSums.size(); i++) {
			lowGreyZoneEmph += colSums[i] / pow(i + 1, 2);
		}
		lowGreyZoneEmph /= totalSum;
	}

}
void GLSZM::calcHighGreyZoneEmph() {
	
	highGreyZoneEmph = 0;

	if (totalSum != 0) {
		for (int i = 0; i < colSums.size(); i++) {
			highGreyZoneEmph += colSums[i] * pow(i + 1, 2);
		}
		highGreyZoneEmph /= totalSum;
	}

}
void GLSZM::calcSmallZoneLowGreyEmph(vector<vector<float>> GLSZMatrix) {

	smallZoneLowGreyEmph = 0;

	if (totalSum != 0) {
		for (int row = 0; row < GLSZMatrix.size(); row++) {
			for (int col = 1; col < GLSZMatrix[0].size() + 1; col++) {
				smallZoneLowGreyEmph += GLSZMatrix[row][col - 1] / (pow(row+1, 2) * pow(col, 2));
			}
		}
		smallZoneLowGreyEmph /= totalSum;
	}

}
void GLSZM::calcSmallZoneHighGreyEmph(vector<vector<float>> GLSZMatrix) {

	smallZoneHighGreyEmph = 0;

	if (totalSum != 0) {
		for (int row = 0; row < GLSZMatrix.size(); row++) {
			for (int col = 1; col < GLSZMatrix[0].size() + 1; col++) {
				smallZoneHighGreyEmph += pow(row+1, 2) * GLSZMatrix[row][col - 1] / pow(col, 2);
			}
		}
		smallZoneHighGreyEmph /= totalSum;
	}

}
void GLSZM::calcLargeZoneLowGreyEmph(vector<vector<float>> GLSZMatrix) {

	largeZoneLowGreyEmph = 0;

	if (totalSum != 0) {
		for (int row = 0; row < GLSZMatrix.size(); row++) {
			for (int col = 1; col < GLSZMatrix[0].size() + 1; col++) {
				largeZoneLowGreyEmph += pow(col, 2) * GLSZMatrix[row][col - 1] / pow(row+1, 2);
			}
		}
		largeZoneLowGreyEmph /= totalSum;
	}

}
void GLSZM::calcLargeZoneHighGreyEmph(vector<vector<float>> GLSZMatrix) {

	largeZoneHighGreyEmph = 0;

	if (totalSum != 0) {
		for (int row = 0; row < GLSZMatrix.size(); row++) {
			for (int col = 1; col < GLSZMatrix[0].size() + 1; col++) {
				largeZoneHighGreyEmph += pow(col, 2) * pow(row+1, 2) * GLSZMatrix[row][col - 1];
			}
		}
		largeZoneHighGreyEmph /= totalSum;
	}

}
void GLSZM::calcGreyNonUniformity() {

	greyNonUniformity = 0;

	if (totalSum != 0) {
		for (int i = 0; i < colSums.size(); i++) {
			greyNonUniformity += pow(colSums[i], 2);
		}
		greyNonUniformity /= totalSum;
	}

}
void GLSZM::calcGreyNonUniformityNorm() {

	greyNonUniformityNorm = 0;

	if (totalSum != 0) {
		for (int i = 0; i < colSums.size(); i++) {
			greyNonUniformityNorm += pow(colSums[i], 2);
		}
		greyNonUniformityNorm /= pow(totalSum, 2);
	}

}
void GLSZM::calcZoneSizeNonUniformity() {
	
	zoneSizeNonUniformity = 0;

	if (totalSum != 0) {
		for (int i = 0; i < rowSums.size(); i++) {
			zoneSizeNonUniformity += pow(rowSums[i], 2);
		}
		zoneSizeNonUniformity /= totalSum;
	}
	
}
void GLSZM::calcZoneSizeNonUniformityNorm() {

	zoneSizeNonUniformityNorm = 0;

	if (totalSum != 0) {
		for (int i = 0; i < rowSums.size(); i++) {
			zoneSizeNonUniformityNorm += pow(rowSums[i], 2);
		}
		zoneSizeNonUniformityNorm /= pow(totalSum, 2);
	}

}
void GLSZM::calcZonePercentage() {

	if (nPixelsInROI != 0) {
		zonePercentage = totalSum / nPixelsInROI;
	}
	else {
		zonePercentage = 0;
	}
	
}
void GLSZM::calcGreyLevelVar(vector<vector<float>> probMatrix) {

	greyLevelVar = 0;

	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			if (!isnan(pow((diffGreyLevels[i] - meanGrey), 2)*probMatrix[i][j])) {
				greyLevelVar += pow((diffGreyLevels[i] - meanGrey), 2)*probMatrix[i][j];
			}
		}
	}
}
void GLSZM::calcZoneSizeVar(vector<vector<float>> probMatrix) {

	zoneSizeVar = 0;

	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			zoneSizeVar += pow((j - meanRun), 2)*probMatrix[i][j];
		}
	}
}
void GLSZM::calcZoneSizeEntropy(vector<vector<float>> probMatrix) {

	zoneSizeEntropy = 0;

	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			if (probMatrix[i][j] > 0) {
				zoneSizeEntropy -= probMatrix[i][j] * log2(probMatrix[i][j]);
			}
		}
	}
}

void GLSZM::calcFeature(int FEATURE_IDX, vector<float> &temp1DirVals1DVec, vector<vector<float>> GLSZMatrix, vector<vector<float>> probMatrix) {

	switch (FEATURE_IDX)
	{
	case SZE:
		calcSmallZoneEmph();
		temp1DirVals1DVec.push_back(smallZoneEmph);
		break;

	case LZE:
		calcLargeZoneEmph();
		temp1DirVals1DVec.push_back(largeZoneEmph);
		break;

	case LGZE:
		calcLowGreyZoneEmph();
		temp1DirVals1DVec.push_back(lowGreyZoneEmph);
		break;

	case HGZE:
		calcHighGreyZoneEmph();
		temp1DirVals1DVec.push_back(highGreyZoneEmph);
		break; 

	case SZLGE:
		calcSmallZoneLowGreyEmph(GLSZMatrix);
		temp1DirVals1DVec.push_back(smallZoneLowGreyEmph);
		break;

	case SZHGE:
		calcSmallZoneHighGreyEmph(GLSZMatrix);
		temp1DirVals1DVec.push_back(smallZoneHighGreyEmph);
		break;

	case LZLGE:
		calcLargeZoneLowGreyEmph(GLSZMatrix);
		temp1DirVals1DVec.push_back(largeZoneLowGreyEmph);
		break;

	case LZHGE:
		calcLargeZoneHighGreyEmph(GLSZMatrix);
		temp1DirVals1DVec.push_back(largeZoneHighGreyEmph);
		break;

	case GNU:
		calcGreyNonUniformity();
		temp1DirVals1DVec.push_back(greyNonUniformity);
		break;

	case GNUN:
		calcGreyNonUniformityNorm();
		temp1DirVals1DVec.push_back(greyNonUniformityNorm);
		break;

	case ZSNU:
		calcZoneSizeNonUniformity();
		temp1DirVals1DVec.push_back(zoneSizeNonUniformity);
		break;

	case ZSNUN:
		calcZoneSizeNonUniformityNorm();
		temp1DirVals1DVec.push_back(zoneSizeNonUniformityNorm);
		break;

	case ZP:
		calcZonePercentage();
		temp1DirVals1DVec.push_back(zonePercentage);
		break;

	case GLV:
		calcGreyLevelVar(probMatrix);
		temp1DirVals1DVec.push_back(greyLevelVar);
		break;

	case ZSV:
		calcZoneSizeVar(probMatrix);
		temp1DirVals1DVec.push_back(zoneSizeVar);
		break;

	case ZSE:
		calcZoneSizeEntropy(probMatrix);
		temp1DirVals1DVec.push_back(zoneSizeEntropy);
		break;

	default:
		break;
	}
}
void GLSZM::featureExtraction(short* psImage, unsigned char* pucMask, int nHeight_, int nWidth_) {

	// claer all values
	clearVariable(); // 슬라이스마다 초기화

	// get discretized 2D input matrix (2d vector)
	nHeight = nHeight_;
	nWidth = nWidth_;
	vector1DofOriPixels = get1DVectorOfPixels(psImage, pucMask);  // 슬라이스마다 초기화, nPixelsInROI 산출
	vector2DofDiscretizedPixels = isFBN? get2DVectorOfDiscretizedPixels_FBN(psImage, pucMask) : get2DVectorOfDiscretizedPixels_FBS(psImage, pucMask); // 슬라이스마다 초기화, calc maxIntensity

	// calculate checked feature
	sizeMatrix = maxIntensity; // maxIntensity만 clearVariable()을 통해 매번 초기화
	maxZoneSize = nHeight * nWidth;
		
	// calculate GLSZM matrix
	vector<vector<float>> GLSZMatrix(sizeMatrix, vector<float>(maxZoneSize, 0));
	fill2DGLSZMatrix(vector2DofDiscretizedPixels, GLSZMatrix); // getNeighbours에서 vec, maxZoneSize 변경됨

	totalSum = getTotalSum(GLSZMatrix);
	rowSums = getRowSums(GLSZMatrix);
	colSums = getColSums(GLSZMatrix);
		
	vector<vector<float>> probMatrix(sizeMatrix, vector<float>(maxZoneSize, 0));
	fill2DprobMatrix(GLSZMatrix, probMatrix);

	meanGrey = getMeanProbGrey(probMatrix);
	meanRun = getMeanProbRun(probMatrix);


	vector<float> tempValues1DVec; // 슬라이스마다 초기화

	// calculate feature for each dir
	for (int i = 0; i < FEATURE_COUNT; i++) {
		if (isCheckedFeature[i]) calcFeature(i, tempValues1DVec, GLSZMatrix, probMatrix);
	}

	final2DVec.push_back(tempValues1DVec); // 모든 ROI 슬라이스 들어올 때까지 누적 (시리즈마다 초기화)
	
}

void GLSZM::averageAllValues() {

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

void GLSZM::defineFeatureNames(vector<string> &features) {
	features[SZE] = "SZE";
	features[LZE] = "LZE";
	features[LGZE] = "LGZE";
	features[HGZE] = "HGZE";
	features[SZLGE] = "SZLGE";
	features[SZHGE] = "SZHGE";
	features[LZLGE] = "LZLGE";
	features[LZHGE] = "LZHGE";
	features[GNU] = "GNU]";
	features[GNUN] = "GNUN";
	features[ZSNU] = "ZSNU";
	features[ZSNUN] = "ZSNUN";
	features[ZP] = "ZP";
	features[GLV] = "GLV";
	features[ZSV] = "ZSV";
	features[ZSE] = "ZSE";
}

