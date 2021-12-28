#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>


// enum, struct, map 고려! => #define은 헤더파일 범위 상관없이 전역에서 적용!
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


//using namespace std; // 헤더에는 쓰는 것 X (cpp에 쓰기)


class IntensityHistogram 
{
private:


public:	
	IntensityHistogram();
	~IntensityHistogram();

	// bool isActivated;
	vector<bool> isChecked;

	vector<unsigned short> vectorOfDiscretizedPixels;
	vector<unsigned int> hist;

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

		
	void calcMean(vector<float> vectorMatrElem);
	void calcVariance();
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


	void defineFeatures(vector<string> &features);
	void extractData(IntensityHistogram intenseHisto, vector<float> &intensityHistogramValues);

};