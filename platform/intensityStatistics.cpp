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
vector<unsigned short> IntensityStatistics::getVectorOfDiscretizedPixels_nBins() {

	float min = (float)*min_element(vectorOfOriPixels.begin(), vectorOfOriPixels.end()); // min_element() : pointer return
	float max = (float)*max_element(vectorOfOriPixels.begin(), vectorOfOriPixels.end());
	vector<float> tempFloatVec(vectorOfOriPixels.begin(), vectorOfOriPixels.end());

	if (min == 0 && min == max) {
		cout << "error in calculating discretization, VOI contains only 0" << endl;
		exit(0);
	}
	else if (min > 0 && min == max) {
		cout << "error in calculating discretization, VOI contains only one intensity value, minimum value is set to 0" << endl;
		min = 0;
	}

	//subtract minimum value from every matrix element
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(minus<float>(), min)); // minus<T> : 이항 연산 함수 객체 (-) / bind2nd : 2번째 인수를 고정해 함수 객체로 변환 / transform : 일괄 연산

	//get the range
	float range = (max - min) / nBins; // range : 몫 (width of a bin) => ***float이 들어가면 / 는 몫이 아니라 진짜 "나누기" 연산!!!***

	//divide every element of the matrix by the range
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(divides<float>(), range));

	//replace 0 to 1 => ibsi 핑크색 마킹 부분! (min - min = 0이 되므로 최소인 1로 바꿔줘야 뒤에 ceil에서 min value of each bin : 1 유지)
	replace(tempFloatVec.begin(), tempFloatVec.end(), 0, 1);

	//do rounding
	for (int i = 0; i < tempFloatVec.size(); i++) {
		tempFloatVec[i] = ceil(tempFloatVec[i]);
	}

	//type casting
	vector<unsigned short> vectorOfDiscretizedPixels(tempFloatVec.begin(), tempFloatVec.end()); // 양자화한 최종 결과 픽셀값들 담을 벡터

	sort(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end());

	return vectorOfDiscretizedPixels;
}
vector<unsigned short> IntensityStatistics::getVectorOfDiffGreyLevels() {

	vector<unsigned short> diffGreyLevels(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end()); // 1~nbins 사이 값으로 양자화된 픽셀값들
	diffGreyLevels.erase(unique(diffGreyLevels.begin(), diffGreyLevels.end()), diffGreyLevels.end()); // size : 168 => 32 bins로 나누면 한 구간 당 약 5개의 픽셀값들의 빈도수들 누적

	return diffGreyLevels;
}
vector<double> IntensityStatistics::getHistogram() {

	vector<double> histVec;
	unsigned int nCnt;
	unsigned short greyLevel;
	for (int i = 0; i < diffGreyLevels.size(); i++) {
		greyLevel = diffGreyLevels[i];
		nCnt = 0;
		for (int j = 0; j < vectorOfDiscretizedPixels.size(); j++) {
			if (vectorOfDiscretizedPixels[j] == greyLevel) {
				nCnt += 1;
			}
		}
		histVec.push_back(nCnt);
	}

	return histVec;
}
vector<double> IntensityStatistics::getProbabilities() {

	vector<double> probabilities(hist.begin(), hist.end()); // 둘다 size : nBins
	transform(probabilities.begin(), probabilities.end(), probabilities.begin(), bind2nd(divides<float>(), nPixels));

	return probabilities;
}
vector<double> IntensityStatistics::getHistGradient() {

	vector<double> histGradient;
	double actualGradient;
	
	for (int i = 1; i < probabilities.size(); i++) {
		if (i == 1) {
			actualGradient = (hist[i] - hist[i - 1]);
		}
		else if (i == probabilities.size() - 1) {
			actualGradient = (hist[i] - hist[i - 1]);
		}
		else {
			actualGradient = (hist[i + 1] - hist[i - 1]) / 2;
		}
		histGradient.push_back(actualGradient);
	}

	histGradient.push_back((hist[probabilities.size() - 1] - hist[probabilities.size() - 2]));

	return histGradient;
}

void IntensityStatistics::calcMean() {

	meanValue = accumulate(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end(), 0.0) / nPixels; // 0.0 : initial value of the sum

}
void IntensityStatistics::calcVariance() {

	unsigned int size = nPixels; // size_t : unsigned int

	if (isnan(meanValue)) {
		calcMean();
	}
		
	vector<float> diff(size);
	transform(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end(), diff.begin(), bind2nd(minus<float>(), meanValue));
	varianceValue = inner_product(diff.begin(), diff.end(), diff.begin(), 0.0) / size;

	/*
	// with anonymous function : func = [capture](parameters){body}
	auto variance_func = [&mean, &size](float accumulator, const float& val) {
	return accumulator + ((val - meanValue) * (val - meanValue) / size);
	};
	variance = accumulate(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end(), 0.0, variance_func);
	*/

	/*
	// with for loop
	float variance = 0;
	for (int n = 0; n < nPixels; n++)
	{
	variance += (vectorOfDiscretizedPixels[n] - meanValue) * (vectorOfDiscretizedPixels[n] - meanValue);
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
		skewnessValue += pow(vectorOfDiscretizedPixels[i] - meanValue, 3);
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
		kurtosisValue += pow(vectorOfDiscretizedPixels[i] - meanValue, 4);
	}
	kurtosisValue /= float(nPixels) * pow(varianceValue, 2);
	kurtosisValue -= 3;

}
void IntensityStatistics::calcMedian() {

	int medianIdx = nPixels / 2; // 몫
	medianValue = vectorOfDiscretizedPixels[medianIdx];

}
void IntensityStatistics::calcMinimum() {

	minimumValue = vectorOfDiscretizedPixels.front();

}
unsigned short IntensityStatistics::getPercentile(float probability){

	int percentileIdx = int(probability * nPixels);

	return vectorOfDiscretizedPixels[percentileIdx];
}
void IntensityStatistics::calc10percentile() {

	percentile10 = getPercentile(0.1);

}
void IntensityStatistics::calc90percentile() {

	percentile90 = getPercentile(0.9);

}
void IntensityStatistics::calcMaximum() {

	maximumValue = vectorOfDiscretizedPixels.back();

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

	vector<float> tempVector(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end());
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

	vector<float> tempVector(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end());
	
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

	vector<float> tempVector(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end());
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
			
			break;

		case ROOTMEANSQUARE:
			
			break;
			
		default:
			break;
	}
}
void IntensityStatistics::featureExtraction(short* psImage, unsigned char* pucMask, int nHeight, int nWidth) {
	
	// claer all values
	clearVariable(); // 슬라이스마다 초기화

	// get histogram and etc
	vectorOfOriPixels = getVectorOfPixelsInROI(psImage, pucMask, nHeight, nWidth);
	vectorOfDiscretizedPixels = getVectorOfDiscretizedPixels_nBins(); // 슬라이스마다 초기화
	nPixels = vectorOfDiscretizedPixels.size();
	diffGreyLevels = getVectorOfDiffGreyLevels();
	hist = getHistogram();
	probabilities = getProbabilities();
	histGradient = getHistGradient();


	vector<float> tempValues1DVec; // 슬라이스마다 초기화

	// calculate checked feature
	for (int i = 0; i < FEATURE_COUNT; i++) {
		if (isCheckedFeature[i]) calcFeature(i, tempValues1DVec);
	}

	final2DVec.push_back(tempValues1DVec); // 모든 ROI 슬라이스 들어올 때까지 누적 (시리즈마다 초기화)

}

void IntensityStatistics::averageAllValues() {
	
	// get final mean vector
	for (int col = 0; col < final2DVec[0].size(); col++) {
		float colSum = 0;
		float colMean;

		for (int row = 0; row < final2DVec.size(); row++) {
			colSum += final2DVec[row][col];
		}
		colMean = colSum / final2DVec.size();
		final1DVec.push_back(colMean);
	}

}

void IntensityStatistics::defineFeatureNames(vector<string> &features) {
	// 총 23가지
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
void IntensityStatistics::extractFeatureValues(vector<float> &intensityHistogramValues) {
	// platform.cpp의 writeCSVCheckedValue()에서 참조 가능. But, 속도 문제로 사용 x 
	intensityHistogramValues[MEAN] = meanValue;
	intensityHistogramValues[VARIANCE] = varianceValue;
	intensityHistogramValues[SKEWNESS] = skewnessValue;
	intensityHistogramValues[KURTOSIS] = kurtosisValue;
	intensityHistogramValues[MEDIAN] = medianValue;
	intensityHistogramValues[MINIMUM] = minimumValue;
	intensityHistogramValues[PERCENTILE10] = percentile10;
	intensityHistogramValues[PERCENTILE90] = percentile90;
	intensityHistogramValues[MAXIMUM] = maximumValue;
	intensityHistogramValues[INTERQUARTILERANGE] = interquartileRange;
	intensityHistogramValues[RANGE] = rangeValue;
	intensityHistogramValues[MEANABSDEV] = meanAbsDev;
	intensityHistogramValues[ROBUSTMEANABSDEV] = robustMeanAbsDev;
	intensityHistogramValues[MEDIANABSDEV] = medianAbsDev;
	intensityHistogramValues[COEFFOFVAR] = coeffOfVar;
	intensityHistogramValues[QUARTILECOEFF] = quartileCoeff;
	intensityHistogramValues[ENERGY] = energy;
	intensityHistogramValues[ROOTMEANSQUARE] = rootMeanSquare;

}

