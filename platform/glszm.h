#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>


class GLSZM
{
public:
	enum FEATURE
	{
		SZE,
		LZE,
		LGZE,
		HGZE,
		SZLGE,
		SZHGE,
		LZLGE,
		LZHGE,
		GNU,
		GNUN,
		ZSNU,
		ZSNUN,
		ZP,
		GLV,
		ZSV,
		ZSE,

		FEATURE_COUNT

	};

public:
	GLSZM();
	~GLSZM();

	void clearVariable();
	void clearVector();

	// set state //
	bool isActivatedFamily = false;			// by. platform main
	std::vector<bool> isCheckedFeature;		// by. platform pop-up
	int nCheckedFeatures;

	// get GLSZ-matrix //
	bool isFBN = false;
	int nBins = -1;			// FBN (Fixed Bin Number) => by. platform pop-up
	float sBin = NAN;		// FBS (Fixed Bin Size) => by. platform pop-up
	int sizeMatrix;			// sizeMatrix == diffGreyLevels.size() (maxIntensity X)
	int maxZoneSize;		
	int nHeight;
	int nWidth;
	int nPixelsInROI;
	std::vector<short> vector1DofOriPixelsInROI;							// ROI 안에 있는 픽셀들만
	std::vector<short> vector1DofOriPixels;									// psImage 전체 픽셀들
	std::vector<std::vector<unsigned short>> vector2DofDiscretizedPixels;	// 양자화한 psImage 전체 픽셀들(0 & 1~nBins) == inputMatrix(2d vector)
	std::vector<unsigned short> diffGreyLevels;
	
	float totalSum;
	std::vector<float> rowSums; 
	std::vector<float> colSums; 
	float meanGrey;
	float meanRun;

	std::vector<short> get1DVectorOfPixels(short* psImage, unsigned char* pucMask);
	std::vector<std::vector<unsigned short>> get2DVectorOfDiscretizedPixels_FBN(short* psImage, unsigned char* pucMask);
	std::vector<std::vector<unsigned short>> get2DVectorOfDiscretizedPixels_FBS(short* psImage, unsigned char* pucMask);
	int findIndex(std::vector<unsigned short> diffGreyLevels, int size, unsigned short target);
	void get360XYDirections(int &directionX, int &directionY, int angle);
	void getNeighbours(std::vector<std::vector<unsigned short>> &vector2DofDiscretizedPixels, unsigned short actElement, std::vector<std::vector<int>> &matrixIndices);
	void fill2DGLSZMatrix(std::vector<std::vector<unsigned short>> vector2DofDiscretizedPixels, std::vector<std::vector<float>> &GLSZMatrix);
	void fill2DprobMatrix(std::vector<std::vector<float>> GLSZMatrix, std::vector<std::vector<float>> &probMatrix);

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
	float smallZoneEmph = NAN;
	float largeZoneEmph = NAN;
	float lowGreyZoneEmph = NAN;
	float highGreyZoneEmph = NAN;
	float smallZoneLowGreyEmph = NAN;
	float smallZoneHighGreyEmph = NAN;
	float largeZoneLowGreyEmph = NAN;
	float largeZoneHighGreyEmph = NAN;
	float greyNonUniformity = NAN;
	float greyNonUniformityNorm = NAN;
	float zoneSizeNonUniformity = NAN;
	float zoneSizeNonUniformityNorm = NAN;
	float zonePercentage = NAN;
	float greyLevelVar = NAN;
	float zoneSizeVar = NAN;
	float zoneSizeEntropy = NAN;

	// calculate feature value //
	void calcSmallZoneEmph();
	void calcLargeZoneEmph();
	void calcLowGreyZoneEmph();
	void calcHighGreyZoneEmph();
	void calcSmallZoneLowGreyEmph(std::vector<std::vector<float>> GLSZMatrix);
	void calcSmallZoneHighGreyEmph(std::vector<std::vector<float>> GLSZMatrix);
	void calcLargeZoneLowGreyEmph(std::vector<std::vector<float>> GLSZMatrix);
	void calcLargeZoneHighGreyEmph(std::vector<std::vector<float>> GLSZMatrix);
	void calcGreyNonUniformity();
	void calcGreyNonUniformityNorm();
	void calcZoneSizeNonUniformity();
	void calcZoneSizeNonUniformityNorm();
	void calcZonePercentage();
	void calcGreyLevelVar(std::vector<std::vector<float>> probMatrix);
	void calcZoneSizeVar(std::vector<std::vector<float>> probMatrix);
	void calcZoneSizeEntropy(std::vector<std::vector<float>> probMatrix);

	// feature extraction - slice by slice //
	void calcFeature(int FEATURE_IDX, std::vector<float> &temp1DirVals1DVec, std::vector<std::vector<float>> GLSZMatrix, std::vector<std::vector<float>> probMatrix);
	void featureExtraction(short* psImage, unsigned char* pucMask, int nHeight, int nWidth);

	// mean all slices - get final feature value //
	void averageAllValues();

	// define and extract calculated values //
	void defineFeatureNames(std::vector<std::string> &features);

};