#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>


class GLRLM
{
public:
	enum FEATURE
	{
		SRE,
		LRE,
		LGE,
		HGE,
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
	GLRLM();
	~GLRLM();

	void clearVariable();
	void clearVector();

	// set state //
	bool isActivatedFamily = false;			// by. platform main
	std::vector<bool> isCheckedFeature;		// by. platform pop-up
	int nCheckedFeatures;

	// get GLRL-matrix //
	int nBins = 32;
	int sizeMatrix;		// N_g == nBins == sizeMatrix == maxIntensity
	int maxRunLength;	// N_r
	int nHeight;
	int nWidth;
	std::vector<short> vector1DofOriPixelsInROI;							// ROI 안에 있는 픽셀들만
	std::vector<short> vector1DofOriPixels;									// psImage 전체 픽셀들
	std::vector<std::vector<unsigned short>> vector2DofDiscretizedPixels;	// 양자화한 psImage 전체 픽셀들(0 & 1~nBins) == inputMatrix(2d vector)
	std::vector<float> diagonalProbabilities;
	std::vector<float> crossProbabilities;
	std::vector<float> sumProbCols;		// for. correlation
	std::vector<float> sumProbRows;		// for. correlation

	std::vector<short> get1DVectorOfPixels(short* psImage, unsigned char* pucMask);
	std::vector<std::vector<unsigned short>> get2DVectorOfDiscretizedPixels_nBins(short* psImage, unsigned char* pucMask);
	void getXYDirections(int &directionX, int &directionY, int angle);
	int findIndex(int size, unsigned short target);
	void fill2DGLRLMatrix(std::vector<std::vector<unsigned short>> vector2DofDiscretizedPixels, std::vector<std::vector<float>> &GLRLMatrix, int angle);
	void calcDiagonalProbabilities(std::vector<std::vector<float>> GLRLMatrix);
	void calcCrossProbabilities(std::vector<std::vector<float>> GLRLMatrix);
	void average4DirValues(std::vector<std::vector<float>> temp4DirVals2DVec, std::vector<float> &tempValues1DVec);

	// common calculation functions
	float getTotalSum(std::vector<std::vector<float>> matrix);
	std::vector<float> getRowSums(std::vector<std::vector<float>> matrix);
	std::vector<float> getColSums(std::vector<std::vector<float>> matrix);

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
	float meanRowProb = NAN;	// for. correlation, clusterTendency, clusterShade, clusterProminence
	float meanColProb = NAN;	// for. correlation
	float stdRowProb = NAN;		// for. correlation
	float stdColProb = NAN;		// for. correlation
	float correlation = NAN;
	float autoCorrelation = NAN;
	float clusterTendency = NAN;
	float clusterShade = NAN;
	float clusterProminence = NAN;
	float HXY = NAN;			// for. firstMCorrelation, secondMCorrelation
	float HX = NAN;				// for. firstMCorrelation
	float HXY1 = NAN;			// for. firstMCorrelation
	float HXY2 = NAN;			// for. secondMCorrelation
	float firstMCorrelation = NAN;
	float secondMCorrelation = NAN;

	// calculate feature value //
	void calcJointMaximum(std::vector<std::vector<float>> GLRLMatrix);
	void calcJointAverage(std::vector<std::vector<float>> GLRLMatrix);
	void calcJointVariance(std::vector<std::vector<float>> GLRLMatrix);
	void calcJointEntropy(std::vector<std::vector<float>> GLRLMatrix);
	void calcDiffAverage(std::vector<std::vector<float>> GLRLMatrix);
	void calcDiffVariance(std::vector<std::vector<float>> GLRLMatrix);
	void calcDiffEntropy(std::vector<std::vector<float>> GLRLMatrix);
	void calcSumAverage(std::vector<std::vector<float>> GLRLMatrix);
	void calcSumVariance(std::vector<std::vector<float>> GLRLMatrix);
	void calcSumEntropy(std::vector<std::vector<float>> GLRLMatrix);
	void calcAngSecMoment(std::vector<std::vector<float>> GLRLMatrix);
	void calcContrast(std::vector<std::vector<float>> GLRLMatrix);
	void calcDissimilarity(std::vector<std::vector<float>> GLRLMatrix);
	void calcInverseDiff(std::vector<std::vector<float>> GLRLMatrix);
	void calcInverseDiffNorm(std::vector<std::vector<float>> GLRLMatrix);
	void calcInverseDiffMom(std::vector<std::vector<float>> GLRLMatrix);
	void calcInverseDiffMomNorm(std::vector<std::vector<float>> GLRLMatrix);
	void calcInverseVar(std::vector<std::vector<float>> GLRLMatrix);
	void calcColProb(std::vector<std::vector<float>> GLRLMatrix);	// for. correlation
	void calcRowProb(std::vector<std::vector<float>> GLRLMatrix);	// for. correlation
	void calcMeanColProb(std::vector<std::vector<float>> GLRLMatrix);	// for. correlation, clusterTendency, clusterShade, clusterProminence
	void calcMeanRowProb(std::vector<std::vector<float>> GLRLMatrix);	// for. correlation
	void calcCorrelation(std::vector<std::vector<float>> GLRLMatrix);
	void calcAutoCorrelation(std::vector<std::vector<float>> GLRLMatrix);
	void calcClusterTendency(std::vector<std::vector<float>> GLRLMatrix);
	void calcClusterShade(std::vector<std::vector<float>> GLRLMatrix);
	void calcClusterProminence(std::vector<std::vector<float>> GLRLMatrix);
	void calcFirstMCorrelation(std::vector<std::vector<float>> GLRLMatrix);
	void calcSecondMCorrelation(std::vector<std::vector<float>> GLRLMatrix);

	// feature extraction - slice by slice //
	void calcFeature(int FEATURE_IDX, std::vector<float> &temp1DirVals1DVec, std::vector<std::vector<float>> sumGLRLM);
	void featureExtraction(short* psImage, unsigned char* pucMask, int nHeight, int nWidth);

	// mean all slices - get final feature value //
	void averageAllValues();

	// define and extract calculated values //
	void defineFeatureNames(std::vector<std::string> &features);

};