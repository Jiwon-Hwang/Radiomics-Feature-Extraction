#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>


class NGTDM
{
public:
	enum FEATURE
	{
		COARSENESS,
		CONTRAST,
		BUSYNESS,
		COMPLEXITY,
		STRENGTH,

		FEATURE_COUNT

	};

public:
	NGTDM();
	~NGTDM();

	void clearVariable();
	void clearVector();

	// set state //
	bool isActivatedFamily = false;			// by. platform main
	std::vector<bool> isCheckedFeature;		// by. platform pop-up
	int nCheckedFeatures;

	// get GLRL-matrix //
	bool isFBN = false;
	int nBins = -1;			// FBN (Fixed Bin Number) => by. platform pop-up
	float sBin = NAN;		// FBS (Fixed Bin Size) => by. platform pop-up
	int sizeMatrix;			// sizeMatrix == diffGreyLevels.size() (maxIntensity X)
	int nHeight;
	int nWidth;
	int nPixelsInROI;
	std::vector<short> vector1DofOriPixelsInROI;							// ROI 안에 있는 픽셀들만
	std::vector<short> vector1DofOriPixels;									// psImage 전체 픽셀들
	std::vector<std::vector<unsigned short>> vector2DofDiscretizedPixels;	// 양자화한 psImage 전체 픽셀들(0 & 1~nBins) == inputMatrix(2d vector)
	std::vector<unsigned short> diffGreyLevels;

	std::vector<short> get1DVectorOfPixels(short* psImage, unsigned char* pucMask);
	std::vector<std::vector<unsigned short>> get2DVectorOfDiscretizedPixels_FBN(short* psImage, unsigned char* pucMask);
	std::vector<std::vector<unsigned short>> get2DVectorOfDiscretizedPixels_FBS(short* psImage, unsigned char* pucMask);
	void getProbability(std::vector<unsigned short> elementsOfWholeNeighborhood, std::vector<std::vector<float>> &probMatrix);
	float getNeighborhood(std::vector<std::vector<unsigned short>> vector2DofDiscretizedPixels, int *indexOfElement);
	void fill2DNGTDMatrix(std::vector<std::vector<unsigned short>> vector2DofDiscretizedPixels, std::vector<std::vector<float>> &NGTDMatrix);
	
	// put extracted values in 2d vector //
	std::vector<std::vector<float>> final2DVec;		// slice by slice
	std::vector<float> final1DVec;					// series by series (average)

	// final feature value //
	float coarseness = NAN;
	float contrast = NAN;
	float busyness = NAN;
	float complexity = NAN;
	float strength = NAN;

	// calculate feature value //
	void calcCoarseness(std::vector<std::vector<float>> NGTDMatrix);
	void calContrast(std::vector<std::vector<float>> NGTDMatrix);
	void calcBusyness(std::vector<std::vector<float>> NGTDMatrix);
	void calcComplexity(std::vector<std::vector<float>> NGTDMatrix);
	void calcStrength(std::vector<std::vector<float>> NGTDMatrix);
	
	// feature extraction - slice by slice //
	void calcFeature(int FEATURE_IDX, std::vector<float> &tempValues1DVec, std::vector<std::vector<float>> NGTDMatrix);
	void featureExtraction(short* psImage, unsigned char* pucMask, int nHeight, int nWidth);

	// mean all slices - get final feature value //
	void averageAllValues();

	// define and extract calculated values //
	void defineFeatureNames(std::vector<std::string> &features);

};