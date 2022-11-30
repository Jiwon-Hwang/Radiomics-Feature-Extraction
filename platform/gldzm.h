#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <iterator>

class GLDZM
{
public:
	enum FEATURE
	{
		SRE,
		LRE,
		LGRE,
		HGRE,
		SRLE,
		SRHE,
		LRLE,
		LRHE,
		GNU,
		GNUN,
		RLNU,
		RLNUN,
		RP,
		GLV,
		RLV,
		RE,

		FEATURE_COUNT

	};

public:
	GLDZM();
	~GLDZM();

	void clearVariable();
	void clearVector();

	// set state //
	bool isActivatedFamily = false;			// by. platform main
	std::vector<bool> isCheckedFeature;		// by. platform pop-up
	int nCheckedFeatures;

	// get GLDZ-matrix //
	bool isFBN = false;
	int nBins = -1;			// FBN (Fixed Bin Number) => by. platform pop-up
	float sBin = NAN;		// FBS (Fixed Bin Size) => by. platform pop-up
	int sizeGreyLevels;		// sizeGreyLevels == diffGreyLevels.size() (maxIntensity X)
	int nrCols;				
	int nHeight;
	int nWidth;
	int nPixelsInROI;
	std::vector<short> vector1DofOriPixelsInROI;							// ROI 안에 있는 픽셀들만
	std::vector<short> vector1DofOriPixelsInROI_morph;
	std::vector<short> vector1DofOriPixels;									// psImage 전체 픽셀들
	std::vector<short> vector1DofOriPixels_morph;
	std::vector<std::vector<unsigned short>> vector2DofDiscretizedPixels;	// 양자화한 psImage 전체 픽셀들(0 & 1~nBins) == inputMatrix(2d vector)
	std::vector<std::vector<unsigned short>> vector2DofDiscretizedPixels_morph;
	std::vector<unsigned short> diffGreyLevels;
	// 방향마다 초기화
	float totalSum;
	std::vector<float> rowSums; 
	std::vector<float> colSums; 
	float meanGrey;
	float meanRun;

	std::vector<short> get1DVectorOfPixels(short* psImage, unsigned char* pucMask);
	std::vector<short> get1DVectorOfPixels(short* psImage, std::vector<std::vector<unsigned char>> pucMask);
	std::vector<std::vector<unsigned short>> get2DVectorOfDiscretizedPixels_FBN(short* psImage, unsigned char* pucMask);
	std::vector<std::vector<unsigned short>> get2DVectorOfDiscretizedPixels_FBN(short* psImage, std::vector<std::vector<unsigned char>> pucMask);
	std::vector<std::vector<unsigned short>> get2DVectorOfDiscretizedPixels_FBS(short* psImage, unsigned char* pucMask);
	std::vector<std::vector<unsigned short>> get2DVectorOfDiscretizedPixels_FBS(short* psImage, std::vector<std::vector<unsigned char>> pucMask);
	void get360XYDirections(int &directionX, int &directionY, int angle);
	int checkNeighbors(std::vector<std::vector<unsigned short>> distanceMap, std::vector<int> actIndex);
	std::vector<std::vector<unsigned short>> generateDistanceMap();
	int findIndex(std::vector<unsigned short> diffGreyLevels, int size, unsigned short target);
	void getNeighbours(std::vector<std::vector<unsigned short>> &vector2DofDiscretizedPixels, unsigned short actElement, std::vector<std::vector<int>> &matrixIndices);
	int getMinimalDistance(std::vector<std::vector<unsigned short>> distanceMap, std::vector<std::vector<int>> matrixIndices);
	void fill2DGLDZMatrix(std::vector<std::vector<unsigned short>> vector2DofDiscretizedPixels, std::vector<std::vector<float>> &GLDZMatrix);
	void fill2DprobMatrix(std::vector<std::vector<float>> GLDZMatrix, std::vector<std::vector<float>> &probMatrix);

	// common calculation functions
	float getTotalSum(std::vector<std::vector<float>> matrix);
	std::vector<float> getRowSums(std::vector<std::vector<float>> matrix);
	std::vector<float> getColSums(std::vector<std::vector<float>> matrix);
	float getMeanProbGrey(std::vector<std::vector<float>> probMatrix);
	float getMeanProbRun(std::vector<std::vector<float>> probMatrix);

	// put extracted values in 2d vector //
	std::vector<std::vector<float>> final2DVec;		// slice by slice
	std::vector<float> final1DVec;					// series by series (average)

	// final feature value //
	float shortRunEmph = NAN;
	float longRunEmph = NAN;
	float lowGreyRunEmph = NAN;
	float highGreyRunEmph = NAN;
	float shortRunLowEmph = NAN;
	float shortRunHighEmph = NAN;
	float longRunLowEmph = NAN;
	float longRunHighEmph = NAN;
	float greyNonUnimformity = NAN;
	float greyNonUnimformityNorm = NAN;
	float runLengthNonUniformity = NAN;
	float runLengthNonUniformityNorm = NAN;
	float runPercentage = NAN;
	float greyLevelVar = NAN;
	float runLengthVar = NAN;
	float runEntropy = NAN;

	// calculate feature value //
	void calcShortRunEmph();
	void calcLongRunEmph();
	void calcLowGreyRunEmph();
	void calcHighGreyRunEmph();
	void calcShortRunLowEmph(std::vector<std::vector<float>> GLDZMatrix);
	void calcShortRunHighEmph(std::vector<std::vector<float>> GLDZMatrix);
	void calcLongRunLowEmph(std::vector<std::vector<float>> GLDZMatrix);
	void calcLongRunHighEmph(std::vector<std::vector<float>> GLDZMatrix);
	void calcGreyNonUnimformity();
	void calcGreyNonUnimformityNorm();
	void calcRunLengthNonUniformity();
	void calcRunLengthNonUniformityNorm();
	void calcRunPercentage();
	void calcGreyLevelVar(std::vector<std::vector<float>> probMatrix);
	void calcRunLengthVar(std::vector<std::vector<float>> probMatrix);
	void calcRunEntropy(std::vector<std::vector<float>> probMatrix);

	// feature extraction - slice by slice //
	void calcFeature(int FEATURE_IDX, std::vector<float> &temp1DirVals1DVec, std::vector<std::vector<float>> GLDZMatrix, std::vector<std::vector<float>> probMatrix);
	void featureExtraction(short* psImage, unsigned char* pucMask, std::vector<std::vector<unsigned char>> mask_morph, int nHeight, int nWidth);

	// mean all slices - get final feature value //
	void averageAllValues();

	// define and extract calculated values //
	void defineFeatureNames(std::vector<std::string> &features);

};