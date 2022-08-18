#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
#include <deque>


class LocalIntensity 
{
	public:
	enum FEATURE
	{
		LOCAL,
		GLOBAL,

		FEATURE_COUNT

	};

	public:	
		LocalIntensity();
		~LocalIntensity();

		void clearVariable();
		void clearVector();

		// set state //
		bool isActivatedFamily = false;					// by. platform main
		std::vector<bool> isCheckedFeature;				// by. platform pop-up
		int nCheckedFeatures;					

		// get histogram //
		int nPixels;
		int nHeight;
		int nWidth;
		float pixelSpacingX;
		float pixelSpacingY;

		// put extracted values in 2d vector //
		std::vector<std::vector<float>> final2DVec;		// slice by slice
		std::vector<float> final1DVec;					// series by series (average)

		// final feature value //
		float localIntensityPeak = NAN;
		float globalIntensityPeak = NAN;

		// calculate feature value //
		std::vector<std::pair<int, int>> getAllMaxIndices(short* pusImage, unsigned char* pucMask);
		void calcLocalIntensityPeak(short* psImage, unsigned char* pucMask);
		void calcGlobalIntensityPeak(short* psImage, unsigned char* pucMask);

		// feature extraction - slice by slice //
		void calcFeature(int FEATURE_IDX, std::vector<float> &tempValues1DVec, short* psImage, unsigned char* pucMask);
		void featureExtraction(short* psImage, unsigned char* pucMask, int nHeight, int nWidth, float pixelSpacingX, float pixelSpacingY);

		// mean all slices - get final feature value //
		void averageAllValues();

		// define and extract calculated values //
		void defineFeatureNames(std::vector<std::string> &features);
		void extractFeatureValues(std::vector<float> &intensityStatisticsValues);

};