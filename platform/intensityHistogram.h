#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>

// enum, struct, map���� ����! => #define�� ������� ���� ������� �������� ����!
// ������ & �Լ��� ���ν�Ű�� (for. platform.cpp�� feature extraction �� for loop���� index�� �ѹ��� ���� �����ϵ���!)
#define MEAN 0
#define VARIANCE 1
#define SKEWNESS 2
#define KURTOSIS 3 
#define MEDIAN 4
#define MINIMUM 5
#define PERCENTILE10 6 
#define PERCENTILE90 7 
#define MAXIMUM 8
#define INTERQUARTILERANGE 9
#define MODE 10
#define RANGE 11
#define MEANABSDEV 12
#define ROBUSTMEANABSDEV 13
#define MEDIANABSDEV 14
#define COEFFOFVAR 15
#define QUARTILECOEFF 16
#define ENTROPY 17
#define UNIFORMITY 18
#define MAXHISTGRADIENT 19
#define MAXHISTGRADGREY 20
#define MINHISTGRADIENT 21
#define MINHISTGRADGREY 22


class IntensityHistogram 
{
	private:


	public:	
		IntensityHistogram();
		~IntensityHistogram();

		// set state //
		bool isActivatedFamily = false;		// by. platform main
		std::vector<bool> isCheckedFeature;		// by. platform pop-up
		int nCheckedFeatures;				// **���� ���� isCheckedFeature ���ͷκ��� TRUE ���� ���ϱ�**

		// get histogram //
		int nBins = 32;
		std::vector<short> vectorOfOriPixels;
		std::vector<unsigned short> vectorOfDiscretizedPixels;
		std::vector<unsigned int> hist;

		std::vector<short> getVectorOfPixelsInROI(short* psImage, unsigned char* pucMask, int nHeight, int nWidth);
		std::vector<unsigned short> getVectorOfDiscretizedPixels_nBins();
		std::vector<unsigned short> getVectorOfDiffGreyLevels();
		std::vector<unsigned int> getHistogram();

		// calculate feature //
		float meanValue = NAN;
		float varianceValue = NAN;
		float skewnessValue = NAN;
		float kurtosisValue = NAN;
		float medianValue = NAN;
		float minimumValue = NAN;
		float percentile10 = NAN;
		float percentile90 = NAN;
		float maximumValue = NAN;
		float interquartileRange = NAN;
		float mode = NAN;
		float rangeValue = NAN;
		float meanAbsDev = NAN;
		float robustMeanAbsDev = NAN;
		float medianAbsDev = NAN;
		float coeffOfVar = NAN;
		float quartileCoeff = NAN;
		float entropy = NAN;
		float histUniformity = NAN;
		float maxHistGradient = NAN;
		float maxHistGradGreyValue = NAN;
		float minHistGradient = NAN;
		float minHistGradGreyValue = NAN;
		//float percentile25 = NAN;
		//float percentile75 = NAN;
		//vector<float> maxHistVecGradient;
		//vector<float> minHistVecGradient;

		float calcMean(std::vector<unsigned short> vectorOfDiscretizedPixels);
		float calcVariance(std::vector<unsigned short> vectorOfDiscretizedPixels);
		void calcSkewness();
		void calcKurtosis();
		void getMedian(std::vector<float> vectorMatrElement);
		void getMinimum(std::vector<float> matrixVector);
		double getPercentile(std::vector<float> matrixVector, float probability);
		void get10percentile(std::vector<float> matrixVector);
		void get90percentile(std::vector<float> matrixVector);
		void getMaximum(std::vector<float> matrixVector);
		void getInterquartileRange(std::vector<float> matrixVector);
		void getMode();
		void getRange();
		void meanAbsoluteDev(std::vector<float> vectorMatrElem);
		void getRobustMeanAbsDev(std::vector<float> vectorMatrElem);
		void medianAbsoluteDev(std::vector<float> vectorMatrElem);
		void getCoeffOfVar();
		void getQuartileCoeff();
		void getEntropy();
		void getHistUniformity();
		void getMaxHistGradient();
		void getMinHistGradient();

		// feature extraction - only checked feature //
		void calcFeature(int FEATURE_IDX);
		void featureExtraction(short* psImage, unsigned char* pucMask, int nHeight, int nWidth);

		// define and extract calculated values //
		void defineFeatureNames(std::vector<std::string> &features);
		void extractFeatureValues(std::vector<float> &intensityHistogramValues);

};