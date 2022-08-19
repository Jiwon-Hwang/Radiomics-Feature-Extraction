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
			int index = row * nWidth + col; // 1�������� ����
			unsigned char maskValue = pucMask[index];
			short imageValue = pusImage[index];

			// ROI ���� && �ӽ� �ִ밪���� ū �� ���� (ROI�� ������ ��� pass)
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
				//posOfCenter = { row, col };	// c++ 11�̻� (vs2012���� �ȵſ� �̤�)
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
	int nPixels = 2 * floor((radius - (float)pixelSpacing / 2.0) / pixelSpacing) + 1; // floor : ����. nPixels (19) : ����� ������� ����ũ�� ������ ���ԵǴ� �ִ� �ȼ� ���� (����, ���� ����)

	unsigned char** convMatrix = new unsigned char*[nPixels]; // 2���� convolution mask ���� �Ҵ�. nPixels�� ��ŭ�� �����͸� ������ ���� �ִ� �迭 (nPixels ��)

	for (int row = 0; row < nPixels; row++) {
	convMatrix[row] = new unsigned char[nPixels]; // �� ���� ������ ä���(�ʱ�ȭ)
	memset(convMatrix[row], 0, sizeof(unsigned char)*nPixels); // �޸� ������ 0���� �ʱ�ȭ
	}

	// 2-2. fill the convolutional matrix (19 x 19)
	pair<float, float> tempPos = { 0, 0 };
	float distFromCenter;

	for (int row = 0; row < nPixels; row++) {
	for (int col = 0; col < nPixels; col++) {
	tempPos.first = float(row) * pixelSpacing + pixelSpacing / 2 - (posOfCenter.first + 0.5) * pixelSpacing; // X��(row) ���� �Ÿ�(mm)
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


	// convMatrix ����? size -> nPixel or 512?
	Mat img(Size(nPixels, nPixels), CV_8UC1, convMatrix);
	imshow("img", img);
	waitKey(0);


	// 2-3. calculateConvolutionMatrix => getConvMatix & fillConvMatrix ȣ�� �� convMatrix return (nPixels x nPixels ũ��)
	*/

	// 2. check distFromCenter and calculate sum (mean) 
	float radius = 6.2;
	int nrPixelsX = floor((radius - (float)pixelSpacingX / 2.0) / pixelSpacingX); // 9, X : width (col), floor : ����
	int nrPixelsY = floor((radius - (float)pixelSpacingY / 2.0) / pixelSpacingY); // 9, Y : height (row), floor : ����
	//int nPixels = 2 * nrPixels + 1; // 19

	pair<float, float> tempDist;
	float distFromCenter;
	float sumIntensity = 0; // ***short -> float!*** (���� ������ ���� sum���� -(���̳ʽ�) ������ ����!)
	int cnt = 0; // 279

	// [230-9][246-9] center ��ǥ�� �ȼ��κ��� �Ÿ� ��� => 9 : nrPixels
	for (int i = 0; i < allMaxIndices.size(); i++) {
		for (int row = allMaxIndices[i].first - nrPixelsY; row < allMaxIndices[i].first + nrPixelsY; row++) {
			for (int col = allMaxIndices[i].second - nrPixelsX; col < allMaxIndices[i].second + nrPixelsX; col++) {
				tempDist.first = float(row) * pixelSpacingY + pixelSpacingY / 2 - (allMaxIndices[i].first + 0.5) * pixelSpacingY; // X��(row) ���� �������� �Ÿ�(mm) => X_temp - X_center
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
	clearVariable(); // �����̽����� �ʱ�ȭ

	nHeight = nHeight_;
	nWidth = nWidth_;
	pixelSpacingX = pixelSpacingX_;
	pixelSpacingY = pixelSpacingY_;

	vector<float> tempValues1DVec; // �����̽����� �ʱ�ȭ

	// calculate checked feature
	for (int i = 0; i < FEATURE_COUNT; i++) {
		if (isCheckedFeature[i]) calcFeature(i, tempValues1DVec, psImage, pucMask);
	}

	final2DVec.push_back(tempValues1DVec); // ��� ROI �����̽� ���� ������ ���� (�ø���� �ʱ�ȭ)

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
	// platform.cpp�� writeCSVCheckedValue()���� ���� ����. But, �ӵ� ������ ��� x 
	LocalIntensityValues[LOCAL] = localIntensityPeak;
	LocalIntensityValues[GLOBAL] = globalIntensityPeak;

}

