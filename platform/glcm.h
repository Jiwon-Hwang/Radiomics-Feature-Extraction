#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>


class GLCM
{
public:
	enum FEATURE
	{
		JOINTMAXIMUM,
		JOINTAVERAGE,
		JOINTVARIANCE,
		JOINTENTROPY,
		DIFFAVERAGE,
		DIFFVARIANCE,
		DIFFENTROPY,
		SUMAVERAGE,
		SUMVARIANCE,
		SUMENTROPY,
		ANGSECMOMENT,
		CONTRAST,
		DISSIMILARITY,
		INVERSEDIFF,
		INVERSEDIFFNORM,
		INVERSEDIFFMOM,
		INVERSEDIFFMOMNORM,
		INVERSEVAR,
		CORRELATION,
		AUTOCORRELATION,
		CLUSTERTENDENCY,
		CLUSTERSHADE,
		CLUSTERPROMINENCE,
		FIRSTMCORRELATION,
		SECONDMCORRELATION,

		FEATURE_COUNT

	};

public:
	GLCM();
	~GLCM();

	void clearVariable();
	void clearVector();

	// set state //
	bool isActivatedFamily = false;			// by. platform main
	std::vector<bool> isCheckedFeature;		// by. platform pop-up
	int nCheckedFeatures;

	// get GLC-matrix //
	int nBins = 32;
	int sizeMatrix; // nBins == sizeMatrix == maxIntensity
	int nHeight;
	int nWidth;
	std::vector<short> vector1DofOriPixelsInROI;							// ROI 안에 있는 픽셀들만
	std::vector<short> vector1DofOriPixels;									// psImage 전체 픽셀들
	std::vector<std::vector<unsigned short>> vector2DofDiscretizedPixels;	// psImage 전체 픽셀들
	std::vector<float> sumProbCols;
	std::vector<float> sumProbRows;
	std::vector<float> diagonalProbabilities;
	std::vector<float> crossProbabilities;
	
	std::vector<short> get1DVectorOfPixels(short* psImage, unsigned char* pucMask, int nHeight, int nWidth);
	std::vector<std::vector<unsigned short>> get2DVectorOfDiscretizedPixels_nBins(short* psImage, unsigned char* pucMask, int nHeight, int nWidth);
	void getXYDirections(int &directionX, int &directionY, int angle);
	std::vector<std::pair<unsigned short, unsigned short>> getNeighbours2D(unsigned char* pucMask, int directionX, int directionY);
	void fill2DGLCMatrix(std::vector<std::vector<float>> &GLCMatrix, unsigned char* pucMask, int angle);
	void GLCM::average4DirValues(std::vector<std::vector<float>> temp4DirVals2DVec, std::vector<float> &tempValues1DVec);

	// put extracted values in 2d vector //
	std::vector<std::vector<float>> final2DVec;		// slice by slice
	std::vector<float> final1DVec;					// series by series (average)

	// final feature value //
	float jointMaximum = NAN;
	float jointAverage = NAN;
	float jointVariance = NAN;
	float jointEntropy = NAN;
	float diffAverage = NAN;
	float diffVariance = NAN;
	float diffEntropy = NAN;
	float sumAverage = NAN;
	float sumVariance = NAN;
	float sumEntropy = NAN;
	float angSecMoment = NAN;
	float contrast = NAN;
	float dissimilarity = NAN;
	float inverseDiff = NAN;
	float inverseDiffNorm = NAN;
	float inverseDiffMom = NAN;
	float inverseDiffMomNorm = NAN;
	float inverseVar = NAN;
	//float meanRowProb = NAN;
	//float meanColProb = NAN;
	//float stdRowProb = NAN;
	//float stdColProb = NAN;
	float correlation = NAN;
	float autoCorrelation = NAN;
	float clusterTendency = NAN;
	float clusterShade = NAN;
	float clusterProminence = NAN;
	float firstMCorrelation = NAN;
	float secondMCorrelation = NAN;

	// calculate feature value //
	void calcJointMaximum(std::vector<std::vector<float>> GLCMatrix);

	// feature extraction - slice by slice //
	void calcFeature(int FEATURE_IDX, std::vector<float> &temp1DirVals1DVec, std::vector<std::vector<float>> sumGLCM);
	void featureExtraction(short* psImage, unsigned char* pucMask, int nHeight, int nWidth);

	// mean all slices - get final feature value //
	void averageAllValues();

	// define and extract calculated values //
	void defineFeatureNames(std::vector<std::string> &features);
	void extractFeatureValues(std::vector<float> &glcmValues);

};