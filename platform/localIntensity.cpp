#include "LocalIntensity.h"

using namespace std;

LocalIntensity::LocalIntensity() {

}
LocalIntensity::~LocalIntensity() {

}
void LocalIntensity::clearVariable() {

	localIntensityPeak = NAN;
	globalIntensityPeak = NAN;

}
void LocalIntensity::clearVector() {

	if (!final2DVec.empty()) {
		final2DVec.clear();
	}

	if (!final1DVec.empty()) {
		final1DVec.clear();
	}

}

vector<pair<int, int>> LocalIntensity::getAllMaxIndices(short* pusImage, unsigned char* pucMask) {

	vector<pair<int, int>> allMaxIndices;

	// get max intensity in ROI
	short maxValue = 0; 

	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col; // 1차원으로 접근
			unsigned char maskValue = pucMask[index];
			short imageValue = pusImage[index];

			// ROI 내부 && 임시 최대값보다 큰 값 선택 (ROI가 없으면 계속 pass)
			if (maskValue >(unsigned char)0 && imageValue > maxValue) {
				maxValue = imageValue;
			}
		}
	}

	// get all max indices (posOfCenter)
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col; 
			unsigned char maskValue = pucMask[index];
			short imageValue = pusImage[index];

			if (maskValue >(unsigned char)0 && imageValue == maxValue) {
				//posOfCenter = { row, col };	// c++ 11이상 (vs2012에서 안돼용 ㅜㅜ)
				allMaxIndices.push_back(make_pair(row, col));
			}
		}
	}

	return allMaxIndices;
}
void LocalIntensity::calcLocalIntensityPeak(short* pusImage, unsigned char* pucMask) {

	// 1. get center pos of max (intensity peak indices in ROI)
	vector<pair<int, int>> allMaxIndices = getAllMaxIndices(pusImage, pucMask);
	
	//cout << "maxValue : " << maxValue << endl; // 180
	//cout << "posOfCenter : " << posOfCenter.first << ", " << posOfCenter.second << endl; // (230, 246)

	/*
	// 2. calculateConvolutionMatrix(image)
	// 2-1. get the size of the convolutional matrix (getConvMatrixSize) and allocate convMatrix
	float radius = 6.2;
	double pixelSpacing = m_ciData.getPixelSpacing(9); // 0.650391mm
	int nPixels = 2 * floor((radius - (float)pixelSpacing / 2.0) / pixelSpacing) + 1; // floor : 버림. nPixels (19) : 원모양 컨볼루션 마스크의 지름에 포함되는 최대 픽셀 개수 (가로, 세로 길이)

	unsigned char** convMatrix = new unsigned char*[nPixels]; // 2차원 convolution mask 동적 할당. nPixels개 만큼의 포인터를 행으로 갖고 있는 배열 (nPixels 행)

	for (int row = 0; row < nPixels; row++) {
	convMatrix[row] = new unsigned char[nPixels]; // 각 행의 열값들 채우기(초기화)
	memset(convMatrix[row], 0, sizeof(unsigned char)*nPixels); // 메모리 공간을 0으로 초기화
	}

	// 2-2. fill the convolutional matrix (19 x 19)
	pair<float, float> tempPos = { 0, 0 };
	float distFromCenter;

	for (int row = 0; row < nPixels; row++) {
	for (int col = 0; col < nPixels; col++) {
	tempPos.first = float(row) * pixelSpacing + pixelSpacing / 2 - (posOfCenter.first + 0.5) * pixelSpacing; // X축(row) 방향 거리(mm)
	tempPos.second = float(col) * pixelSpacing + pixelSpacing / 2 - (posOfCenter.second + 0.5) * pixelSpacing; // Y
	cout << "tempPos.first : " << tempPos.first << endl;
	cout << "tempPos.second : " << tempPos.second << endl;

	distFromCenter = sqrt(pow((tempPos.first - (posOfCenter.first + 0.5)*pixelSpacing), 2) + pow((tempPos.second - (posOfCenter.second + 0.5) * pixelSpacing), 2));
	if (abs(tempPos.first) <= 6.2 && abs(tempPos.second) <= 6.2) {
	//cout << "hi?" << endl;
	convMatrix[row][col] = 1;
	}
	//cout << convMatrix[row][col] << " ";
	}
	//cout << endl;
	}


	// convMatrix 찍어보기? size -> nPixel or 512?
	Mat img(Size(nPixels, nPixels), CV_8UC1, convMatrix);
	imshow("img", img);
	waitKey(0);


	// 2-3. calculateConvolutionMatrix => getConvMatix & fillConvMatrix 호출 후 convMatrix return (nPixels x nPixels 크기)
	*/

	// 2. check distFromCenter and calculate sum (mean) 
	float radius = 6.2;
	int nrPixelsX = floor((radius - (float)pixelSpacingX / 2.0) / pixelSpacingX); // 9, X : width (col), floor : 버림
	int nrPixelsY = floor((radius - (float)pixelSpacingY / 2.0) / pixelSpacingY); // 9, Y : height (row), floor : 버림
	//int nPixels = 2 * nrPixels + 1; // 19

	pair<float, float> tempDist;
	float distFromCenter;
	float sumIntensity = 0; // ***short -> float!*** (범위 문제로 최종 sum값이 -(마이너스) 값으로 나옴!)
	int cnt = 0; // 279

	// [230-9][246-9] center 좌표의 픽셀로부터 거리 계산 => 9 : nrPixels
	for (int i = 0; i < allMaxIndices.size(); i++) {
		for (int row = allMaxIndices[i].first - nrPixelsY; row < allMaxIndices[i].first + nrPixelsY; row++) {
			for (int col = allMaxIndices[i].second - nrPixelsX; col < allMaxIndices[i].second + nrPixelsX; col++) {
				tempDist.first = float(row) * pixelSpacingY + pixelSpacingY / 2 - (allMaxIndices[i].first + 0.5) * pixelSpacingY; // X축(row) 방향 중점과의 거리(mm) => X_temp - X_center
				tempDist.second = float(col) * pixelSpacingX + pixelSpacingX / 2 - (allMaxIndices[i].second + 0.5) * pixelSpacingX; // Y
				distFromCenter = sqrt(pow(tempDist.first, 2) + pow(tempDist.second, 2));
				if (distFromCenter <= 6.2 && 0 <= row && row < nHeight && 0 <= col && col < nWidth) {
					// sum pixel
					int index = row * nWidth + col;
					sumIntensity += pusImage[index];
					cnt++;
				}
			}
		}	
		if (isnan(localIntensityPeak)) {
			localIntensityPeak = sumIntensity / cnt; // initialize
		}
		else {
			localIntensityPeak = (sumIntensity / cnt) > localIntensityPeak ? (sumIntensity / cnt) : localIntensityPeak;
		}
	 }

}
void LocalIntensity::calcGlobalIntensityPeak(short* psImage, unsigned char* pucMask) {


	
}

void LocalIntensity::calcFeature(int FEATURE_IDX, vector<float> &tempValues1DVec, short* psImage, unsigned char* pucMask) {
	
	switch (FEATURE_IDX)
	{
		case LOCAL:
			calcLocalIntensityPeak(psImage, pucMask);
			tempValues1DVec.push_back(localIntensityPeak);
			break;

		case GLOBAL:
			calcGlobalIntensityPeak(psImage, pucMask);
			tempValues1DVec.push_back(globalIntensityPeak);
			break;

		default:
			break;
	}
}
void LocalIntensity::featureExtraction(short* psImage, unsigned char* pucMask, int nHeight_, int nWidth_, float pixelSpacingX_, float pixelSpacingY_) {
	
	// claer all values
	clearVariable(); // 슬라이스마다 초기화

	nHeight = nHeight_;
	nWidth = nWidth_;
	pixelSpacingX = pixelSpacingX_;
	pixelSpacingY = pixelSpacingY_;

	vector<float> tempValues1DVec; // 슬라이스마다 초기화

	// calculate checked feature
	for (int i = 0; i < FEATURE_COUNT; i++) {
		if (isCheckedFeature[i]) calcFeature(i, tempValues1DVec, psImage, pucMask);
	}

	final2DVec.push_back(tempValues1DVec); // 모든 ROI 슬라이스 들어올 때까지 누적 (시리즈마다 초기화)

}

void LocalIntensity::averageAllValues() {
	
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

void LocalIntensity::defineFeatureNames(vector<string> &features) {
	
	features[LOCAL] = "local intensity peak";
	features[GLOBAL] = "global intensity peak";

}
void LocalIntensity::extractFeatureValues(vector<float> &LocalIntensityValues) {
	// platform.cpp의 writeCSVCheckedValue()에서 참조 가능. But, 속도 문제로 사용 x 
	LocalIntensityValues[LOCAL] = localIntensityPeak;
	LocalIntensityValues[GLOBAL] = globalIntensityPeak;

}

