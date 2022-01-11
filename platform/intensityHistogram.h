#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>




class IntensityHistogram 
{
	public:
	enum FEATURE
	{
		MEAN,
		VARIANCE,
		SKEWNESS,
		KURTOSIS,
		MEDIAN,
		MINIMUM,
		PERCENTILE10,
		PERCENTILE90,
		MAXIMUM,
		INTERQUARTILERANGE,
		MODE,
		RANGE,
		MEANABSDEV,
		ROBUSTMEANABSDEV,
		MEDIANABSDEV,
		COEFFOFVAR,
		QUARTILECOEFF,
		ENTROPY,
		UNIFORMITY,
		MAXHISTGRADIENT,
		MAXHISTGRADGREY,
		MINHISTGRADIENT,
		MINHISTGRADGREY,

		FEATURE_COUNT

	};

	public:	
		IntensityHistogram();
		~IntensityHistogram();

		void clearVariable();
		void clearVector();

		// set state //
		bool isActivatedFamily = false;			// by. platform main
		std::vector<bool> isCheckedFeature;		// by. platform pop-up
		int nCheckedFeatures;					// **추후 위의 isCheckedFeature 벡터로부터 TRUE 개수 구하기**

		// get histogram //
		int nBins = 32;
		int nPixels;
		std::vector<short> vectorOfOriPixels;
		std::vector<unsigned short> vectorOfDiscretizedPixels;
		std::vector<unsigned int> hist;

		std::vector<short> getVectorOfPixelsInROI(short* psImage, unsigned char* pucMask, int nHeight, int nWidth);
		std::vector<unsigned short> getVectorOfDiscretizedPixels_nBins();
		std::vector<unsigned short> getVectorOfDiffGreyLevels();
		std::vector<unsigned int> getHistogram();

		// put extracted values in 2d vector //
		std::vector<std::vector<float>> final2DVec;		// slice by slice
		std::vector<float> final1DVec;					// series by series (average)

		// final feature value //
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

		// calculate feature value //
		void calcMean();
		void calcVariance();
		void calcSkewness();
		void calcKurtosis();
		void calcMedian();
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

		// feature extraction - slice by slice //
		void calcFeature(int FEATURE_IDX, std::vector<float> &tempValues1DVec);
		void featureExtraction(short* psImage, unsigned char* pucMask, int nHeight, int nWidth);

		// mean all slices - get final feature value //
		void averageAllValues();

		// define and extract calculated values //
		void defineFeatureNames(std::vector<std::string> &features);
		void extractFeatureValues(std::vector<float> &intensityHistogramValues);

};