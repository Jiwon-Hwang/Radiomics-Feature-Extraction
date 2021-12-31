#include "IntensityHistogram.h"

using namespace std;

IntensityHistogram::IntensityHistogram() {

}
IntensityHistogram::~IntensityHistogram() {

}
void IntensityHistogram::clear() {

	if (!tempValues2DVec.empty()) {
		tempValues2DVec.clear();
	}

	if (!final1DVec.empty()) {
		final1DVec.clear();
	}

	cout << "clear intenseHisto's all vectors of this series!" << endl;
}

vector<short> IntensityHistogram::getVectorOfPixelsInROI(short* psImage, unsigned char* pucMask, int nHeight, int nWidth) {

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
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(minus<float>(), minimumValue)); // minus<T> : 이항 연산 함수 객체 (-) / bind2nd : 2번째 인수를 고정해 함수 객체로 변환 / transform : 일괄 연산

																													  //get the range
	float range = (maximumValue - minimumValue) / nBins; // range : 몫 (width of a bin) => ***float이 들어가면 / 는 몫이 아니라 진짜 "나누기" 연산!!!***

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

	return vectorOfDiscretizedPixels;
}
vector<unsigned short> IntensityHistogram::getVectorOfDiffGreyLevels() {

	vector<unsigned short> diffGreyLevels(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end()); // 1~nbins 사이 값으로 양자화된 픽셀값들
	diffGreyLevels.erase(unique(diffGreyLevels.begin(), diffGreyLevels.end()), diffGreyLevels.end()); // size : 168 => 32 bins로 나누면 한 구간 당 약 5개의 픽셀값들의 빈도수들 누적

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

float IntensityHistogram::calcMean(vector<unsigned short> vectorOfDiscretizedPixels) {
	float mean = accumulate(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end(), 0.0) / vectorOfDiscretizedPixels.size(); // 0.0 : initial value of the sum

	return mean;
}
float IntensityHistogram::calcVariance(vector<unsigned short> vectorOfDiscretizedPixels) {
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

void IntensityHistogram::calcFeature(int FEATURE_IDX, vector<float> &tempValues1DVec) {
	
	switch (FEATURE_IDX)
	{
		case MEAN:
			tempValues1DVec.push_back(calcMean(vectorOfDiscretizedPixels));
			break;

		case VARIANCE:
			tempValues1DVec.push_back(calcVariance(vectorOfDiscretizedPixels));
			break;

		default:
			//cout << "error : Unknown Intensity Histogram Feature!" << endl;
			break;
	}
}
void IntensityHistogram::featureExtraction(short* psImage, unsigned char* pucMask, int nHeight, int nWidth) {
	
	// get histogram
	vectorOfOriPixels = getVectorOfPixelsInROI(psImage, pucMask, nHeight, nWidth);
	vectorOfDiscretizedPixels = getVectorOfDiscretizedPixels_nBins();
	hist = getHistogram();

	vector<float> tempValues1DVec;

	// calculate checked feature
	for (int i = 0; i < isCheckedFeature.size(); i++) {
		if (isCheckedFeature[i]) calcFeature(i, tempValues1DVec);
	}

	tempValues2DVec.push_back(tempValues1DVec);

}

void IntensityHistogram::averageAllValues() {
	
	// get final mean vector
	for (int col = 0; col < tempValues2DVec[0].size(); col++) {
		float colSum = 0;
		float colMean;

		for (int row = 0; row < tempValues2DVec.size(); row++) {
			colSum += tempValues2DVec[row][col];
		}
		colMean = colSum / tempValues2DVec.size();
		final1DVec.push_back(colMean);
	}

	// set final feature value => Q. 꼭 멤버변수에 넣어줘야 하나..? final1DVec이면 되는데..
	/*
	for (int i = 0; i < isCheckedFeature.size(); i++) {
		if (isCheckedFeature[i]) {
			
		}
	}
	*/

}

void IntensityHistogram::defineFeatureNames(vector<string> &features) {
	// 총 23가지
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

