#include "NGTDM.h"

using namespace std;

NGTDM::NGTDM() {

}
NGTDM::~NGTDM() {

}
void NGTDM::clearVariable() {
	// 슬라이스마다, 방향마다 초기화
	vector1DofOriPixelsInROI.clear();
	vector<short>().swap(vector1DofOriPixelsInROI); // size & capacity 모두 0으로 초기화

	coarseness = NAN;
	contrast = NAN;
	busyness = NAN;
	complexity = NAN;
	strength = NAN;

}
void NGTDM::clearVector() {

	if (!final2DVec.empty()) {
		final2DVec.clear();
		vector<vector<float>>().swap(final2DVec); // size & capacity 모두 0으로 초기화
	}

	if (!final1DVec.empty()) {
		final1DVec.clear();
		vector<float>().swap(final1DVec);
	}

}

vector<short> NGTDM::get1DVectorOfPixels(short* psImage, unsigned char* pucMask) {
	
	vector<short> vector1DofOriPixels(nHeight*nWidth, 0); // 0으로 초기화 후, ROI 바깥 영역은 그대로 0. 미리 크기 할당해 두는게 훨씬 안정적
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			short imageValue = psImage[index];

			// ROI 밖(0)과 안 픽셀값들 모두 할당 (in 1d vector => for fast discretising!)
			vector1DofOriPixels[index] = (maskValue > (unsigned char)0) ? imageValue : 0;
			// ROI 내부값 push (ROI가 없으면 계속 pass => for min, max)
			if (maskValue >(unsigned char)0) {
				vector1DofOriPixelsInROI.push_back(imageValue); // 슬라이스마다 초기화 해줘야 함! (clearVariable)
			}
		}
	}
	nPixelsInROI = vector1DofOriPixelsInROI.size();

	return vector1DofOriPixels;
}
vector<vector<unsigned short>> NGTDM::get2DVectorOfDiscretizedPixels_FBN(short* psImage, unsigned char* pucMask) {

	// min(front) : -1, max(back) : 180
	float min = (float)*min_element(vector1DofOriPixelsInROI.begin(), vector1DofOriPixelsInROI.end()); // min_element() : pointer return
	float max = (float)*max_element(vector1DofOriPixelsInROI.begin(), vector1DofOriPixelsInROI.end());
	vector<float> tempFloatVec(vector1DofOriPixels.begin(), vector1DofOriPixels.end()); // short -> float 형변환

	if (min == 0 && min == max) {
		cout << "error in calculating discretization, VOI contains only 0" << endl;
		exit(0);
	}
	else if (min > 0 && min == max) {
		cout << "error in calculating discretization, VOI contains only one intensity value, minimum value is set to 0" << endl;
		min = 0;
	}


	// 우선 ROI 밖의 부분까지 모두 양자화 후, 맨 마지막에 if문으로 ROI 밖 영역은 0 처리

	//subtract minimum value from every matrix element
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(minus<float>(), min)); // minus<T> : 이항 연산 함수 객체 (-) / bind2nd : 2번째 인수를 고정해 함수 객체로 변환 / transform : 일괄 연산

	//get the range
	float range = (max - min) / nBins; // range : 몫 (width of a bin) => ***float이 들어가면 / 는 몫이 아니라 진짜 "나누기" 연산!!!***

	//divide every element of the matrix by the range
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(divides<float>(), range));

	//replace 0 to 1 => ibsi 핑크색 마킹 부분! (min - min = 0이 되므로 최소인 1로 바꿔줘야 뒤에 ceil에서 min value of each bin : 1 유지)
	replace(tempFloatVec.begin(), tempFloatVec.end(), 0, 1);

	// clear diffGreyLevels (슬라이스마다 초기화)
	diffGreyLevels.clear();
	vector<unsigned short>().swap(diffGreyLevels);

	//do rounding(ceil), type casting(unsigned short) and asigning to 2D Vec (with ROI)
	vector<vector<unsigned short>> vector2DofDiscretizedPixels(nHeight, vector<unsigned short>(nWidth, 0)); // 양자화한 최종 결과 픽셀값들 담을 벡터
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			unsigned short imageValue_discretized = (unsigned short)ceil(tempFloatVec[index]); // psImage (ori) -> tempFloatVec (discretized)

			// ROI 밖(0)과 안(discretized) 픽셀값들 2D Vec에 할당
			if (maskValue >(unsigned char)0) {
				vector2DofDiscretizedPixels[row][col] = imageValue_discretized;
				// diffGreyLevels에 담기
				diffGreyLevels.push_back(imageValue_discretized); // not 0 (in ROI)
			}
			else {
				vector2DofDiscretizedPixels[row][col] = 0;
			}
		}
	}

	// get diffGreyLevels (vector containing the different grey levels of the matrix -> extract every element only once / not 0 => in ROI!)
	sort(diffGreyLevels.begin(), diffGreyLevels.end());
	diffGreyLevels.erase(unique(diffGreyLevels.begin(), diffGreyLevels.end()), diffGreyLevels.end()); // 중복 제거

	return vector2DofDiscretizedPixels;
}
vector<vector<unsigned short>> NGTDM::get2DVectorOfDiscretizedPixels_FBS(short* psImage, unsigned char* pucMask) {

	float min = (float)*min_element(vector1DofOriPixelsInROI.begin(), vector1DofOriPixelsInROI.end());
	float max = (float)*max_element(vector1DofOriPixelsInROI.begin(), vector1DofOriPixelsInROI.end());
	vector<float> tempFloatVec(vector1DofOriPixels.begin(), vector1DofOriPixels.end());

	//subtract minimum value from every matrix element
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(minus<float>(), min));

	//divide every element of the matrix by the sBin (width of bin)
	if (isnan(sBin)) {
		cout << "invalid value error : sBin! It will be replaced with nBins=32..." << endl;
		sBin = (max - min) / 32; // range
	}
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(divides<float>(), sBin));

	// clear diffGreyLevels (슬라이스마다 초기화)
	diffGreyLevels.clear();
	vector<unsigned short>().swap(diffGreyLevels);

	//do rounding(floor) +1, type casting(unsigned short) and asigning to 2D Vec (with ROI)
	vector<vector<unsigned short>> vector2DofDiscretizedPixels(nHeight, vector<unsigned short>(nWidth, 0));
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			unsigned short imageValue_discretized = (unsigned short)floor(tempFloatVec[index]) + 1;

			// ROI 밖(0)과 안(discretized) 픽셀값들 2D Vec에 할당
			if (maskValue > (unsigned char)0) {
				vector2DofDiscretizedPixels[row][col] = imageValue_discretized;
				// diffGreyLevels에 담기
				diffGreyLevels.push_back(imageValue_discretized);
			}
			else {
				vector2DofDiscretizedPixels[row][col] = 0;
			}
		}
	}

	// get diffGreyLevels (vector containing the different grey levels of the matrix -> extract every element only once)
	sort(diffGreyLevels.begin(), diffGreyLevels.end());
	diffGreyLevels.erase(unique(diffGreyLevels.begin(), diffGreyLevels.end()), diffGreyLevels.end()); 

	return vector2DofDiscretizedPixels;
}
float NGTDM::getNeighborhood(vector<vector<unsigned short>> vector2DofDiscretizedPixels, int *indexOfElement) {
	vector<float> neighborhood;
	unsigned short actElement;
	//float weight;
	int dist = 1; // 임의로 지정 (distance = 1이면, 8인접)
	for (int i = -dist; i < dist + 1; i++) {
		for (int j = -dist; j < dist + 1; j++) {
			if (i != 0 || j != 0) {
				if (indexOfElement[0] + i > -1 && indexOfElement[0] + i < nHeight && indexOfElement[1] + j > -1 && indexOfElement[1] + j < nWidth) {
					actElement = vector2DofDiscretizedPixels[indexOfElement[0] + i][indexOfElement[1] + j];
					//weight = calculateWeight2D(i, j, normNGTDM, actualSpacing); // Chebyshev가 디폴트(weight : 1)
					neighborhood.push_back(actElement); // weight * actElement
					
				}
			}
		}
	}
	
	float sum = accumulate(neighborhood.begin(), neighborhood.end(), 0.0); // numeric.h
	sum = sum / neighborhood.size();

	return sum;
}
void NGTDM::getProbability(vector<unsigned short> elementsOfWholeNeighborhood, vector<vector<float>> &NGTDMatrix) {
	
	int numItem = 0;
	
	for (int actElementIndex = 0; actElementIndex<diffGreyLevels.size(); actElementIndex++) {
		numItem += NGTDMatrix[actElementIndex][0];
	}
	for (int actElementIndex = 0; actElementIndex < diffGreyLevels.size(); actElementIndex++) {
		NGTDMatrix[actElementIndex][1] = NGTDMatrix[actElementIndex][0] / numItem;
	}

}
void NGTDM::fill2DNGTDMatrix(vector<vector<unsigned short>> vector2DofDiscretizedPixels, vector<vector<float>> &NGTDMatrix) {

	// vector2DofDiscretizedPixels : 멤버변수 copy (매개변수로 새 벡터 할당)

	int indexOfElement[2] = { 0,0 };
	vector<unsigned short> elementsOfWholeNeighborhoods;
	float sumOfActualNeighborhood;
	unsigned short s;
	unsigned short actualElement;
	int posActualElement;
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			indexOfElement[0] = row;
			indexOfElement[1] = col;
			//get actual Element if it is the centre of a whole neighborhood
			actualElement = vector2DofDiscretizedPixels[row][col];
			
			if (actualElement != 0) { // 0이면 ROI 밖이므로 X (greyDiffLevel, NGTDM의 행 index > 0)
				//get the sum of the actual neighborhood
				sumOfActualNeighborhood = getNeighborhood(vector2DofDiscretizedPixels, indexOfElement);
				//get the s_i value
				s = abs(actualElement - sumOfActualNeighborhood);
				//get the position of the actual Element in the diffGreyLevel vector and so also in the matrix
				posActualElement = find(diffGreyLevels.begin(), diffGreyLevels.end(), actualElement) - diffGreyLevels.begin();
				//add the s_i value to the right element in the matrix
				NGTDMatrix[posActualElement][2] += s;
				NGTDMatrix[posActualElement][0] += 1;
				//save the actual Element in a vector, so we can calculate later the probabilities
				elementsOfWholeNeighborhoods.push_back(actualElement);
			}
		}
	}
	
	getProbability(elementsOfWholeNeighborhoods, NGTDMatrix); // 1번째 열 채우기(p_i)

}

void NGTDM::calcCoarseness(vector<vector<float>> NGTDMatrix) {
	
	coarseness = 0;

	float sumSiPi = 0;
	for (int row = 0; row<sizeMatrix; row++) {
		sumSiPi += NGTDMatrix[row][1] * NGTDMatrix[row][2];
	}

	if (sumSiPi != 0) {
		coarseness = 1 / sumSiPi;
	}
	
}
void NGTDM::calContrast(vector<vector<float>> NGTDMatrix) {

	// getNGP & getNV
	int ng = 0;
	int nv = 0;
	for (int i = 0; i< sizeMatrix; i++) {
		if (NGTDMatrix[i][0] != 0) {
			ng += 1;
		}
		nv += NGTDMatrix[i][0];
	}

	contrast = 0;
	float sumSi = 0;
	for (int row = 0; row<sizeMatrix; row++) {
		for (int rowj = 0; rowj<sizeMatrix; rowj++) {
			contrast += NGTDMatrix[row][1] * NGTDMatrix[rowj][1] * pow((diffGreyLevels[row] - diffGreyLevels[rowj]), 2);
		}
		// calculateSumSi
		sumSi += NGTDMatrix[row][2];
	}
	
	contrast = contrast*sumSi;
	contrast = contrast / (ng*(ng - 1)*nv);

}
void NGTDM::calcBusyness(vector<vector<float>> NGTDMatrix) {

	float sumSiPi = 0;
	for (int row = 0; row<sizeMatrix; row++) {
		sumSiPi += NGTDMatrix[row][1] * NGTDMatrix[row][2];
	}

	// getNGP
	int ng = 0;
	for (int i = 0; i< sizeMatrix; i++) {
		if (NGTDMatrix[i][0] != 0) {
			ng += 1;
		}
	}

	float denominator = 0;
	if (ng > 1) {
		for (int row = 0; row < sizeMatrix; row++) {
			for (int rowj = 0; rowj < sizeMatrix; rowj++) {
				if (NGTDMatrix[row][1] != 0 && NGTDMatrix[rowj][1] != 0) {
					denominator += abs(diffGreyLevels[row] * NGTDMatrix[row][1] - NGTDMatrix[rowj][1] * diffGreyLevels[rowj]);
				}
			}
		}
		busyness = sumSiPi / denominator;
	}
	else {
		busyness = 0;
	}

}
void NGTDM::calcComplexity(vector<vector<float>> NGTDMatrix) {
	
	// getNGP & getNV
	int ng = 0;
	int nv = 0;
	for (int i = 0; i< sizeMatrix; i++) {
		if (NGTDMatrix[i][0] != 0) {
			ng += 1;
		}
		nv += NGTDMatrix[i][0];
	}

	complexity = 0;
	float nominator;
	float denominator;
	for (int row = 0; row < sizeMatrix; row++) {
		for (int rowj = 0; rowj < sizeMatrix; rowj++) {
			if (NGTDMatrix[row][0] != 0 && NGTDMatrix[rowj][0] != 0) {
				nominator = abs(diffGreyLevels[row] - diffGreyLevels[rowj])*(NGTDMatrix[row][1] * NGTDMatrix[row][2] + NGTDMatrix[rowj][2] * NGTDMatrix[rowj][1]);
				denominator = NGTDMatrix[row][1] + NGTDMatrix[rowj][1];
				complexity += nominator / denominator;
			}
		}
	}
	complexity = complexity / nv;

}
void NGTDM::calcStrength(vector<vector<float>> NGTDMatrix) {

	strength = 0;

	float sumSi = 0;
	for (int row = 0; row<sizeMatrix; row++) {
		// calculateSumSi
		sumSi += NGTDMatrix[row][2];
	}

	for (int row = 0; row < sizeMatrix; row++) {
		for (int rowj = 0; rowj < sizeMatrix; rowj++) {
			if (NGTDMatrix[row][0] != 0 && NGTDMatrix[rowj][0] != 0) {
				strength += (NGTDMatrix[row][1] + NGTDMatrix[rowj][1]) * pow(diffGreyLevels[row] - diffGreyLevels[rowj], 2);
			}
		}
	}

	strength = strength / sumSi;

}

void NGTDM::calcFeature(int FEATURE_IDX, vector<float> &tempValues1DVec, vector<vector<float>> NGTDMatrix) {

	switch (FEATURE_IDX)
	{
	case COARSENESS:
		calcCoarseness(NGTDMatrix);
		tempValues1DVec.push_back(coarseness);
		break;

	case CONTRAST:
		calContrast(NGTDMatrix);
		tempValues1DVec.push_back(contrast);
		break;

	case BUSYNESS:
		calcBusyness(NGTDMatrix);
		tempValues1DVec.push_back(busyness);
		break;

	case COMPLEXITY:
		calcComplexity(NGTDMatrix);
		tempValues1DVec.push_back(complexity);
		break; 

	case STRENGTH:
		calcStrength(NGTDMatrix);
		tempValues1DVec.push_back(strength);
		break;

	default:
		break;
	}
}
void NGTDM::featureExtraction(short* psImage, unsigned char* pucMask, int nHeight_, int nWidth_) {

	// claer all values
	clearVariable(); // 슬라이스마다 초기화

	// get discretized 2D input matrix (2d vector)
	nHeight = nHeight_;
	nWidth = nWidth_;
	vector1DofOriPixels = get1DVectorOfPixels(psImage, pucMask);  // 슬라이스마다 초기화, nPixelsInROI 산출
	vector2DofDiscretizedPixels = isFBN? get2DVectorOfDiscretizedPixels_FBN(psImage, pucMask) : get2DVectorOfDiscretizedPixels_FBS(psImage, pucMask); // 슬라이스마다 초기화

	// calculate checked feature
	sizeMatrix = diffGreyLevels.size();		// ***NGTDM에서는 Matrix의 row 크기인 sizeMatrix가 maxIntensity가 아닌 diffGreyLevels의 size가 정확!!***
		
	// calculate NGTDM matrix
	vector<vector<float>> NGTDMatrix(sizeMatrix, vector<float>(3, 0));
	fill2DNGTDMatrix(vector2DofDiscretizedPixels, NGTDMatrix); 

	vector<float> tempValues1DVec;			// 슬라이스마다 초기화 (for. final2DVec에 누적)

	// calculate feature for each dir
	for (int i = 0; i < FEATURE_COUNT; i++) {
		if (isCheckedFeature[i]) calcFeature(i, tempValues1DVec, NGTDMatrix);
	}

	final2DVec.push_back(tempValues1DVec); // 모든 ROI 슬라이스 들어올 때까지 누적 (시리즈마다 초기화)
	
}

void NGTDM::averageAllValues() {

	// get final mean vector
	if (!final2DVec.empty()) {
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
}

void NGTDM::defineFeatureNames(vector<string> &features) {
	features[COARSENESS] = "coarseness";
	features[CONTRAST] = "contrast";
	features[BUSYNESS] = "busyness";
	features[COMPLEXITY] = "complexity";
	features[STRENGTH] = "strength";
}

