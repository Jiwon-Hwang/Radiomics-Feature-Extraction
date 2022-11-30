#include "GLDZM.h"

using namespace std;

GLDZM::GLDZM() {

}
GLDZM::~GLDZM() {

}
void GLDZM::clearVariable() {
	// 슬라이스마다 초기화
	vector1DofOriPixelsInROI.clear();
	vector<short>().swap(vector1DofOriPixelsInROI); // size & capacity 모두 0으로 초기화
	vector1DofOriPixelsInROI_morph.clear();
	vector<short>().swap(vector1DofOriPixelsInROI_morph); // size & capacity 모두 0으로 초기화
	rowSums.clear();
	vector<float>().swap(rowSums);
	colSums.clear();
	vector<float>().swap(colSums);
	vector1DofOriPixels.clear();
	vector<short>().swap(vector1DofOriPixels);
	vector1DofOriPixels_morph.clear();
	vector<short>().swap(vector1DofOriPixels_morph);
	vector2DofDiscretizedPixels.clear();
	vector<vector<unsigned short>>().swap(vector2DofDiscretizedPixels);
	vector2DofDiscretizedPixels_morph.clear();
	vector<vector<unsigned short>>().swap(vector2DofDiscretizedPixels_morph);

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
void GLDZM::clearVector() {

	if (!final2DVec.empty()) {
		final2DVec.clear();
		vector<vector<float>>().swap(final2DVec); // size & capacity 모두 0으로 초기화
	}

	if (!final1DVec.empty()) {
		final1DVec.clear();
		vector<float>().swap(final1DVec);
	}

}

vector<short> GLDZM::get1DVectorOfPixels(short* psImage, unsigned char* pucMask) {
	
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
vector<short> GLDZM::get1DVectorOfPixels(short* psImage, vector<vector<unsigned char>> pucMask) {

	vector<short> vector1DofOriPixels_morph(nHeight*nWidth, 0); // 0으로 초기화 후, ROI 바깥 영역은 그대로 0. 미리 크기 할당해 두는게 훨씬 안정적
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[row][col];
			short imageValue = psImage[index];

			// ROI 밖(0)과 안 픽셀값들 모두 할당 (in 1d vector => for fast discretising!)
			vector1DofOriPixels_morph[index] = (maskValue >(unsigned char)0) ? imageValue : 0;
			// ROI 내부값 push (ROI가 없으면 계속 pass => for min, max)
			if (maskValue >(unsigned char)0) {
				vector1DofOriPixelsInROI_morph.push_back(imageValue); // 슬라이스마다 초기화 해줘야 함! (clearVariable)
			}
		}
	}

	return vector1DofOriPixels_morph;
}
vector<vector<unsigned short>> GLDZM::get2DVectorOfDiscretizedPixels_FBN(short* psImage, unsigned char* pucMask) {

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
vector<vector<unsigned short>> GLDZM::get2DVectorOfDiscretizedPixels_FBN(short* psImage, vector<vector<unsigned char>> pucMask) {

	// min(front) : -1, max(back) : 180
	float min = (float)*min_element(vector1DofOriPixelsInROI_morph.begin(), vector1DofOriPixelsInROI_morph.end()); // min_element() : pointer return
	float max = (float)*max_element(vector1DofOriPixelsInROI_morph.begin(), vector1DofOriPixelsInROI_morph.end());
	vector<float> tempFloatVec(vector1DofOriPixels_morph.begin(), vector1DofOriPixels_morph.end()); // short -> float 형변환

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
	vector<vector<unsigned short>> vector2DofDiscretizedPixels_morph(nHeight, vector<unsigned short>(nWidth, 0)); // 양자화한 최종 결과 픽셀값들 담을 벡터
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[row][col];
			unsigned short imageValue_discretized = (unsigned short)ceil(tempFloatVec[index]); // psImage (ori) -> tempFloatVec (discretized)

			// ROI 밖(0)과 안(discretized) 픽셀값들 2D Vec에 할당
			if (maskValue >(unsigned char)0) {
				vector2DofDiscretizedPixels_morph[row][col] = imageValue_discretized;
			}
			else {
				vector2DofDiscretizedPixels_morph[row][col] = 0;
			}
		}
	}

	return vector2DofDiscretizedPixels_morph;
}
vector<vector<unsigned short>> GLDZM::get2DVectorOfDiscretizedPixels_FBS(short* psImage, unsigned char* pucMask) {

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
vector<vector<unsigned short>> GLDZM::get2DVectorOfDiscretizedPixels_FBS(short* psImage, vector<vector<unsigned char>> pucMask) {

	float min = (float)*min_element(vector1DofOriPixelsInROI_morph.begin(), vector1DofOriPixelsInROI_morph.end());
	float max = (float)*max_element(vector1DofOriPixelsInROI_morph.begin(), vector1DofOriPixelsInROI_morph.end());
	vector<float> tempFloatVec(vector1DofOriPixels_morph.begin(), vector1DofOriPixels_morph.end());

	//subtract minimum value from every matrix element
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(minus<float>(), min));

	//divide every element of the matrix by the sBin (width of bin)
	if (isnan(sBin)) {
		cout << "invalid value error : sBin! It will be replaced with nBins=32..." << endl;
		sBin = (max - min) / 32; // range
	}
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(divides<float>(), sBin));

	//do rounding(floor) +1, type casting(unsigned short) and asigning to 2D Vec (with ROI)
	vector<vector<unsigned short>> vector2DofDiscretizedPixels_morph(nHeight, vector<unsigned short>(nWidth, 0));
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[row][col];
			unsigned short imageValue_discretized = (unsigned short)floor(tempFloatVec[index]) + 1;

			// ROI 밖(0)과 안(discretized) 픽셀값들 2D Vec에 할당
			if (maskValue >(unsigned char)0) {
				vector2DofDiscretizedPixels_morph[row][col] = imageValue_discretized;
			}
			else {
				vector2DofDiscretizedPixels_morph[row][col] = 0;
			}
		}
	}

	return vector2DofDiscretizedPixels_morph;
}
void GLDZM::get360XYDirections(int &directionX, int &directionY, int angle) {

	if (angle == 360) {
		directionX = 0;
		directionY = 1;
	}
	else if (angle == 315) {
		directionX = 1;
		directionY = 1;
	}
	else if (angle == 270) {
		directionX = 1;
		directionY = 0;
	}
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
int GLDZM::checkNeighbors(vector<vector<unsigned short>> distanceMap, vector<int> actIndex) {
	
	vector<int> neighborElements;
	int tempMin = 0;
	int neighborDist = 1;
	float actMatrElement;
	int directionX;
	int directionY;
	int ang;
	int distance;
	int actMatrElementDist;
	//look at all indices which are saved in the vector matrix indices
	int actPoint, distBorder2;

	actPoint = min(actIndex[0], actIndex[1]);

	for (int i = 0; i < 8; i++) {
		
		ang = 360 - i * 45;
		get360XYDirections(directionX, directionY, ang);
		
		if (actIndex[0] + directionX>-1 && actIndex[0] + directionX<nHeight && actIndex[1] + directionY>-1 && actIndex[1] + directionY<nWidth) {
			actMatrElement = vector2DofDiscretizedPixels_morph[actIndex[0] + directionX][actIndex[1] + directionY];
			actMatrElementDist = distanceMap[actIndex[0] + directionX][actIndex[1] + directionY];
			actPoint = min(actIndex[0], actIndex[1]);
			distBorder2 = nWidth - actIndex[1] - 1;
			actPoint = min(actPoint, distBorder2);
			distBorder2 = nHeight - actIndex[0] - 1;
			actPoint = min(actPoint, distBorder2);
			if (vector2DofDiscretizedPixels_morph[actIndex[0]][actIndex[1]] == 0) {
				distance = 0;
				break;
			}
			if (actMatrElement == 0) {
				distance = 1;
				neighborElements.clear();
				break;
			}

			else if (actPoint == 0) {
				distance = 1;
				break;

			}
			else if (actMatrElementDist>0) {
				neighborElements.push_back(actMatrElementDist);
			}
		}
	}

	if (neighborElements.size() > 0) {
		distance = *min_element(neighborElements.begin(), neighborElements.end()) + 1;
	}
	
	return distance;
}
vector<vector<unsigned short>> GLDZM::generateDistanceMap() {
	// ***반드시 속도 개선 필요!!!***
	vector<vector<unsigned short>> distanceMap(nHeight, vector<unsigned short>(nWidth, 0));
	vector<int> actualIndex;
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			actualIndex.push_back(row);
			actualIndex.push_back(col);

			distanceMap[row][col] = checkNeighbors(distanceMap, actualIndex);
			actualIndex.clear();
		}
	}
	return distanceMap;
}
int GLDZM::findIndex(vector<unsigned short> diffGreyLevels, int size, unsigned short target) {
	int i = 0;
	while ((i < size) && (diffGreyLevels[i] != target)) i++; 
	return (i < size) ? (i) : (-1);
}
void GLDZM::getNeighbours(vector<vector<unsigned short>> &vector2DofDiscretizedPixels, unsigned short actElement, vector<vector<int>> &matrixIndices) {

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
				vector2DofDiscretizedPixels[actIndex[0] + directionX][actIndex[1] + directionY] = 0; // NAN말고 0으로 교체
				actIndex[0] += directionX;
				actIndex[1] += directionY;
				tempMatrixIndices.push_back(actIndex);
				matrixIndices.push_back(actIndex);
			}
		}
	}
}
int GLDZM::getMinimalDistance(vector<vector<unsigned short>> distanceMap, vector<vector<int>> matrixIndices) {

	vector<int> distances;
	int actualX;
	int actualY;
	int actualZ;
	int actualDistance;
	int minDistance;
	vector<int> actualIndex;
	//check for every element of the vector matrix indices the distance
	while (matrixIndices.size()>0) {
		actualIndex = matrixIndices.back();
		matrixIndices.pop_back();
		actualX = actualIndex[0];
		actualY = actualIndex[1];
		actualDistance = distanceMap[actualX][actualY];
		if (actualDistance > 0) {
			distances.push_back(actualDistance);
		}

	}
	//get minimum of all distances of the actual neighborhood
	minDistance = *min_element(distances.begin(), distances.end());

	return minDistance;
}
void GLDZM::fill2DGLDZMatrix(vector<vector<unsigned short>> vector2DofDiscretizedPixels, vector<vector<float>> &GLDZMatrix) {

	// vector2DofDiscretizedPixels : 멤버변수 copy (매개변수로 새 벡터 할당 후 값 변경)
	// vector2DofDiscretizedPixels_morph : 전역 변수(morphological ROI mask 이용) => only distanceMap 생성 시에만 이용
	
	vector<vector<unsigned short>> distanceMap = generateDistanceMap(); // ***반드시 속도 개선 필요!!!***

	//store the matrix indices of a neighborhood in a vector
	vector<vector<int>> matrixIndices;
	//vector with indices of actual position
	vector<int> actualIndex;
	unsigned short actualElement = 0;
	int minDistance = 0;

	//get zones for every grey level present in ROI
	int actualGreyIndex;

	for (int row = 0; row<nHeight; row++) {
		for (int col = 0; col<nWidth; col++) {
			actualElement = vector2DofDiscretizedPixels[row][col];
			
			if (actualElement) {
				actualGreyIndex = findIndex(diffGreyLevels, diffGreyLevels.size(), actualElement); // in ROI : 0~(nBins-1), not in ROI : -1 return
				vector2DofDiscretizedPixels[row][col] = 0;
				actualIndex.push_back(row);
				actualIndex.push_back(col);
				matrixIndices.push_back(actualIndex);
				actualIndex.clear();
				getNeighbours(vector2DofDiscretizedPixels, actualElement, matrixIndices);
			}
			if (matrixIndices.size() > 0) {
				minDistance = getMinimalDistance(distanceMap, matrixIndices);
				matrixIndices.clear();
				GLDZMatrix[actualGreyIndex][minDistance - 1] += 1;
			}
		}
	}
	
}
void GLDZM::fill2DprobMatrix(vector<vector<float>> GLDZMatrix, vector<vector<float>> &probMatrix) {
	
	probMatrix = GLDZMatrix; // 깊은 복사
	for (int i = 0; i < probMatrix.size(); i++) {
		transform(probMatrix[i].begin(), probMatrix[i].end(), probMatrix[i].begin(), bind2nd(divides<float>(), int(totalSum)));
	}

}

// common calculation functions
float GLDZM::getTotalSum(vector<vector<float>> matrix) {
	// calculate the sum of all matrix elements
	float sum = 0;
	for (int i = 0; i<matrix.size(); i++) {
		for (int j = 0; j<matrix[0].size(); j++) {
			sum += matrix[i][j];
		}
	}

	return sum;
}
vector<float> GLDZM::getRowSums(vector<vector<float>> matrix) {
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
vector<float> GLDZM::getColSums(vector<vector<float>> matrix) {
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
float GLDZM::getMeanProbGrey(vector<vector<float>> probMatrix) {
	// calculate the mean probability of the appearance of every grey level
	float mean = 0;
	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			mean += diffGreyLevels[i] * probMatrix[i][j];
		}
	}

	return mean;
}
float GLDZM::getMeanProbRun(vector<vector<float>> probMatrix) {
	// calculate the mean probability of the runlength
	float mean = 0;
	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			mean += j * probMatrix[i][j];
		}
	}

	return mean;
}

void GLDZM::calcShortRunEmph() {
	
	shortRunEmph = 0;

	if (totalSum != 0) {
		for (int j = 0; j < rowSums.size(); j++) {
			shortRunEmph += rowSums[j] / pow(j + 1, 2);
		}
		shortRunEmph /= totalSum;
	}
	
}
void GLDZM::calcLongRunEmph() {

	longRunEmph = 0;

	if (totalSum != 0) {
		for (int j = 0; j < rowSums.size(); j++) {
			longRunEmph += rowSums[j] * pow(j + 1, 2);
		}
		longRunEmph /= totalSum;
	}

}
void GLDZM::calcLowGreyRunEmph() {

	lowGreyRunEmph = 0;

	if (totalSum != 0) {
		for (int i = 0; i < colSums.size(); i++) {
			lowGreyRunEmph += colSums[i] / pow(i + 1, 2);
		}
		lowGreyRunEmph /= totalSum;
	}

}
void GLDZM::calcHighGreyRunEmph() {
	
	highGreyRunEmph = 0;

	if (totalSum != 0) {
		for (int i = 0; i < colSums.size(); i++) {
			highGreyRunEmph += colSums[i] * pow(i + 1, 2);
		}
		highGreyRunEmph /= totalSum;
	}

}
void GLDZM::calcShortRunLowEmph(vector<vector<float>> GLDZMatrix) {

	shortRunLowEmph = 0;

	if (totalSum != 0) {
		for (int row = 0; row < GLDZMatrix.size(); row++) {
			for (int col = 1; col < GLDZMatrix[0].size() + 1; col++) {
				shortRunLowEmph += GLDZMatrix[row][col - 1] / (pow(row+1, 2) * pow(col, 2));
			}
		}
		shortRunLowEmph /= totalSum;
	}

}
void GLDZM::calcShortRunHighEmph(vector<vector<float>> GLDZMatrix) {

	shortRunHighEmph = 0;

	if (totalSum != 0) {
		for (int row = 0; row < GLDZMatrix.size(); row++) {
			for (int col = 1; col < GLDZMatrix[0].size() + 1; col++) {
				shortRunHighEmph += pow(row+1, 2) * GLDZMatrix[row][col - 1] / pow(col, 2);
			}
		}
		shortRunHighEmph /= totalSum;
	}

}
void GLDZM::calcLongRunLowEmph(vector<vector<float>> GLDZMatrix) {

	longRunLowEmph = 0;

	if (totalSum != 0) {
		for (int row = 0; row < GLDZMatrix.size(); row++) {
			for (int col = 1; col < GLDZMatrix[0].size() + 1; col++) {
				longRunLowEmph += pow(col, 2) * GLDZMatrix[row][col - 1] / pow(row+1, 2);
			}
		}
		longRunLowEmph /= totalSum;
	}

}
void GLDZM::calcLongRunHighEmph(vector<vector<float>> GLDZMatrix) {

	longRunHighEmph = 0;

	if (totalSum != 0) {
		for (int row = 0; row < GLDZMatrix.size(); row++) {
			for (int col = 1; col < GLDZMatrix[0].size() + 1; col++) {
				longRunHighEmph += pow(col, 2) * pow(row+1, 2) * GLDZMatrix[row][col - 1];
			}
		}
		longRunHighEmph /= totalSum;
	}

}
void GLDZM::calcGreyNonUnimformity() {

	greyNonUnimformity = 0;

	if (totalSum != 0) {
		for (int i = 0; i < colSums.size(); i++) {
			greyNonUnimformity += pow(colSums[i], 2);
		}
		greyNonUnimformity /= totalSum;
	}

}
void GLDZM::calcGreyNonUnimformityNorm() {

	greyNonUnimformityNorm = 0;

	if (totalSum != 0) {
		for (int i = 0; i < colSums.size(); i++) {
			greyNonUnimformityNorm += pow(colSums[i], 2);
		}
		greyNonUnimformityNorm /= pow(totalSum, 2);
	}

}
void GLDZM::calcRunLengthNonUniformity() {
	
	runLengthNonUniformity = 0;

	if (totalSum != 0) {
		for (int i = 0; i < rowSums.size(); i++) {
			runLengthNonUniformity += pow(rowSums[i], 2);
		}
		runLengthNonUniformity /= totalSum;
	}
	
}
void GLDZM::calcRunLengthNonUniformityNorm() {

	runLengthNonUniformityNorm = 0;

	if (totalSum != 0) {
		for (int i = 0; i < rowSums.size(); i++) {
			runLengthNonUniformityNorm += pow(rowSums[i], 2);
		}
		runLengthNonUniformityNorm /= pow(totalSum, 2);
	}

}
void GLDZM::calcRunPercentage() {

	if (nPixelsInROI != 0) {
		runPercentage = totalSum / nPixelsInROI;
	}
	else {
		runPercentage = 0;
	}
	
}
void GLDZM::calcGreyLevelVar(vector<vector<float>> probMatrix) {

	greyLevelVar = 0;

	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			if (!isnan(pow((diffGreyLevels[i] - meanGrey), 2)*probMatrix[i][j])) {
				greyLevelVar += pow((diffGreyLevels[i] - meanGrey), 2)*probMatrix[i][j];
			}
		}
	}
}
void GLDZM::calcRunLengthVar(vector<vector<float>> probMatrix) {

	runLengthVar = 0;

	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			runLengthVar += pow((j - meanRun), 2)*probMatrix[i][j];
		}
	}
}
void GLDZM::calcRunEntropy(vector<vector<float>> probMatrix) {

	runEntropy = 0;

	for (int i = 0; i<probMatrix.size(); i++) {
		for (int j = 0; j<probMatrix[0].size(); j++) {
			if (probMatrix[i][j] > 0) {
				runEntropy -= probMatrix[i][j] * log2(probMatrix[i][j]);
			}
		}
	}
}

void GLDZM::calcFeature(int FEATURE_IDX, vector<float> &temp1DirVals1DVec, vector<vector<float>> GLDZMatrix, vector<vector<float>> probMatrix) {

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
		calcShortRunLowEmph(GLDZMatrix);
		temp1DirVals1DVec.push_back(shortRunLowEmph);
		break;

	case SRHE:
		calcShortRunHighEmph(GLDZMatrix);
		temp1DirVals1DVec.push_back(shortRunHighEmph);
		break;

	case LRLE:
		calcLongRunLowEmph(GLDZMatrix);
		temp1DirVals1DVec.push_back(longRunLowEmph);
		break;

	case LRHE:
		calcLongRunHighEmph(GLDZMatrix);
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
void GLDZM::featureExtraction(short* psImage, unsigned char* pucMask, vector<vector<unsigned char>> mask_morph, int nHeight_, int nWidth_) {
	
	// claer all values
	clearVariable(); // 슬라이스마다 초기화

	// get discretized 2D input matrix (2d vector)
	nHeight = nHeight_;
	nWidth = nWidth_;
	vector1DofOriPixels = get1DVectorOfPixels(psImage, pucMask);  // 슬라이스마다 초기화, nPixelsInROI 산출
	vector1DofOriPixels_morph = get1DVectorOfPixels(psImage, mask_morph);  // 슬라이스마다 초기화, nPixelsInROI 산출
	vector2DofDiscretizedPixels = isFBN? get2DVectorOfDiscretizedPixels_FBN(psImage, pucMask) : get2DVectorOfDiscretizedPixels_FBS(psImage, pucMask); // 슬라이스마다 초기화, diffGreyLevels 산출
	vector2DofDiscretizedPixels_morph = isFBN ? get2DVectorOfDiscretizedPixels_FBN(psImage, mask_morph) : get2DVectorOfDiscretizedPixels_FBS(psImage, mask_morph); // 슬라이스마다 초기화, diffGreyLevels 산출
	
	sizeGreyLevels = diffGreyLevels.size(); // ***GLDZM에서는 Matrix의 row 크기인 sizeGreyLevels가 maxIntensity가 아닌 diffGreyLevels의 size가 정확!!***
	nrCols = min(ceil(nHeight / 2), ceil(nWidth / 2)) + 1;
	
	// calculate GLDZM matrix
	vector<vector<float>> GLDZMatrix(sizeGreyLevels, vector<float>(nrCols, 0));
	fill2DGLDZMatrix(vector2DofDiscretizedPixels, GLDZMatrix); 
	
	totalSum = getTotalSum(GLDZMatrix);
	rowSums = getRowSums(GLDZMatrix);
	colSums = getColSums(GLDZMatrix);
		
	vector<vector<float>> probMatrix(sizeGreyLevels, vector<float>(nrCols, 0));
	fill2DprobMatrix(GLDZMatrix, probMatrix);

	meanGrey = getMeanProbGrey(probMatrix);
	meanRun = getMeanProbRun(probMatrix);

	vector<float> tempValues1DVec; // 슬라이스마다 초기화 (for. final2DVec에 누적)
	
	// calculate feature
	for (int i = 0; i < FEATURE_COUNT; i++) {
		if (isCheckedFeature[i]) calcFeature(i, tempValues1DVec, GLDZMatrix, probMatrix);
	}
	
	final2DVec.push_back(tempValues1DVec); // 모든 ROI 슬라이스 들어올 때까지 누적 (시리즈마다 초기화)
	
}

void GLDZM::averageAllValues() {

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

void GLDZM::defineFeatureNames(vector<string> &features) {
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
	features[SRE] = "SDE";
	features[LRE] = "LDE";
	features[LGRE] = "LGZE";
	features[HGRE] = "HGZE";
	features[SRLE] = "SDLGE";
	features[SRHE] = "SDHGE";
	features[LRLE] = "LDLGE";
	features[LRHE] = "LDHGE";
	features[GNU] = "GNU";
	features[GNUN] = "GNUN";
	features[RLNU] = "ZDNU";
	features[RLNUN] = "ZDNUN";
	features[RP] = "ZP";
	features[GLV] = "GLV";
	features[RLV] = "ZDV";
	features[RE] = "ZDE";
}

