#include "IntensityHistogram.h"

using namespace std;

IntensityHistogram::IntensityHistogram() {

}
IntensityHistogram::~IntensityHistogram() {

}
void IntensityHistogram::clearVariable() {

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
	mode = NAN;
	rangeValue = NAN;
	meanAbsDev = NAN;
	robustMeanAbsDev = NAN;
	medianAbsDev = NAN;
	coeffOfVar = NAN;
	quartileCoeff = NAN;
	entropy = NAN;
	histUniformity = NAN;
	maxHistGradient = NAN;
	maxHistGradGreyValue = NAN;
	minHistGradient = NAN;
	minHistGradGreyValue = NAN;
	//percentile25 = NAN;
	//percentile75 = NAN;
	//vector<float> maxHistVecGradient;
	//vector<float> minHistVecGradient;

}
void IntensityHistogram::clearVector() {

	if (!final2DVec.empty()) {
		final2DVec.clear();
	}

	if (!final1DVec.empty()) {
		final1DVec.clear();
	}

}

vector<short> IntensityHistogram::getVectorOfPixelsInROI(short* psImage, unsigned char* pucMask, int nHeight, int nWidth) {

	vector<short> vectorOfOriPixels; // size : 3032
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			short imageValue = psImage[index];

			// ROI ���ΰ� push (ROI�� ������ ��� pass)
			if (maskValue >(unsigned char)0) {
				vectorOfOriPixels.push_back(imageValue);
			}
		}
	}

	sort(vectorOfOriPixels.begin(), vectorOfOriPixels.end()); // min(front) : -1, max(back) : 180

	return vectorOfOriPixels;
}
vector<unsigned short> IntensityHistogram::getVectorOfDiscretizedPixels_nBins() {

	float minimumValue = (float)*min_element(vectorOfOriPixels.begin(), vectorOfOriPixels.end()); // min_element() : pointer return
	float maximumValue = (float)*max_element(vectorOfOriPixels.begin(), vectorOfOriPixels.end());
	vector<float> tempFloatVec(vectorOfOriPixels.begin(), vectorOfOriPixels.end());

	if (minimumValue == 0 && minimumValue == maximumValue) {
		cout << "error in calculating discretization, VOI contains only 0" << endl;
		exit(0);
	}
	else if (minimumValue > 0 && minimumValue == maximumValue) {
		cout << "error in calculating discretization, VOI contains only one intensity value, minimum value is set to 0" << endl;
		minimumValue = 0;
	}

	//subtract minimum value from every matrix element
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(minus<float>(), minimumValue)); // minus<T> : ���� ���� �Լ� ��ü (-) / bind2nd : 2��° �μ��� ������ �Լ� ��ü�� ��ȯ / transform : �ϰ� ����

																													  //get the range
	float range = (maximumValue - minimumValue) / nBins; // range : �� (width of a bin) => ***float�� ���� / �� ���� �ƴ϶� ��¥ "������" ����!!!***

	//divide every element of the matrix by the range
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(divides<float>(), range));

	//replace 0 to 1 => ibsi ��ũ�� ��ŷ �κ�! (min - min = 0�� �ǹǷ� �ּ��� 1�� �ٲ���� �ڿ� ceil���� min value of each bin : 1 ����)
	replace(tempFloatVec.begin(), tempFloatVec.end(), 0, 1);

	//do rounding
	for (int i = 0; i < tempFloatVec.size(); i++) {
		tempFloatVec[i] = ceil(tempFloatVec[i]);
	}

	//type casting
	vector<unsigned short> vectorOfDiscretizedPixels(tempFloatVec.begin(), tempFloatVec.end()); // ����ȭ�� ���� ��� �ȼ����� ���� ����

	return vectorOfDiscretizedPixels;
}
vector<unsigned short> IntensityHistogram::getVectorOfDiffGreyLevels() {

	vector<unsigned short> diffGreyLevels(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end()); // 1~nbins ���� ������ ����ȭ�� �ȼ�����
	diffGreyLevels.erase(unique(diffGreyLevels.begin(), diffGreyLevels.end()), diffGreyLevels.end()); // size : 168 => 32 bins�� ������ �� ���� �� �� 5���� �ȼ������� �󵵼��� ����

	return diffGreyLevels;
}
vector<unsigned int> IntensityHistogram::getHistogram() {

	vector<unsigned short> diffGreyLevels = getVectorOfDiffGreyLevels();

	vector<unsigned int> hist;
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
		hist.push_back(nCnt);
	}

	return hist;
}

void IntensityHistogram::calcMean() {

	meanValue = accumulate(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end(), 0.0) / nPixels; // 0.0 : initial value of the sum

}
void IntensityHistogram::calcVariance() {

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
void IntensityHistogram::calcSkewness() {

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
void IntensityHistogram::calcKurtosis() {
	
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
void IntensityHistogram::calcMedian() {

}

void IntensityHistogram::calcFeature(int FEATURE_IDX, vector<float> &tempValues1DVec) {
	
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

		default:
			//cout << "error : Unknown Intensity Histogram Feature!" << endl;
			break;
	}
}
void IntensityHistogram::featureExtraction(short* psImage, unsigned char* pucMask, int nHeight, int nWidth) {
	
	// claer all values
	clearVariable(); // �����̽����� �ʱ�ȭ

	// get histogram
	vectorOfOriPixels = getVectorOfPixelsInROI(psImage, pucMask, nHeight, nWidth);
	vectorOfDiscretizedPixels = getVectorOfDiscretizedPixels_nBins(); // �����̽����� �ʱ�ȭ
	nPixels = vectorOfDiscretizedPixels.size();
	hist = getHistogram();

	vector<float> tempValues1DVec; // �����̽����� �ʱ�ȭ

	// calculate checked feature
	for (int i = 0; i < FEATURE_COUNT; i++) {
		if (isCheckedFeature[i]) calcFeature(i, tempValues1DVec);
	}

	final2DVec.push_back(tempValues1DVec); // ��� ROI �����̽� ���� ������ ���� (�ø���� �ʱ�ȭ)

}

void IntensityHistogram::averageAllValues() {
	
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

void IntensityHistogram::defineFeatureNames(vector<string> &features) {
	// �� 23����
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
	features[MODE] = "mode";
	features[RANGE] = "range";
	features[MEANABSDEV] = "Mean absolut deviation";
	features[ROBUSTMEANABSDEV] = "Robust mean absolute deviation";
	features[MEDIANABSDEV] = "Median absolut deviation";
	features[COEFFOFVAR] = "Coefficient of variation";
	features[QUARTILECOEFF] = "Quartile coefficient";
	features[ENTROPY] = "Entropy";
	features[UNIFORMITY] = "Uniformity";
	features[MAXHISTGRADIENT] = "Maximum histogram gradient";
	features[MAXHISTGRADGREY] = "Maximum histogram gradient grey level";
	features[MINHISTGRADIENT] = "Minimum histogram gradient";
	features[MINHISTGRADGREY] = "Minimum histogram gradient grey level";

}
void IntensityHistogram::extractFeatureValues(vector<float> &intensityHistogramValues) {
	// platform.cpp�� writeCSVCheckedValue()���� ���� ����. But, �ӵ� ������ ��� x 
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
	intensityHistogramValues[MODE] = mode;
	intensityHistogramValues[RANGE] = rangeValue;
	intensityHistogramValues[MEANABSDEV] = meanAbsDev;
	intensityHistogramValues[ROBUSTMEANABSDEV] = robustMeanAbsDev;
	intensityHistogramValues[MEDIANABSDEV] = medianAbsDev;
	intensityHistogramValues[COEFFOFVAR] = coeffOfVar;
	intensityHistogramValues[QUARTILECOEFF] = quartileCoeff;
	intensityHistogramValues[ENTROPY] = entropy;
	intensityHistogramValues[UNIFORMITY] = histUniformity;
	intensityHistogramValues[MAXHISTGRADIENT] = maxHistGradient;
	intensityHistogramValues[MAXHISTGRADGREY] = maxHistGradGreyValue;
	intensityHistogramValues[MINHISTGRADIENT] = minHistGradient;
	intensityHistogramValues[MINHISTGRADGREY] = minHistGradGreyValue;

}

