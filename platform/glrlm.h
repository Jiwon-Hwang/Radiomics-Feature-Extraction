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
	std::vector<float> diagonalProbabilities;
	std::vector<float> crossProbabilities;
	std::vector<float> sumProbCols;		// for. correlation
	std::vector<float> sumProbRows;		// for. correlation

	std::vector<short> get1DVectorOfPixels(short* psImage, unsigned char* pucMask);
	std::vector<std::vector<unsigned short>> get2DVectorOfDiscretizedPixels_nBins(short* psImage, unsigned char* pucMask);
	void getXYDirections(int &directionX, int &directionY, int angle);
	std::vector<std::pair<unsigned short, unsigned short>> getNeighbours2D(unsigned char* pucMask, int directionX, int directionY);
	void fill2DGLCMatrix(std::vector<std::vector<float>> &GLCMatrix, unsigned char* pucMask, int angle);
	void calcDiagonalProbabilities(std::vector<std::vector<float>> GLCMatrix);
	void calcCrossProbabilities(std::vector<std::vector<float>> GLCMatrix);
	void average4DirValues(std::vector<std::vector<float>> temp4DirVals2DVec, std::vector<float> &tempValues1DVec);

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
	void calcJointMaximum(std::vector<std::vector<float>> GLCMatrix);
	void calcJointAverage(std::vector<std::vector<float>> GLCMatrix);
	void calcJointVariance(std::vector<std::vector<float>> GLCMatrix);
	void calcJointEntropy(std::vector<std::vector<float>> GLCMatrix);
	void calcDiffAverage(std::vector<std::vector<float>> GLCMatrix);
	void calcDiffVariance(std::vector<std::vector<float>> GLCMatrix);
	void calcDiffEntropy(std::vector<std::vector<float>> GLCMatrix);
	void calcSumAverage(std::vector<std::vector<float>> GLCMatrix);
	void calcSumVariance(std::vector<std::vector<float>> GLCMatrix);
	void calcSumEntropy(std::vector<std::vector<float>> GLCMatrix);
	void calcAngSecMoment(std::vector<std::vector<float>> GLCMatrix);
	void calcContrast(std::vector<std::vector<float>> GLCMatrix);
	void calcDissimilarity(std::vector<std::vector<float>> GLCMatrix);
	void calcInverseDiff(std::vector<std::vector<float>> GLCMatrix);
	void calcInverseDiffNorm(std::vector<std::vector<float>> GLCMatrix);
	void calcInverseDiffMom(std::vector<std::vector<float>> GLCMatrix);
	void calcInverseDiffMomNorm(std::vector<std::vector<float>> GLCMatrix);
	void calcInverseVar(std::vector<std::vector<float>> GLCMatrix);
	void calcColProb(std::vector<std::vector<float>> GLCMatrix);	// for. correlation
	void calcRowProb(std::vector<std::vector<float>> GLCMatrix);	// for. correlation
	void calcMeanColProb(std::vector<std::vector<float>> GLCMatrix);	// for. correlation, clusterTendency, clusterShade, clusterProminence
	void calcMeanRowProb(std::vector<std::vector<float>> GLCMatrix);	// for. correlation
	void calcCorrelation(std::vector<std::vector<float>> GLCMatrix);
	void calcAutoCorrelation(std::vector<std::vector<float>> GLCMatrix);
	void calcClusterTendency(std::vector<std::vector<float>> GLCMatrix);
	void calcClusterShade(std::vector<std::vector<float>> GLCMatrix);
	void calcClusterProminence(std::vector<std::vector<float>> GLCMatrix);
	void calcFirstMCorrelation(std::vector<std::vector<float>> GLCMatrix);
	void calcSecondMCorrelation(std::vector<std::vector<float>> GLCMatrix);

	// feature extraction - slice by slice //
	void calcFeature(int FEATURE_IDX, std::vector<float> &temp1DirVals1DVec, std::vector<std::vector<float>> sumGLCM);
	void featureExtraction(short* psImage, unsigned char* pucMask, int nHeight, int nWidth);

	// mean all slices - get final feature value //
	void averageAllValues();

	// define and extract calculated values //
	void defineFeatureNames(std::vector<std::string> &features);

};