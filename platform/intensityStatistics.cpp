#include "IntensityStatistics.h"

using namespace std;

IntensityStatistics::IntensityStatistics() {

}
IntensityStatistics::~IntensityStatistics() {

}
void IntensityStatistics::clearVariable() {

	meanValue = NAN;
	varianceValue = NAN;
	skewnessValue = NAN;
	kurtosisValue = NAN;
	medianValue = NAN;
	minimumValue = NAN;
	percentile10 = NAN;
	percentile90 = NAN;
	maximumValue = NAN;
	interquartileRange = NAN;
	rangeValue = NAN;
	meanAbsDev = NAN;
	robustMeanAbsDev = NAN;
	medianAbsDev = NAN;
	coeffOfVar = NAN;
	quartileCoeff = NAN;
	energy = NAN;
	rootMeanSquare = NAN;

}
void IntensityStatistics::clearVector() {

	if (!final2DVec.empty()) {
		final2DVec.clear();
	}

	if (!final1DVec.empty()) {
		final1DVec.clear();
	}

}

vector<short> IntensityStatistics::getVectorOfPixelsInROI(short* psImage, unsigned char* pucMask, int nHeight, int nWidth) {

	vector<short> vectorOfOriPixels; // size : 3032
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			short imageValue = psImage[index];

			// ROI 내부값 push (ROI가 없으면 계속 pass)
			if (maskValue >(unsigned char)0) {
				vectorOfOriPixels.push_back(imageValue);
			}
		}
	}

	sort(vectorOfOriPixels.begin(), vectorOfOriPixels.end()); // min(front) : -1, max(back) : 180
	return vectorOfOriPixels;
}

void IntensityStatistics::calcMean() {

	meanValue = accumulate(vectorOfOriPixels.begin(), vectorOfOriPixels.end(), 0.0) / nPixels; // 0.0 : initial value of the sum

}
void IntensityStatistics::calcVariance() {

	unsigned int size = nPixels; // size_t : unsigned int

	if (isnan(meanValue)) {
		calcMean();
	}
		
	vector<float> diff(size);
	transform(vectorOfOriPixels.begin(), vectorOfOriPixels.end(), diff.begin(), bind2nd(minus<float>(), meanValue));
	varianceValue = inner_product(diff.begin(), diff.end(), diff.begin(), 0.0) / size;

	/*
	// with anonymous function : func = [capture](parameters){body}
	auto variance_func = [&mean, &size](float accumulator, const float& val) {
	return accumulator + ((val - meanValue) * (val - meanValue) / size);
	};
	variance = accumulate(vectorOfOriPixels.begin(), vectorOfOriPixels.end(), 0.0, variance_func);
	*/

	/*
	// with for loop
	float variance = 0;
	for (int n = 0; n < nPixels; n++)
	{
	variance += (vectorOfOriPixels[n] - meanValue) * (vectorOfOriPixels[n] - meanValue);
	}
	variance /= nPixels;
	*/
}
void IntensityStatistics::calcSkewness() {

	skewnessValue = 0;

	if (isnan(meanValue)) {
		calcMean();
	}

	if (isnan(varianceValue)) {
		calcVariance();
	}

	for (int i = 0; i < nPixels; ++i) {
		skewnessValue += pow(vectorOfOriPixels[i] - meanValue, 3);
	}
	skewnessValue *= sqrt(float(nPixels)) / pow(varianceValue*float(nPixels), 1.5);

}
void IntensityStatistics::calcKurtosis() {
	
	kurtosisValue = 0;

	if (isnan(meanValue)) {
		calcMean();
	}

	if (isnan(varianceValue)) {
		calcVariance();
	}

	for (int i = 0; i < nPixels; ++i) {
		kurtosisValue += pow(vectorOfOriPixels[i] - meanValue, 4);
	}
	kurtosisValue /= float(nPixels) * pow(varianceValue, 2);
	kurtosisValue -= 3;

}
void IntensityStatistics::calcMedian() {

	int medianIdx = nPixels / 2; // 몫
	medianValue = vectorOfOriPixels[medianIdx];

}
void IntensityStatistics::calcMinimum() {

	minimumValue = vectorOfOriPixels.front();

}
short IntensityStatistics::getPercentile(float probability){

	int percentileIdx = int(probability * nPixels);

	return vectorOfOriPixels[percentileIdx];
}
void IntensityStatistics::calc10percentile() {

	percentile10 = getPercentile(0.1);

}
void IntensityStatistics::calc90percentile() {

	percentile90 = getPercentile(0.9);
}
void IntensityStatistics::calcMaximum() {

	maximumValue = vectorOfOriPixels.back();

}
void IntensityStatistics::calcInterquartileRange() {

	interquartileRange = getPercentile(0.75) - getPercentile(0.25);

}
void IntensityStatistics::calcRange() {

	if (isnan(maximumValue)) {
		calcMaximum();
	}

	if (isnan(minimumValue)) {
		calcMinimum();
	}

	rangeValue = maximumValue - minimumValue;

}
void IntensityStatistics::calcMeanAbsoluteDev() {

	if (isnan(meanValue)) {
		calcMean();
	}

	vector<float> tempVector(vectorOfOriPixels.begin(), vectorOfOriPixels.end());
	transform(tempVector.begin(), tempVector.end(), tempVector.begin(), bind2nd(minus<float>(), meanValue));

	meanAbsDev = 0;
	for (int i = 0; i < nPixels; i++) {
		meanAbsDev += abs(tempVector[i]);
	}
	meanAbsDev /= nPixels;

}
// LNK2005 error 방지 멤버함수로 등록
void IntensityStatistics::getSmallerElements(vector<float> &vec, float max) {

	vector<float>::iterator lessThan;
	lessThan = remove_if(vec.begin(), vec.end(), bind2nd(less<float>(), max));
	vec.erase(lessThan, vec.end());

}
void IntensityStatistics::getGreaterElements(vector<float> &vec, float min) {
	
	vector<float>::iterator greaterThan;
	greaterThan = remove_if(vec.begin(), vec.end(), bind2nd(greater<float>(), min));
	vec.erase(greaterThan, vec.end());

}
void IntensityStatistics::calcRobustMeanAbsDev() {
	
	if (isnan(percentile10)) {
		calc10percentile();
	}

	if (isnan(percentile90)) {
		calc90percentile();
	}

	vector<float> tempVector(vectorOfOriPixels.begin(), vectorOfOriPixels.end());
	
	getSmallerElements(tempVector, percentile10);
	getGreaterElements(tempVector, percentile90);

	float tempMeanValue = accumulate(tempVector.begin(), tempVector.end(), 0.0) / tempVector.size(); 
	transform(tempVector.begin(), tempVector.end(), tempVector.begin(), bind2nd(minus<float>(), tempMeanValue));

	robustMeanAbsDev = 0;
	for (int i = 0; i < tempVector.size(); i++) {
		robustMeanAbsDev += abs(tempVector[i]);
	}
	robustMeanAbsDev /= tempVector.size();

}
void IntensityStatistics::calcMedianAbsoluteDev() {

	if (isnan(medianValue)) {
		calcMedian();
	}

	vector<float> tempVector(vectorOfOriPixels.begin(), vectorOfOriPixels.end());
	transform(tempVector.begin(), tempVector.end(), tempVector.begin(), bind2nd(minus<float>(), medianValue));

	medianAbsDev = 0;
	for (int i = 0; i < nPixels; i++) {
		medianAbsDev += abs(tempVector[i]);
	}
	medianAbsDev /= nPixels;

}
void IntensityStatistics::calcCoeffOfVar() {

	if (isnan(varianceValue)) {
		calcVariance();
	}

	if (isnan(meanValue)) {
		calcMean();
	}

	coeffOfVar = pow(varianceValue, 0.5) / meanValue;

}
void IntensityStatistics::calcQuartileCoeff() {

	unsigned short percentile75 = getPercentile(0.75);
	unsigned short percentile25 = getPercentile(0.25);
	quartileCoeff = (float)(percentile75 - percentile25) / (float)(percentile75 + percentile25); // float, int 나눗셈 주의

}
void IntensityStatistics::calcEnergy() {

	energy = 0;
	for (int i = 0; i < nPixels; i++) {
		energy += pow(vectorOfOriPixels[i], 2);
	}

}
void IntensityStatistics::calcRootMeanSquare() {

	if (isnan(energy)) {
		calcEnergy();
	}

	rootMeanSquare = sqrt(energy / nPixels);

}

void IntensityStatistics::calcFeature(int FEATURE_IDX, vector<float> &tempValues1DVec) {
	
	switch (FEATURE_IDX)
	{
		case MEAN:
			calcMean();
			tempValues1DVec.push_back(meanValue);
			break;

		case VARIANCE:
			calcVariance();
			tempValues1DVec.push_back(varianceValue);
			break;

		case SKEWNESS:
			calcSkewness();
			tempValues1DVec.push_back(skewnessValue);
			break;

		case KURTOSIS:
			calcKurtosis();
			tempValues1DVec.push_back(kurtosisValue);
			break;

		case MEDIAN:
			calcMedian();
			tempValues1DVec.push_back(medianValue);
			break;
		
		case MINIMUM:
			calcMinimum();
			tempValues1DVec.push_back(minimumValue);
			break;

		case PERCENTILE10:
			calc10percentile();
			tempValues1DVec.push_back(percentile10);
			break;

		case PERCENTILE90:
			calc90percentile();
			tempValues1DVec.push_back(percentile90);
			break;

		case MAXIMUM:
			calcMaximum();
			tempValues1DVec.push_back(maximumValue);
			break;

		case INTERQUARTILERANGE:
			calcInterquartileRange();
			tempValues1DVec.push_back(interquartileRange);
			break;

		case RANGE:
			calcRange();
			tempValues1DVec.push_back(rangeValue);
			break;

		case MEANABSDEV:
			calcMeanAbsoluteDev();
			tempValues1DVec.push_back(meanAbsDev);
			break;
			
		case ROBUSTMEANABSDEV:
			calcRobustMeanAbsDev();
			tempValues1DVec.push_back(robustMeanAbsDev);
			break;
			
		case MEDIANABSDEV:
			calcMedianAbsoluteDev();
			tempValues1DVec.push_back(medianAbsDev);
			break;

		case COEFFOFVAR:
			calcCoeffOfVar();
			tempValues1DVec.push_back(coeffOfVar);
			break;

		case QUARTILECOEFF:
			calcQuartileCoeff();
			tempValues1DVec.push_back(quartileCoeff);
			break;

		case ENERGY:
			calcEnergy();
			tempValues1DVec.push_back(energy);
			break;

		case ROOTMEANSQUARE:
			calcRootMeanSquare();
			tempValues1DVec.push_back(rootMeanSquare);
			break;
			
		default:
			break;
	}
}
void IntensityStatistics::featureExtraction(short* psImage, unsigned char* pucMask, int nHeight, int nWidth) {
	
	// claer all values
	clearVariable(); // 슬라이스마다 초기화

	// get histogram and etc
	vectorOfOriPixels = getVectorOfPixelsInROI(psImage, pucMask, nHeight, nWidth); // 슬라이스마다 초기화
	nPixels = vectorOfOriPixels.size();

	vector<float> tempValues1DVec; // 슬라이스마다 초기화

	// calculate checked feature
	for (int i = 0; i < FEATURE_COUNT; i++) {
		if (isCheckedFeature[i]) calcFeature(i, tempValues1DVec);
	}

	final2DVec.push_back(tempValues1DVec); // 모든 ROI 슬라이스 들어올 때까지 누적 (시리즈마다 초기화)

}

void IntensityStatistics::averageAllValues() {
	
	// get final mean vector
	for (int col = 0, nCols = final2DVec[0].size(); col < nCols; col++) {
		float colSum = 0;
		float colMean;

		for (int row = 0, nRows = final2DVec.size(); row < nRows; row++) {
			colSum += final2DVec[row][col];
		}
		colMean = colSum / final2DVec.size();
		final1DVec.push_back(colMean);
	}

}

void IntensityStatistics::defineFeatureNames(vector<string> &features) {
	
	features[MEAN] = "mean";
	features[VARIANCE] = "variance";
	features[SKEWNESS] = "skewness";
	features[KURTOSIS] = "kurtosis";
	features[MEDIAN] = "median";
	features[MINIMUM] = "minimum";
	features[PERCENTILE10] = "10th percentile";
	features[PERCENTILE90] = "90th percentile";
	features[MAXIMUM] = "maximum";
	features[INTERQUARTILERANGE] = "Interquartile range";
	features[RANGE] = "range";
	features[MEANABSDEV] = "Mean absolute deviation";
	features[ROBUSTMEANABSDEV] = "Robust mean absolute deviation";
	features[MEDIANABSDEV] = "Median absolut deviation";
	features[COEFFOFVAR] = "Coefficient of variation";
	features[QUARTILECOEFF] = "Quartile coefficient";
	features[ENERGY] = "Energy";
	features[ROOTMEANSQUARE] = "Root mean square";

}
void IntensityStatistics::extractFeatureValues(vector<float> &intensityStatisticsValues) {
	// platform.cpp의 writeCSVCheckedValue()에서 참조 가능. But, 속도 문제로 사용 x 
	intensityStatisticsValues[MEAN] = meanValue;
	intensityStatisticsValues[VARIANCE] = varianceValue;
	intensityStatisticsValues[SKEWNESS] = skewnessValue;
	intensityStatisticsValues[KURTOSIS] = kurtosisValue;
	intensityStatisticsValues[MEDIAN] = medianValue;
	intensityStatisticsValues[MINIMUM] = minimumValue;
	intensityStatisticsValues[PERCENTILE10] = percentile10;
	intensityStatisticsValues[PERCENTILE90] = percentile90;
	intensityStatisticsValues[MAXIMUM] = maximumValue;
	intensityStatisticsValues[INTERQUARTILERANGE] = interquartileRange;
	intensityStatisticsValues[RANGE] = rangeValue;
	intensityStatisticsValues[MEANABSDEV] = meanAbsDev;
	intensityStatisticsValues[ROBUSTMEANABSDEV] = robustMeanAbsDev;
	intensityStatisticsValues[MEDIANABSDEV] = medianAbsDev;
	intensityStatisticsValues[COEFFOFVAR] = coeffOfVar;
	intensityStatisticsValues[QUARTILECOEFF] = quartileCoeff;
	intensityStatisticsValues[ENERGY] = energy;
	intensityStatisticsValues[ROOTMEANSQUARE] = rootMeanSquare;

}

