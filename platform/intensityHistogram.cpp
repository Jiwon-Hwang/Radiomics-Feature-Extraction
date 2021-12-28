#include "IntensityHistogram.h"

using namespace std;

IntensityHistogram::IntensityHistogram() {

}

IntensityHistogram::~IntensityHistogram() {

}


vector<short> getVectorOfPixelsInROI(short* pusImage, unsigned char* pucMask, int nHeight, int nWidth) {

	vector<short> vectorOfOriPixels; // size : 3032
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			short imageValue = pusImage[index];

			// ROI ���ΰ� push (ROI�� ������ ��� pass)
			if (maskValue >(unsigned char)0) {
				vectorOfOriPixels.push_back(imageValue);
			}
		}
	}

	sort(vectorOfOriPixels.begin(), vectorOfOriPixels.end()); // min(front) : -1, max(back) : 180

	return vectorOfOriPixels;
}
vector<unsigned short> getVectorOfDiscretizedPixels_nBins(vector<short> vectorOfOriPixels, int nBins = 32) {

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
vector<unsigned short> getVectorOfDiffGreyLevels(vector<unsigned short> vectorOfDiscretizedPixels) {

	vector<unsigned short> diffGreyLevels(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end()); // 1~nbins ���� ������ ����ȭ�� �ȼ�����
	diffGreyLevels.erase(unique(diffGreyLevels.begin(), diffGreyLevels.end()), diffGreyLevels.end()); // size : 168 => 32 bins�� ������ �� ���� �� �� 5���� �ȼ������� �󵵼��� ����

	return diffGreyLevels;
}
vector<unsigned int> getHistogram(vector<unsigned short> vectorOfDiscretizedPixels) {

	vector<unsigned short> diffGreyLevels = getVectorOfDiffGreyLevels(vectorOfDiscretizedPixels);

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

float calcMean(vector<unsigned short> vectorOfDiscretizedPixels) {
	float mean = accumulate(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end(), 0.0) / vectorOfDiscretizedPixels.size(); // 0.0 : initial value of the sum

	return mean;
}
float calcVariance(vector<unsigned short> vectorOfDiscretizedPixels) {
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


void defineFeatures(vector<string> &features) {
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
void extractData(IntensityHistogram intenseHisto, vector<float> &intensityHistogramValues) {
	// �ϴ� ���⼭ intensityHistogramFeatures��� ��ü�� ������� ����(NAN�� ������ Ư¡����) ��� �� ����(intensityHistogramValues)�� push 
	// ���� �ۿ��� �� ���͸� ���� Ư¡�� üũ T/F ���� ��� bool �迭 �ݺ������� üũ�ϸ鼭 True�� �ش��ϴ� idx�� Ư¡���鸸 �̾Ƽ� writeCSV

	intensityHistogramValues.push_back(intenseHisto.meanValue);
	intensityHistogramValues.push_back(intenseHisto.varianceValue);
	intensityHistogramValues.push_back(intenseHisto.skewnessValue);
	intensityHistogramValues.push_back(intenseHisto.kurtosisValue);
	intensityHistogramValues.push_back(intenseHisto.medianValue);
	intensityHistogramValues.push_back(intenseHisto.minimumValue);
	intensityHistogramValues.push_back(intenseHisto.percentile10);
	intensityHistogramValues.push_back(intenseHisto.percentile90);
	intensityHistogramValues.push_back(intenseHisto.maximumValue);
	intensityHistogramValues.push_back(intenseHisto.interquartileRange);
	intensityHistogramValues.push_back(intenseHisto.mode);
	intensityHistogramValues.push_back(intenseHisto.rangeValue);
	intensityHistogramValues.push_back(intenseHisto.meanAbsDev);
	intensityHistogramValues.push_back(intenseHisto.robustMeanAbsDev);
	intensityHistogramValues.push_back(intenseHisto.medianAbsDev);
	intensityHistogramValues.push_back(intenseHisto.coeffOfVar);
	intensityHistogramValues.push_back(intenseHisto.quartileCoeff);
	intensityHistogramValues.push_back(intenseHisto.entropy);
	intensityHistogramValues.push_back(intenseHisto.histUniformity);
	intensityHistogramValues.push_back(intenseHisto.maxHistGradient);
	intensityHistogramValues.push_back(intenseHisto.maxHistGradGreyValue);
	intensityHistogramValues.push_back(intenseHisto.minHistGradient);
	intensityHistogramValues.push_back(intenseHisto.minHistGradGreyValue);

}