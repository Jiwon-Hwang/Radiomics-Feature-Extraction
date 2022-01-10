#include "IntensityHistogram.h"

using namespace std;

IntensityHistogram::IntensityHistogram() {

}
IntensityHistogram::~IntensityHistogram() {

}
void IntensityHistogram::clear() {

	if (!final2DVec.empty()) {
		final2DVec.clear();
	}

	if (!final1DVec.empty()) {
		final1DVec.clear();
	}

	cout << "clear intenseHisto..." << endl;
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

float IntensityHistogram::calcMean() {
	float mean = accumulate(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end(), 0.0) / vectorOfDiscretizedPixels.size(); // 0.0 : initial value of the sum

	return mean;
}
float IntensityHistogram::calcVariance() {
	unsigned int size = vectorOfDiscretizedPixels.size(); // size_t : unsigned int
	float mean = accumulate(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end(), 0.0) / size;
	vector<float> diff(size);
	transform(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end(), diff.begin(), bind2nd(minus<float>(), mean));
	float variance = inner_product(diff.begin(), diff.end(), diff.begin(), 0.0) / size;

	/*
	// with anonymous function : func = [capture](parameters){body}
	auto variance_func = [&mean, &size](float accumulator, const float& val) {
	return accumulator + ((val - mean) * (val - mean) / size);
	};
	variance = accumulate(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end(), 0.0, variance_func);
	*/

	/*
	// with for loop
	float variance = 0;
	for (int n = 0; n < vectorOfDiscretizedPixels.size(); n++)
	{
	variance += (vectorOfDiscretizedPixels[n] - meanValue) * (vectorOfDiscretizedPixels[n] - meanValue);
	}
	variance /= vectorOfDiscretizedPixels.size();
	*/

	return variance;
}
float IntensityHistogram::calcSkewness() {
	float skew = 0;
	float mean = calcMean();
	float var = calcVariance();
	int count = vectorOfDiscretizedPixels.size();

	for (int i = 0; i < count; ++i) {
		skew += pow(vectorOfDiscretizedPixels[i] - mean, 3);
	}
	skew *= sqrt(float(count)) / pow(var*float(count), 1.5);

	return skew;
}
float IntensityHistogram::calcKurtosis() {
	float kurto = 0;
	float mean = calcMean();
	float var = calcVariance();
	int count = vectorOfDiscretizedPixels.size();

	for (int i = 0; i < count; ++i) {
		kurto += pow(vectorOfDiscretizedPixels[i] - mean, 4);
	}
	kurto /= float(count) * pow(var, 2);
	kurto -= 3;

	return kurto;
}
float IntensityHistogram::calcMedian() {
	float median = 0;
	float mean = calcMean();
	float var = calcVariance();
	int count = vectorOfDiscretizedPixels.size();

	return median;
}

void IntensityHistogram::calcFeature(int FEATURE_IDX, vector<float> &tempValues1DVec) {
	
	switch (FEATURE_IDX)
	{
		case MEAN:
			tempValues1DVec.push_back(calcMean());
			break;

		case VARIANCE:
			tempValues1DVec.push_back(calcVariance());
			break;

		case SKEWNESS:
			tempValues1DVec.push_back(calcSkewness());
			break;

		case KURTOSIS:
			tempValues1DVec.push_back(calcKurtosis());
			break;

		default:
			//cout << "error : Unknown Intensity Histogram Feature!" << endl;
			break;
	}
}
void IntensityHistogram::featureExtraction(short* psImage, unsigned char* pucMask, int nHeight, int nWidth) {
	
	// get histogram
	vectorOfOriPixels = getVectorOfPixelsInROI(psImage, pucMask, nHeight, nWidth);
	vectorOfDiscretizedPixels = getVectorOfDiscretizedPixels_nBins(); // �����̽����� �ʱ�ȭ
	hist = getHistogram();

	vector<float> tempValues1DVec; // �����̽����� �ʱ�ȭ

	// calculate checked feature
	for (int i = 0; i < isCheckedFeature.size(); i++) {
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

	// set final feature value => Q. �� ��������� �־���� �ϳ�..? final1DVec�̸� �Ǵµ�..
	/*
	for (int i = 0; i < isCheckedFeature.size(); i++) {
		if (isCheckedFeature[i]) {
			
		}
	}
	*/

}

void IntensityHistogram::defineFeatureNames(vector<string> &features) {
	// �� 23����
	features.push_back("mean");
	features.push_back("variance");
	features.push_back("skewness");
	features.push_back("kurtosis");
	features.push_back("median");
	features.push_back("minimum");
	features.push_back("10th percentile");
	features.push_back("90th percentile");
	features.push_back("maximum");
	features.push_back("Interquartile range");
	features.push_back("mode");
	features.push_back("range");
	features.push_back("Mean absolut deviation");
	features.push_back("Robust mean absolute deviation");
	features.push_back("Median absolut deviation");
	features.push_back("Coefficient of variation");
	features.push_back("Quartile coefficient");
	features.push_back("Entropy");
	features.push_back("Uniformity");
	features.push_back("Maximum histogram gradient");
	features.push_back("Maximum histogram gradient grey level");
	features.push_back("Minimum histogram gradient");
	features.push_back("Minimum histogram gradient grey level");
}
void IntensityHistogram::extractFeatureValues(vector<float> &intensityHistogramValues) {
	
	intensityHistogramValues.push_back(meanValue);
	intensityHistogramValues.push_back(varianceValue);
	intensityHistogramValues.push_back(skewnessValue);
	intensityHistogramValues.push_back(kurtosisValue);
	intensityHistogramValues.push_back(medianValue);
	intensityHistogramValues.push_back(minimumValue);
	intensityHistogramValues.push_back(percentile10);
	intensityHistogramValues.push_back(percentile90);
	intensityHistogramValues.push_back(maximumValue);
	intensityHistogramValues.push_back(interquartileRange);
	intensityHistogramValues.push_back(mode);
	intensityHistogramValues.push_back(rangeValue);
	intensityHistogramValues.push_back(meanAbsDev);
	intensityHistogramValues.push_back(robustMeanAbsDev);
	intensityHistogramValues.push_back(medianAbsDev);
	intensityHistogramValues.push_back(coeffOfVar);
	intensityHistogramValues.push_back(quartileCoeff);
	intensityHistogramValues.push_back(entropy);
	intensityHistogramValues.push_back(histUniformity);
	intensityHistogramValues.push_back(maxHistGradient);
	intensityHistogramValues.push_back(maxHistGradGreyValue);
	intensityHistogramValues.push_back(minHistGradient);
	intensityHistogramValues.push_back(minHistGradGreyValue);

}

