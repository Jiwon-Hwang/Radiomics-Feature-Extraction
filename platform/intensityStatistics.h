#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
#include <deque>


class IntensityStatistics 
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
		RANGE,
		MEANABSDEV,
		ROBUSTMEANABSDEV,
		MEDIANABSDEV,
		COEFFOFVAR,
		QUARTILECOEFF,
		ENERGY,
		ROOTMEANSQUARE,

		FEATURE_COUNT

	};

	public:	
		IntensityStatistics();
		~IntensityStatistics();

		void clearVariable();
		void clearVector();

		// set state //
		bool isActivatedFamily = false;			// by. platform main
		std::vector<bool> isCheckedFeature;		// by. platform pop-up
		int nCheckedFeatures;					

		// get histogram //
		int nBins = 32;
		int nPixels;
		std::vector<short> vectorOfOriPixels;					// sorted
		std::vector<unsigned short> vectorOfDiscretizedPixels;  // sorted
		std::vector<unsigned short> diffGreyLevels;
		std::vector<double> hist;
		std::vector<double> probabilities;
		std::vector<double> histGradient;

		std::vector<short> getVectorOfPixelsInROI(short* psImage, unsigned char* pucMask, int nHeight, int nWidth);
		std::vector<unsigned short> getVectorOfDiscretizedPixels_nBins();
		std::vector<unsigned short> getVectorOfDiffGreyLevels();
		std::vector<double> getHistogram();
		std::vector<double> getProbabilities();
		std::vector<double> getHistGradient();

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
		float rangeValue = NAN;
		float meanAbsDev = NAN;
		float robustMeanAbsDev = NAN;
		float medianAbsDev = NAN;
		float coeffOfVar = NAN;
		float quartileCoeff = NAN;
		float energy = NAN;
		float rootMeanSquare = NAN;

		// calculate feature value //
		void calcMean();
		void calcVariance();
		void calcSkewness();
		void calcKurtosis();
		void calcMedian();
		void calcMinimum();
		unsigned short getPercentile(float probability);
		void calc10percentile();
		void calc90percentile();
		void calcMaximum();
		void calcInterquartileRange();
		void calcRange();
		void calcMeanAbsoluteDev();
		void getSmallerElements(std::vector<float> &vec, float max);
		void getGreaterElements(std::vector<float> &vec, float min);
		void calcRobustMeanAbsDev();
		void calcMedianAbsoluteDev();
		void calcCoeffOfVar();
		void calcQuartileCoeff();
		void calcEnergy();
		void calcRootMeanSquare();

		// feature extraction - slice by slice //
		void calcFeature(int FEATURE_IDX, std::vector<float> &tempValues1DVec);
		void featureExtraction(short* psImage, unsigned char* pucMask, int nHeight, int nWidth);

		// mean all slices - get final feature value //
		void averageAllValues();

		// define and extract calculated values //
		void defineFeatureNames(std::vector<std::string> &features);
		void extractFeatureValues(std::vector<float> &intensityHistogramValues);

};