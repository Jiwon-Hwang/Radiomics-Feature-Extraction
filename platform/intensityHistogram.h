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
		void clear(void);

		// set state //
		bool isActivatedFamily = FALSE;		// by. platform main
		vector<bool> isCheckedFeature;		// by. platform pop-up
		int nCheckedFeatures;				// **���� ���� isCheckedFeature ���ͷκ��� TRUE ���� ���ϱ�**

		// get histogram //
		int nBins = 32;
		vector<short> vectorOfOriPixels;
		vector<unsigned short> vectorOfDiscretizedPixels;
		vector<unsigned int> hist;

		vector<short> getVectorOfPixelsInROI(short* psImage, unsigned char* pucMask, int nHeight, int nWidth);
		vector<unsigned short> getVectorOfDiscretizedPixels_nBins(vector<short> vectorOfOriPixels, int nBins = 32);
		vector<unsigned short> getVectorOfDiffGreyLevels(vector<unsigned short> vectorOfDiscretizedPixels);
		vector<unsigned int> getHistogram(vector<unsigned short> vectorOfDiscretizedPixels);

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

		float calcMean(vector<unsigned short> vectorOfDiscretizedPixels);
		float calcVariance(vector<unsigned short> vectorOfDiscretizedPixels);
		void calcSkewness();
		void calcKurtosis();
		void getMedian(std::vector<float> vectorMatrElement);
		void getMinimum(vector<float> matrixVector);
		double getPercentile(vector<float> matrixVector, float probability);
		void get10percentile(vector<float> matrixVector);
		void get90percentile(vector<float> matrixVector);
		void getMaximum(vector<float> matrixVector);
		void getInterquartileRange(vector<float> matrixVector);
		void getMode();
		void getRange();
		void meanAbsoluteDev(vector<float> vectorMatrElem);
		void getRobustMeanAbsDev(vector<float> vectorMatrElem);
		void medianAbsoluteDev(vector<float> vectorMatrElem);
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
		void defineFeatureNames(vector<string> &features);
		void extractFeatureValues(vector<float> &intensityHistogramValues);

};