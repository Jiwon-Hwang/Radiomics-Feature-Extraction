#include "data.h"

CData::CData() {
	init();
}
CData::~CData() {
	clear();
}
void CData::init() {
	
}
void CData::clear() {
	for(int i=0, ni=getSeriesCount(); i<ni; i++) {
		SAFE_DELETE(m_seriesList[i]);
	}
	m_seriesList.clear();
}
void CData::clearImages(int nSeriesIdx) {
	if(nSeriesIdx == -1) {
		for(int s=0, ns=getSeriesCount(); s<ns; s++) {
			CSeries* pTargetSeries = getSeries(s);

			for(int i=0, ni=pTargetSeries->getImageCount(); i<ni; i++) {
				CImage<short>* pTargetImage = pTargetSeries->getImage(i);
				CImage<unsigned char>* pTargetMask = pTargetSeries->getMask(i);

				pTargetImage->clearImage();
				pTargetMask->clearImage();
			}
		}
	}
	else {
		CSeries* pTargetSeries = getSeries(nSeriesIdx);

		for(int i=0, ni=pTargetSeries->getImageCount(); i<ni; i++) {
			CImage<short>* pTargetImage = pTargetSeries->getImage(i);
			CImage<unsigned char>* pTargetMask = pTargetSeries->getMask(i);

			pTargetImage->clearImage();
			pTargetMask->clearImage();
		}
	}
}

void CData::readDir(std::string sPath) {
	// 1. image 파일 혹은 폴더를 순회하면서 읽음
	// Path에 있는 File 혹은 Dir를 순회하면서 읽어야 할 File 목록을 생성 //
	std::list<std::string> sFilePaths;
	if(isFile(sPath)) {
		// sPath가 파일인 경우
		sFilePaths.push_back(sPath);
	}
	else {
		// sPath가 폴더인 경우 하위 디렉토리까지 순회하면서, 읽어야 할 파일의 목록을 생성
		searchingDir(sPath, sFilePaths);
	}

	for(std::list<std::string>::iterator it=sFilePaths.begin(); it!=sFilePaths.end(); it++) {
		readImage(*it);
	}

	// 2. 매칭되지 못했던 mask를 매칭시킴
	matchingImageAndMask();

	// 3. 전체 Series를 순회하면서, Image, Mask 순서 Sorting
	sortingImageAndMask();
}
void CData::readImage(std::string sImagePath) {	
	bool isLoad = false;
	bool isMask = false;

	// 1개 파일에 여러개의 이미지 혹은 mask가 들어있는 경우를 처리하기 위함
	std::vector<CImage<short>*> ciImages;
	std::vector<CImage<unsigned char>*> ciMasks;
	CSeries* pCiSeries = NULL;

	// 1-1. 파일명 혹은 폴더명으로 mask여부 판별
	isMask = isMask_namingRule(sImagePath);

	// 1-2. 이미지를 Load 및 mask여부 판별, 파일명 혹은 폴더명으로 이미 구분된 경우, 별도 검사하지 않음
	if(isMask) {
		// 파일명, 확장자 등으로 Mask로 구분이 이미 된 경우
		isLoad = loadImage(sImagePath, pCiSeries, ciMasks);
	}
	else {
		// Image인지 Mask인지 모르는 경우, isMask_imageRule로 구분
		isLoad = loadImage(sImagePath, pCiSeries, ciImages);

		if(isLoad) {
			// Mask인 경우, 자료형을 short->unsigned char로 바꿔줌
			for(int i=0, ni=ciImages.size(); i<ni; i++) {
				CImage<short>* pCiImage = ciImages[i];

				isMask = isMask_imageRule(pCiImage);
				if(isMask) {
					CImage<unsigned char>* pCiMask = NULL;
					pCiImage->convertTo(pCiMask);
					SAFE_DELETE(pCiImage);

					ciMasks.push_back(pCiMask);
				}
			}
		}
	}

	// 1-3. 파일에서 읽은 Series 정보를 사용자 임의로 수정
	if(isLoad && pCiSeries) {
		if(!isMask) {
			setPatientName(pCiSeries, ciImages);
			setStudyName(pCiSeries, ciImages);
			setSeriesName(pCiSeries, ciImages);

		}
		else {
			setPatientName(pCiSeries, ciMasks);
			setStudyName(pCiSeries, ciMasks);
			setSeriesName(pCiSeries, ciMasks);
		}
	}

	// 1-4. Series에 load한 이미지 매칭
	if(isLoad) {
		if(!isMask) {
			// 1-4-1. Image인 경우, Series 중복 검사
			CSeries* pCiMatchedSeries = NULL;
			for(int s=0, ns=getSeriesCount(); s<ns; s++) {
				if(isSameSeries(pCiSeries, getSeries(s))) {
					pCiMatchedSeries = m_seriesList[s];
					break;
				}
			}
			if(pCiMatchedSeries != NULL) {
				// Series가 중복되는 경우
				SAFE_DELETE(pCiSeries);
				pCiSeries = pCiMatchedSeries;
			}
			else {
				// Series가 중복되지 않는 경우 (= 새로운 Series인 경우 Series를 추가)
				addSeries(pCiSeries);
			}

			// 1-4-2. Series에 Image, Mask 추가
			for(int idx=0, nIdx=ciImages.size(); idx<nIdx; idx++) {
				// Image
				CImage<short>* pCiImage = ciImages[idx];
				pCiSeries->addImage(pCiImage);

				// Mask (Image 정보를 복사, Lazy Loading인 경우 NULL값만 추가)
				CImage<unsigned char>* pCiMask = NULL;
				pCiImage->convertTo(pCiMask);
				pCiMask->setImageExtension("");
				pCiMask->setImagePath("");

				if(!LAZY_LOADING) {
					memset(pCiMask->m_image, 0, sizeof(unsigned char)*pCiMask->getSize());
				}
				else {
					pCiMask->clearImage();
				}
				pCiSeries->addMask(pCiMask);
			}
			
		}
		else {
			// 1-4-1. Mask인 경우, Series 중복 검사
			CSeries* pCiMatchedSeries = NULL;
			for(int s=0, ns=getSeriesCount(); s<ns; s++) {
				if(isSameSeries(pCiSeries, getSeries(s))) {
					pCiMatchedSeries = m_seriesList[s];
					break;
				}
			}
			SAFE_DELETE(pCiSeries);

			// 1-4-2. 매칭되는 Mask를 찾아서 현재 Mask로 수정
			if(pCiMatchedSeries) {
				for(int idx=0, nIdx=ciMasks.size(); idx<nIdx; idx++) {
					CImage<unsigned char>* pCiMask = ciMasks[idx];
					int nMatchedImageIdx = -1;

					// 매칭되는 Image 검사
					for(int i=0, ni=pCiMatchedSeries->getImageCount(); i<ni; i++) {
						if(isSameImage(pCiMask, pCiMatchedSeries->getImage(i))) {
							nMatchedImageIdx = i;
							break;
						}
					}
					// 매칭되는 image가 없지만, images 갯수가 동일하면 차례대로 매칭 (임시)
					if(nMatchedImageIdx == -1 && ciMasks.size()== pCiMatchedSeries->getImageCount()) {
						nMatchedImageIdx = idx;
					}


					if(nMatchedImageIdx != -1) {
						// 매칭되는 Image가 있는 경우
						pCiMatchedSeries->setMask(nMatchedImageIdx, pCiMask);
					}
					else {
						// 매칭되는 Image가 없는 경우 (나중에 매칭할 수 있도록 일단 저장)
						// 여기 loop로 빠지는건 데이터가 잘못되어있을 확률이 높음
						m_nonMatchedMaskPaths.push_back(sImagePath);

						SAFE_DELETE_ARRAY(pCiMask);
					}
				}
			}
			else {
				// 매칭되는 Series 가 없는 경우 (나중에 매칭할 수 있도록 파일 경로만 저장)
				m_nonMatchedMaskPaths.push_back(sImagePath);

				for(int idx=0, nIdx=ciMasks.size(); idx<nIdx; idx++) {
					SAFE_DELETE(ciMasks[idx]);
				}
			}
		}
	}

	ciImages.clear();
	ciMasks.clear();
}
void CData::matchingImageAndMask() {
	for(int i=0, ni=m_nonMatchedMaskPaths.size(); i<ni; i++) {
		readImage(m_nonMatchedMaskPaths[i]);
	}
}
void CData::sortingImageAndMask() {
	for(int i=0,ni=m_seriesList.size(); i<ni; i++) {

		// 1. 파일 이름순으로 정렬
		sort(m_seriesList[i]->m_images.begin(), m_seriesList[i]->m_images.end(), sorting_fileName<short>);
		sort(m_seriesList[i]->m_masks.begin(), m_seriesList[i]->m_masks.end(), sorting_fileName<unsigned char>);
	}
}

CSeries* CData::getSeries(int nSeriesIdx) {
	CSeries* pTargetSeries = NULL;
	if(nSeriesIdx >= 0 && nSeriesIdx < getSeriesCount()) {
		pTargetSeries = m_seriesList[nSeriesIdx];
	}

	return pTargetSeries;
}
int CData::getSeriesCount(void) {
	return m_seriesList.size();
}
bool CData::setSeries(CSeries* pCiSeries, int nSeriesIdx) {
	if(pCiSeries == NULL) {
		return false;
	}

	if(nSeriesIdx == -1) {
		// nTargetSeriesIdx가 명시되어 있지 않은 경우, 맨 뒤에 넣음
		addSeries(pCiSeries);
	}
	else {
		if(m_seriesList[nSeriesIdx] != NULL) {
			SAFE_DELETE(m_seriesList[nSeriesIdx]);
		}
		m_seriesList[nSeriesIdx] = pCiSeries;
	}

	return true;
}
bool CData::addSeries(CSeries* pCiSeries) {
	if(pCiSeries == NULL) {
		return false;
	}

	m_seriesList.push_back(pCiSeries);
	return true;
}

////////////////////////////////////////////////////////////////////
CImage<short>* CData::getCImage(int nSliceIdx) {
	CImage<short>* pTargetImage = NULL;
	int nTargetSeriesIdx = -1;
	int nTargetImageIdx = -1;
	
	int nIdx = 0;
	for(int i=0, ni=getSeriesCount(); i<ni; i++) {
		int nImageCnt = getSeries(i)->getImageCount();

		if(nSliceIdx >= nIdx) {
			nTargetSeriesIdx = i;
			nTargetImageIdx = nSliceIdx-nIdx;
			pTargetImage = getSeries(nTargetSeriesIdx)->getImage(nTargetImageIdx);
			break;
		}

		nIdx = nIdx + nImageCnt-1;
	}

	return pTargetImage;
}
CImage<short>* CData::getCImage(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = NULL;
	
	if(nSeriesIdx >= 0 && nSeriesIdx < getSeriesCount()) {
		if(nImageIdx >= 0 && nImageIdx < getSeries(nSeriesIdx)->getImageCount()) {
			pTargetImage = getSeries(nSeriesIdx)->getImage(nImageIdx);
		}
	}

	return pTargetImage;
}
short* CData::getImage(int nSliceIdx) {
	CImage<short>* pTargetImage = getCImage(nSliceIdx);
	short* psImage = pTargetImage->getImage();

	if(LAZY_LOADING && psImage == NULL) {
		if(loadImage(pTargetImage, false)) {
			psImage = pTargetImage->getImage();
		}
	}
	return psImage;
}

bool CData::setImage(int nSliceIdx, CImage<short>* pCiImage) {
	bool isSet = false;
	CImage<short>* pTargetImage = getCImage(nSliceIdx);

	if(pTargetImage) {
		isSet = pTargetImage->setImage(pCiImage);
	}

	return isSet;
}
bool CData::setImage(int nSliceIdx, short* psImage, int nWidth, int nHeight, int nChannel) {
	bool isSet = false;
	CImage<short>* pTargetImage = getCImage(nSliceIdx);

	if(pTargetImage) {
		isSet = pTargetImage->setImage(psImage, nWidth, nHeight);
	}

	return isSet;
}
bool CData::setImage(int nSeriesIdx, int nImageIdx, CImage<short>* pCiImage) {
	bool isSet = false;
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);

	if(pTargetImage) {
		isSet = pTargetImage->setImage(pCiImage);
	}

	return isSet;
}
bool CData::setImage(int nSeriesIdx, int nImageIdx, short* psImage, int nWidth, int nHeight, int nChannel) {
	bool isSet = false;
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);

	if(pTargetImage) {
		isSet = pTargetImage->setImage(psImage, nWidth, nHeight, nChannel);
	}

	return isSet;
}
bool CData::setImages(int nSeriesIdx, short** ppsImages, int nImageCnt, int nWidth, int nHeight, int nChannel) {
	bool isSet = false;
	CSeries* pTargetSeries = getSeries(nSeriesIdx);

	if(ppsImages == NULL || pTargetSeries == NULL) {
		return isSet;
	}

	if(pTargetSeries && pTargetSeries->getImageCount() <= nImageCnt) {
		for(int i=0, ni=pTargetSeries->getImageCount(); i<ni; i++) {
			isSet = pTargetSeries->getImage(i)->setImage(ppsImages[i], nWidth, nHeight, nChannel);
			if(!isSet) {
				return isSet;
			}
		}
	}

	return isSet;
}
bool CData::setImages(int nStartSliceIdx, int nEndSliceIdx, short** ppsImages, int nImageCnt, int nWidth, int nHeight, int nChannel) {
	bool isSet = false;

	for(int i=0, ni=(nEndSliceIdx-nStartSliceIdx+1); i<ni; i++) {
		isSet = setImage(nStartSliceIdx+i, ppsImages[i], nWidth, nHeight, nChannel);
		if(!isSet) {
			return isSet;
		}
	}
	return isSet;
}

bool CData::copyImage(int nSliceIdx, short* &psImage, int &nWidth, int &nHeight) {
	bool isCopy = false;
	CImage<short>* pTargetImage = getCImage(nSliceIdx);

	if(pTargetImage) {
		isCopy = pTargetImage->copyImage(psImage, nWidth, nHeight);
		if(LAZY_LOADING && !isCopy) {
			if(loadImage(pTargetImage, false)) {
				isCopy = pTargetImage->copyImage(psImage, nWidth, nHeight);
			}
		}
	}

	return isCopy;
}
bool CData::copyImage(int nSliceIdx, short* &psImage, int &nWidth, int &nHeight, int &nChannel) {
	bool isCopy = false;
	CImage<short>* pTargetImage = getCImage(nSliceIdx);

	if(pTargetImage) {
		isCopy = pTargetImage->copyImage(psImage, nWidth, nHeight, nChannel);
		if(LAZY_LOADING && !isCopy) {
			if(loadImage(pTargetImage, false)) {
				isCopy = pTargetImage->copyImage(psImage, nWidth, nHeight, nChannel);
			}
		}
	}

	return isCopy;
}
bool CData::copyImages(int nSeriesIdx, short** &ppsImages, int &nImageCnt, int &nWidth, int &nHeight) {
	bool isCopy = false;

	CSeries* pTargetSeries = getSeries(nSeriesIdx);

	if(ppsImages == NULL && pTargetSeries) {
		nImageCnt = pTargetSeries->getImageCount();
		ppsImages = new short* [nImageCnt];
		memset(ppsImages, 0, sizeof(short*)*nImageCnt);

		for(int i=0; i< nImageCnt; i++) {
			isCopy = false;

			CImage<short>* pTargetImage = pTargetSeries->getImage(i);
			isCopy = pTargetImage->copyImage(ppsImages[i], nWidth, nHeight);

			if(LAZY_LOADING && !isCopy) {
				if(loadImage(pTargetImage, false)) {
					isCopy = pTargetImage->copyImage(ppsImages[i], nWidth, nHeight);
				}
			}
			
		}
	}

	return isCopy;
}
bool CData::copyImages(int nSeriesIdx, short** &ppsImages, int &nImageCnt, int &nWidth, int &nHeight, int &nChannel) {
	bool isCopy = false;

	CSeries* pTargetSeries = getSeries(nSeriesIdx);

	if(ppsImages == NULL && pTargetSeries) {
		nImageCnt = pTargetSeries->getImageCount();
		ppsImages = new short* [nImageCnt];
		memset(ppsImages, 0, sizeof(short*)*nImageCnt);

		for(int i=0; i< nImageCnt; i++) {
			isCopy = false;

			CImage<short>* pTargetImage = pTargetSeries->getImage(i);
			isCopy = pTargetImage->copyImage(ppsImages[i], nWidth, nHeight);

			if(LAZY_LOADING && !isCopy) {
				if(loadImage(pTargetImage, false)) {
					isCopy = pTargetImage->copyImage(ppsImages[i], nWidth, nHeight, nChannel);
				}
			}
		}
	}

	return isCopy;
}
bool CData::copyImages(int nStartSliceIdx, int nEndSliceIdx, short** &ppsImages, int &nImageCnt, int &nWidth, int &nHeight) {
	bool isCopy = false;

	nImageCnt = nEndSliceIdx - nStartSliceIdx + 1;

	if(ppsImages != NULL) {
		return isCopy;
	}
	ppsImages = new short* [nImageCnt];
	memset(ppsImages, 0, sizeof(short*) * nImageCnt);

	for(int i=0; i< nImageCnt; i++) {
		isCopy = copyImage(i+nStartSliceIdx, ppsImages[i], nWidth, nHeight);
	}

	return isCopy;
}
bool CData::copyImages(int nStartSliceIdx, int nEndSliceIdx, short** &ppsImages, int &nImageCnt, int &nWidth, int &nHeight, int &nChannel) {
	bool isCopy = false;

	nImageCnt = nEndSliceIdx - nStartSliceIdx + 1;
	ppsImages = new short* [nImageCnt];
	memset(ppsImages, 0, sizeof(short*) * nImageCnt);

	for(int i=0; i< nImageCnt; i++) {
		isCopy = copyImage(i+nStartSliceIdx, ppsImages[i], nWidth, nHeight, nChannel);
	}

	return isCopy;
}

CImage<unsigned char>* CData::getCMask(int nSliceIdx) {
	CImage<unsigned char>* pTargetImage = NULL;
	int nTargetSeriesIdx = -1;
	int nTargetImageIdx = -1;
	
	int nIdx = 0;
	for(int i=0, ni=getSeriesCount(); i<ni; i++) {
		int nImageCnt = getSeries(i)->getMaskCount();

		if(nSliceIdx >= nIdx) {
			nTargetSeriesIdx = i;
			nTargetImageIdx = nSliceIdx-nIdx;
			pTargetImage = getSeries(nTargetSeriesIdx)->getMask(nTargetImageIdx);
			break;
		}

		nIdx = nIdx + nImageCnt-1;
	}

	return pTargetImage;
}
CImage<unsigned char>* CData::getCMask(int nSeriesIdx, int nMaskIdx) {
	CImage<unsigned char>* pTargetMask = NULL;
	
	if(nSeriesIdx >= 0 && nSeriesIdx < getSeriesCount()) {
		if(nMaskIdx >= 0 && nMaskIdx < getSeries(nSeriesIdx)->getMaskCount()) {
			pTargetMask = getSeries(nSeriesIdx)->getMask(nMaskIdx);
		}
	}

	return pTargetMask;
}
unsigned char* CData::getMask(int nSliceIdx) {
	CImage<unsigned char>* pTargetMask = getCMask(nSliceIdx);
	unsigned char* pucMask = pTargetMask->getImage();

	if(LAZY_LOADING && pucMask == NULL) {
		if(loadImage(pTargetMask, true)) {
			pucMask = pTargetMask->getImage();
		}
	}

	return pucMask;
}

bool CData::setMask(int nSliceIdx, CImage<unsigned char>* pCiMask) {
	bool isSet = false;
	CImage<unsigned char>* pTargetMask = getCMask(nSliceIdx);

	if(pTargetMask) {
		isSet = pTargetMask->setImage(pCiMask);
	}

	return isSet;
}
bool CData::setMask(int nSliceIdx, unsigned char* pucMask, int nWidth, int nHeight, int nChannel) {
	bool isSet = false;
	CImage<unsigned char>* pTargetMask = getCMask(nSliceIdx);

	if(pTargetMask) {
		isSet = pTargetMask->setImage(pucMask, nWidth, nHeight);
	}

	return isSet;
}
bool CData::setMask(int nSeriesIdx, int nImageIdx, CImage<unsigned char>* pCiMask) {
	bool isSet = false;
	CImage<unsigned char>* pTargetMask = getCMask(nSeriesIdx, nImageIdx);

	if(pTargetMask) {
		isSet = pTargetMask->setImage(pCiMask);
	}

	return isSet;
}
bool CData::setMask(int nSeriesIdx, int nImageIdx, unsigned char* pucMask, int nWidth, int nHeight, int nChannel) {
	bool isSet = false;
	CImage<unsigned char>* pTargetMask = getCMask(nSeriesIdx, nImageIdx);

	if(pTargetMask) {
		isSet = pTargetMask->setImage(pucMask, nWidth, nHeight);
	}

	return isSet;
}
bool CData::setMasks(int nSeriesIdx, unsigned char** ppucMasks, int nMaskCnt, int nWidth, int nHeight, int nChannel) {
	bool isSet = false;
	CSeries* pTargetSeries = getSeries(nSeriesIdx);
	
	if(ppucMasks == NULL || pTargetSeries == NULL){
		return isSet;
	}

	if(pTargetSeries) {
		int nImageCount = pTargetSeries->getImageCount();

		for(int i=0; i< nImageCount; i++) {
			isSet = setMask(nSeriesIdx, i, ppucMasks[i], nWidth, nHeight, nChannel);
			if(!isSet) {
				return isSet;
			}
		}
	}

	return isSet;
}
bool CData::setMasks(int nStartSliceIdx, int nEndSliceIdx, unsigned char** ppucMasks, int nMaskCnt, int nWidth, int nHeight, int nChannel) {
	bool isSet = false;

	for(int i=0, ni=(nEndSliceIdx-nStartSliceIdx+1); i<ni; i++) {
		isSet = setMask(nStartSliceIdx+i, ppucMasks[i], nWidth, nHeight, nChannel);
		if(!isSet) {
			return isSet;
		}
	}
	return isSet;
}

bool CData::copyMask(int nSliceIdx, unsigned char* &pucMask, int &nWidth, int &nHeight) {
	bool isCopy = false;
	CImage<unsigned char>* pTargetMask = getCMask(nSliceIdx);

	if(pTargetMask) {
		isCopy = pTargetMask->copyImage(pucMask, nWidth, nHeight);
		if(LAZY_LOADING && !isCopy) {
			if(loadImage(pTargetMask, true)) {
				isCopy = pTargetMask->copyImage(pucMask, nWidth, nHeight);
			}
		}
	}

	return isCopy;
}
bool CData::copyMask(int nSliceIdx, unsigned char* &pucMask, int &nWidth, int &nHeight, int &nChannel) {
	bool isCopy = false;
	CImage<unsigned char>* pTargetMask = getCMask(nSliceIdx);

	if(pTargetMask) {
		isCopy = pTargetMask->copyImage(pucMask, nWidth, nHeight, nChannel);
		if(LAZY_LOADING && !isCopy) {
			if(loadImage(pTargetMask, true)) {
				isCopy = pTargetMask->copyImage(pucMask, nWidth, nChannel);
			}
		}
	}

	return isCopy;
}
bool CData::copyMasks(int nSeriesIdx, unsigned char** &ppucMasks, int &nMaskCnt, int &nWidth, int &nHeight) {
	bool isCopy = false;
	CSeries* pTargetSeries = getSeries(nSeriesIdx);

	if(ppucMasks == NULL && pTargetSeries) {
		nMaskCnt = pTargetSeries->getImageCount();
		ppucMasks = new unsigned char* [nMaskCnt];
		memset(ppucMasks, 0, sizeof(unsigned char*)*nMaskCnt);

		for(int i=0; i< nMaskCnt; i++) {
			isCopy = false;

			CImage<unsigned char>* pTargetMask = pTargetSeries->getMask(i);
			isCopy = pTargetMask->copyImage(ppucMasks[i], nWidth, nHeight);

			if(LAZY_LOADING && !isCopy) {
				if(loadImage(pTargetMask, true)) {
					isCopy = pTargetMask->copyImage(ppucMasks[i], nWidth, nHeight);
				}
			}
		}
	}

	return isCopy;
}
bool CData::copyMasks(int nSeriesIdx, unsigned char** &ppucMasks, int &nMaskCnt, int &nWidth, int &nHeight, int &nChannel) {
	bool isCopy = false;
	CSeries* pTargetSeries = getSeries(nSeriesIdx);

	if(ppucMasks == NULL && pTargetSeries) {
		nMaskCnt = pTargetSeries->getImageCount();
		ppucMasks = new unsigned char* [nMaskCnt];
		memset(ppucMasks, 0, sizeof(unsigned char*)*nMaskCnt);

		for(int i=0; i< nMaskCnt; i++) {
			isCopy = false;

			CImage<unsigned char>* pTargetMask = pTargetSeries->getMask(i);
			isCopy = pTargetMask->copyImage(ppucMasks[i], nWidth, nHeight, nChannel);

			if(LAZY_LOADING && !isCopy) {
				if(loadImage(pTargetMask, true)) {
					isCopy = pTargetMask->copyImage(ppucMasks[i], nWidth, nHeight, nChannel);
				}
			}
		}
	}

	return isCopy;
}
bool CData::copyMasks(int nStartSliceIdx, int nEndSliceIdx, unsigned char** &ppucMasks, int &nMaskCnt, int &nWidth, int &nHeight) {
	bool isCopy = false;

	nMaskCnt = nEndSliceIdx - nStartSliceIdx + 1;
	ppucMasks = new unsigned char* [nMaskCnt];
	memset(ppucMasks, 0, sizeof(unsigned char*) * nMaskCnt);

	for(int i=0; i< nMaskCnt; i++) {
		isCopy = copyMask(i+nStartSliceIdx, ppucMasks[i], nWidth, nHeight);
	}

	return isCopy;
}
bool CData::copyMasks(int nStartSliceIdx, int nEndSliceIdx, unsigned char** &ppucMasks, int &nMaskCnt, int &nWidth, int &nHeight, int &nChannel) {
	bool isCopy = false;

	nMaskCnt = nEndSliceIdx - nStartSliceIdx + 1;
	ppucMasks = new unsigned char* [nMaskCnt];
	memset(ppucMasks, 0, sizeof(unsigned char*) * nMaskCnt);

	for(int i=0; i< nMaskCnt; i++) {
		isCopy = copyMask(i+nStartSliceIdx, ppucMasks[i], nWidth, nHeight, nChannel);
	}

	return isCopy;
}

int CData::getWidth(int nSliceIdx) {
	CImage<short>* pTargetImage = getCImage(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getWidth();
	}
	return 0;
}
int CData::getWidth(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getWidth();
	}
	return 0;
}
int CData::getHeight(int nSliceIdx) {
	CImage<short>* pTargetImage = getCImage(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getHeight();
	}
	return 0;
}
int CData::getHeight(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getHeight();
	}
	return 0;
}
int CData::getChannel(int nSliceIdx) {
	CImage<short>* pTargetImage = getCImage(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getChannel();
	}
	return 0;
}
int CData::getChannel(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getChannel();
	}
	return 0;
}

std::string CData::getImagePath(int nSliceIdx) {
	CImage<short>* pTargetImage = getCImage(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getImagePath();
	}
	return "";
}
std::string CData::getImagePath(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getImagePath();
	}
	return "";
}
std::string CData::getImageName(int nSliceIdx) {
	CImage<short>* pTargetImage = getCImage(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getImageName();
	}
	return "";
}
std::string CData::getImageName(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getImageName();
	}
	return "";
}
std::string CData::getImageExtension(int nSliceIdx) {
	CImage<short>* pTargetImage = getCImage(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getImageExtension();
	}
	return "";
}
std::string CData::getImageExtension(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getImageExtension();
	}
	return "";
}

void CData::getImagePosition(int nSliceIdx, float& fImagePositionX, float& fImagePositionY, float& fImagePositionZ) {
	CImage<short>* pTargetImage = getCImage(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getImagePosition(fImagePositionX, fImagePositionY, fImagePositionZ);
	}
}
void CData::getImagePosition(int nSeriesIdx, int nImageIdx, float& fImagePositionX, float& fImagePositionY, float& fImagePositionZ) {
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getImagePosition(fImagePositionX, fImagePositionY, fImagePositionZ);
	}
}
void CData::getImageOrientation(int nSliceIdx, float& fImageOrientationRowX, float& fImageOrientationRowY, float& fImageOrientationRowZ, float& fImageOrientationColX, float& fImageOrientationColY, float& fImageOrientationColZ) {
	CImage<short>* pTargetImage = getCImage(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getImageOrientation(fImageOrientationRowX, fImageOrientationRowY, fImageOrientationRowZ, fImageOrientationColX, fImageOrientationColY, fImageOrientationColZ);
	}
}
void CData::getImageOrientation(int nSeriesIdx, int nImageIdx, float& fImageOrientationRowX, float& fImageOrientationRowY, float& fImageOrientationRowZ, float& fImageOrientationColX, float& fImageOrientationColY, float& fImageOrientationColZ) {
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getImageOrientation(fImageOrientationRowX, fImageOrientationRowY, fImageOrientationRowZ, fImageOrientationColX, fImageOrientationColY, fImageOrientationColZ);
	}
}
float CData::getPixelSpacing(int nSliceIdx) {
	CImage<short>* pTargetImage = getCImage(nSliceIdx);

	float fPixelSpacingX=0, fPixelSpacingY=0;
	if(pTargetImage) {
		pTargetImage->getPixelSpacing(fPixelSpacingX, fPixelSpacingY);
	}
	
	return fPixelSpacingX;		// 대부분 X,Y가 동일하니....
}
float CData::getPixelSpacing(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);

	float fPixelSpacingX=0, fPixelSpacingY=0;
	if(pTargetImage) {
		pTargetImage->getPixelSpacing(fPixelSpacingX, fPixelSpacingY);
	}

	return fPixelSpacingX;		// 대부분 X,Y가 동일하니....
}
void CData::getPixelSpacing(int nSliceIdx, float& fPixelSpacingX, float& fPixelSpacingY) {
	CImage<short>* pTargetImage = getCImage(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getPixelSpacing(fPixelSpacingX, fPixelSpacingY);
	}
	else {
		fPixelSpacingX = 0;
		fPixelSpacingY = 0;
	}
}
void CData::getPixelSpacing(int nSeriesIdx, int nImageIdx, float& fPixelSpacingX, float& fPixelSpacingY) {
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getPixelSpacing(fPixelSpacingX, fPixelSpacingY);
	}
	else {
		fPixelSpacingX = 0;
		fPixelSpacingY = 0;
	}
}
float CData::getSliceThickness(int nSliceIdx) {
	CImage<short>* pTargetImage = getCImage(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getSliceThickness();
	}
	return 0;
}
float CData::getSliceThickness(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getSliceThickness();
	}
	return 0;
}
void CData::getWLWW(int nSliceIdx, int& nWL, int& nWW) {
	CImage<short>* pTargetImage = getCImage(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getWLWW(nWL, nWW);
	}
	else {
		nWL = 0;
		nWW = 0;
	}
}
void CData::getWLWW(int nSeriesIdx, int nImageIdx, int& nWL, int& nWW) {
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getWLWW(nWL, nWW);
	}
	else {
		nWL = 0;
		nWW = 0;
	}
}
int CData::getInstanceNumber(int nSliceIdx) {
	CImage<short>* pTargetImage = getCImage(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getInstanceNumber();
	}
	return -1;
}
int CData::getInstanceNumber(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getInstanceNumber();
	}
	return -1;
}

////////////////////////////////////////////////////////////////////
bool CData::isMask_namingRule(std::string sPath) {
	std::string sFileName = getFileName(sPath);
	transform(sFileName.begin(), sFileName.end(), sFileName.begin(), ::tolower);	// 파일명 소문자로

	// 1. 파일명의 첫 두글자가 "m_"으로 시작하는 경우
	if(sFileName.substr(0, 2).compare("m_") == 0) {
		return true;
	}
	// 2. 파일명에 "_m_" 문자열이 포함된 경우
	if(sFileName.find("_m_") != std::string::npos) {
		return true;
	}
	// 3. 파일명에 "mask" 또는 "label" 단어를 포함하는 경우
	if(sFileName.find("mask") != std::string::npos || sFileName.find("label") != std::string::npos) {
		return true;
	}
	// 4. 상위 디렉토리 명이 mask 또는 label 단어를 포함하는 경우
	std::vector<std::string> paths = splitPath(sPath);
	if(paths.size() >= 2) {
		std::string sParentFolderName = paths[paths.size()-2];
		if(sParentFolderName.find("mask") != std::string::npos || sParentFolderName.find("label") != std::string::npos) {
			return true;
		}
	}
	// 5. 파일명의 확장자가 tiff인 경우
	std::string sFileExtension = getFileExtension(sFileName);
	if(sFileExtension == "tiff") {
		return true;
	}

	return false;
}
template<typename T>
bool CData::isMask_imageRule(CImage<T>* pCiImage) {
	// 1. pixel 값이 binary mask거나, 1~10까지의 label을 갖는 이미지 (0~10, 255)
	if(pCiImage->m_nChannel == 1) {
		for(int r=0; r< pCiImage->m_nHeight; r++) {
			for(int c=0; c< pCiImage->m_nWidth; c++) {
				int idx = r*pCiImage->m_nWidth + c;
				unsigned char val = (T)pCiImage->m_image[idx];
				
				if( !(val >= 0 && val <= 10 && val == 255)) {
					return false;
				}
			}
		}
	}

	return false;
}

template<typename T>
void CData::setPatientName(CSeries* pCiSeries, std::vector<CImage<T>*> pCiImages) {
	if(pCiSeries) {
		CImage<T>* pCiImage = NULL;
		int nImageCnt = pCiImages.size();

		if(nImageCnt > 0) {
			pCiImage = pCiImages[0];
			std::string sImagePath = pCiImage->getImagePath();

			// 1. imagePath 기준으로 상위 n단계 폴더명
			std::vector<std::string> paths = splitPath(sImagePath);
			if(nImageCnt == 1) {
				// 한 파일에 이미지가 1개 들어있을 때
				if(paths.size()-4 >= 0) {
					pCiSeries->m_sPatientName = paths[paths.size()-4];
				}
			}
			else if(nImageCnt > 1) {
				// 한 파일에 이미지가 n개 들어있을 때
				if(paths.size()-3 >= 0) {
					pCiSeries->m_sPatientName = paths[paths.size()-3];
				}
			}
			
			// 
		}
		
	}
}
template<typename T>
void CData::setStudyName(CSeries* pCiSeries, std::vector<CImage<T>*> pCiImages) {
	if(pCiSeries) {
		CImage<T>* pCiImage = NULL;
		int nImageCnt = pCiImages.size();

		if(nImageCnt > 0) {
			pCiImage = pCiImages[0];
			std::string sImagePath = pCiImage->getImagePath();

			// 1. imagePath 기준으로 상위 2단계 폴더명
			std::vector<std::string> paths = splitPath(sImagePath);
			if(nImageCnt == 1) {
				// 한 파일에 이미지가 1개 들어있을 때
				if(paths.size()-3 >= 0) {
					pCiSeries->m_sStudyName = paths[paths.size()-3];
				}
			}
			else if(nImageCnt > 1) {
				// 한 파일에 이미지가 n개 들어있을 때
				if(paths.size()-2 >= 0) {
					pCiSeries->m_sStudyName = paths[paths.size()-2];
				}
			}

			// 
		}
	}
}
template<typename T>
void CData::setSeriesName(CSeries* pCiSeries, std::vector<CImage<T>*> pCiImages) {
	if (pCiSeries) {
		CImage<T>* pCiImage = NULL;
		int nImageCnt = pCiImages.size();

		if (nImageCnt > 0) {
			pCiImage = pCiImages[0];
			std::string sImagePath = pCiImage->getImagePath();

			// 1. imagePath 기준으로 상위 2단계 폴더명
			std::vector<std::string> paths = splitPath(sImagePath);
			if (nImageCnt == 1) {
				// 한 파일에 이미지가 1개 들어있을 때
				if (paths.size() - 2 >= 0) {
					std::string seriesName = paths[paths.size()-2];
					replaceAll(seriesName, std::string("_mask"), std::string(""));
					replaceAll(seriesName, std::string("_Mask"), std::string(""));

					pCiSeries->m_sSeriesName = seriesName;
				}
			}
			else if (nImageCnt > 1) {
				// 한 파일에 이미지가 n개 들어있을 때
				if (paths.size() - 1 >= 0) {
					std::string seriesName = getFileName(paths[paths.size() - 1]); // "Portal_Ax_Mask"
					replaceAll(seriesName, std::string("_mask"), std::string(""));
					replaceAll(seriesName, std::string("_Mask"), std::string(""));

					pCiSeries->m_sSeriesName = seriesName;
				}
			}
		}
	}
}

template<typename T1, typename T2>
bool CData::isSameImage(CImage<T1>* pCiImage1, CImage<T2>* pCiImage2) {
	bool isSame = false;

	if(pCiImage1 != NULL && pCiImage2 != NULL) {
		// 1. 이름이 동일할 때
		if(pCiImage1->getImageName().compare(pCiImage2->getImageName()) == 0) {
			isSame = true;
		}
	}

	return isSame;
}
bool CData::isSameSeries(CSeries* pCiSeries1, CSeries* pCiSeries2) {
	bool isSame = false;

	if(pCiSeries1 != NULL && pCiSeries2 != NULL) {
		// 1. PatientName, SeriesName이 같은 경우
		if ((pCiSeries1->m_sPatientName.compare(pCiSeries2->m_sPatientName) == 0) && (pCiSeries1->m_sSeriesName.compare(pCiSeries2->m_sSeriesName) == 0)) {
			isSame = true;
		}

		/*
		// 1. Series Instance UID가 같은 경우
		if(pCiSeries1->m_sSeriesInstanceUID.compare(pCiSeries2->m_sSeriesInstanceUID) == 0) {
			isSame = true;
		}
		// 2. FrameOfReferenceUID가 같은 경우
		else if(pCiSeries1->m_sFrameOfReferenceUID.compare(pCiSeries2->m_sFrameOfReferenceUID) == 0) {
			isSame = true;
		}
		*/
	}

	return isSame;
}

template <typename T>
bool CData::sorting_fileName(CImage<T>* &pCiImage1, CImage<T>* &pCiImage2) {
	// 문자열 비교 (윈도우 탐색기 정렬) //
	std::string s1 = pCiImage1->getImageName();
	std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
	std::string s2 = pCiImage2->getImageName();
	std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);	// 소문자

	// ignore common prefix..
	int i = 0;
	while ((i < s1.length()) && (i < s2.length()) && (s1.at(i) == s2.at(i)))
		++i;
	++i;
	// something left to compare?
	if ((i < s1.length()) && (i < s2.length()))
	{
		// get number prefix from position i - doesnt matter from which string
		int k = i-1;
		//If not number return native comparator
		if(!isdigit(s1.at(k)) || !isdigit(s2.at(k)))
		{
			//Two next lines
			//E.g. 1_... < 12_...
			if(isdigit(s1.at(k)))
				return false;
			if(isdigit(s2.at(k)))
				return true;
			return s1.compare(s2) < 0;
		}
		std::string n = "";
		k--;
		while ((k >= 0) && (isdigit(s1.at(k))))
		{
			n = s1.at(k)+n;
			--k;
		}
		// get relevant/signficant number string for s1
		k = i-1;
		std::string n1 = "";
		while ((k < s1.length()) && (isdigit(s1.at(k))))
		{
			n1 += s1.at(k);
			++k;
		}

		// get relevant/signficant number string for s2
		//Decrease by
		k = i-1;
		std::string n2 = "";
		while ((k < s2.length()) && (isdigit(s2.at(k))))
		{
			n2 += s2.at(k);
			++k;
		}

		// got two numbers to compare?
		if (!n1.empty() && !n2.empty())
			return atoi((n+n1).c_str()) < atoi((n+n2).c_str());
		else
		{
			// not a number has to win over a number.. number could have ended earlier... same prefix..
			if (!n1.empty())
				return false;
			if (!n2.empty())
				return true;
			return s1.at(i) < s2.at(i);
		}
	}
	else
	{
		// shortest string wins
		return s1.length() < s2.length();
	}
}
template <typename T>
bool CData::sorting_imagePosition(CImage<T>* &pCiImage1, CImage<T>* &pCiImage2) {
	// position Z로 Sorting
	float fImagePositionX1, fImagePositionY1, fImagePositionZ1;
	float fImagePositionX2, fImagePositionY2, fImagePositionZ2;

	pCiImage1->getImagePosition(fImagePositionX1, fImagePositionY1, fImagePositionZ1);
	pCiImage2->getImagePosition(fImagePositionX2, fImagePositionY2, fImagePositionZ2);

	return fImagePositionZ1 < fImagePositionZ2;
}

////////////////////////////////////////////////////////////////////
template <typename T>
bool CData::loadImage(std::string sImagePath, CSeries* &pCiSeries, std::vector<CImage<T>*> &ciImages) {
	// series header 정보, image header 정보까지 모두 읽어야 할 때 사용

	// exception
	if(pCiSeries != NULL) {
		return false;
	}

	const char* pcImagePath = sImagePath.c_str();
	std::string sImageName = getFileName(sImagePath);
	std::string sImageExtension = getFileExtension(sImagePath);

	// 이미지 load, 이미지 정보로 일치하는 Series를 찾고, 없으면 새로운 Series를 열어서 반환
	if(sImageExtension == "dcm") {
		DcmDataset* dataSet = NULL;
		std::vector<short*> images;
		
		if(loadDICOM_dcmtk(pcImagePath, dataSet, images, LAZY_LOADING)) {

			OFString OFStrPatientID, OFStrPatientName;
			OFString OFStrStudyInstanceUID, OFStrStudyDescription, OFStrStudyDate;
			OFString OFStrSeriesInstanceUID, OFStrFrameOfReferenceUID;
			OFString OFStrSeriesDate, OFStrSeriesDescription, OFStrModality;
			OFString OFStrInstanceNumber, OFStrRows, OFStrColumns, OFStrPixelSpacing, OFStrSliceThickeness;
			OFString OFStrImagePosition, OFStrImageOrientation;
			OFString OFStrWindowLevel, OFStrWindowWidth;

			// series
			dataSet->findAndGetOFString(DcmTagKey(0x0010, 0x0020), OFStrPatientID);
			dataSet->findAndGetOFString(DcmTagKey(0x0010, 0x0010), OFStrPatientName);

			dataSet->findAndGetOFString(DcmTagKey(0x0020, 0x000D), OFStrStudyInstanceUID);
			dataSet->findAndGetOFString(DcmTagKey(0x0008, 0x1030), OFStrStudyDescription);
			dataSet->findAndGetOFString(DcmTagKey(0x0008, 0x0020), OFStrStudyDate);
			
			dataSet->findAndGetOFString(DcmTagKey(0x0020, 0x000E), OFStrSeriesInstanceUID);
			dataSet->findAndGetOFString(DcmTagKey(0x0020, 0x0052), OFStrFrameOfReferenceUID);
			dataSet->findAndGetOFString(DcmTagKey(0x0008, 0x0021), OFStrSeriesDate);
			dataSet->findAndGetOFString(DcmTagKey(0x0008, 0x103E), OFStrSeriesDescription);
			dataSet->findAndGetOFString(DcmTagKey(0x0008, 0x0060), OFStrModality);

			// image
			dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x0010), OFStrRows);
			dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x0011), OFStrColumns);
			dataSet->findAndGetOFStringArray(DcmTagKey(0x0028, 0x0030), OFStrPixelSpacing);
			dataSet->findAndGetOFString(DcmTagKey(0x0018, 0x0050), OFStrSliceThickeness);

			dataSet->findAndGetOFStringArray(DcmTagKey(0x0020, 0x0032), OFStrImagePosition);
			dataSet->findAndGetOFStringArray(DcmTagKey(0x0020, 0x0037), OFStrImageOrientation);

			dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x1050), OFStrWindowLevel);
			dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x1051), OFStrWindowWidth);
			dataSet->findAndGetOFString(DcmTagKey(0x0020, 0x0013), OFStrInstanceNumber);

			// Image 정보
			int nWidth = atoi(OFStrColumns.c_str());
			int nHeight = atoi(OFStrRows.c_str());
			int nChannel = 1;
			std::vector<std::string> sImagePosition = splitPath(OFStrImagePosition.c_str());
			float fImagePositionX=0, fImagePositionY=0, fImagePositionZ=0;
			if(sImagePosition.size() == 3) {
				fImagePositionX = atof(sImagePosition[0].c_str());
				fImagePositionY = atof(sImagePosition[1].c_str());
				fImagePositionZ = atof(sImagePosition[2].c_str());
			}
			std::vector<std::string> sImageOrientation = splitPath(OFStrImageOrientation.c_str());
			float fImageOrientationRowX=0, fImageOrientationRowY=0, fImageOrientationRowZ=0;
			float fImageOrientationColX=0, fImageOrientationColY=0, fImageOrientationColZ=0;
			if(sImageOrientation.size() == 6) {
				fImageOrientationRowX = atof(sImageOrientation[0].c_str());
				fImageOrientationRowY = atof(sImageOrientation[1].c_str());
				fImageOrientationRowZ = atof(sImageOrientation[2].c_str());
				fImageOrientationColX = atof(sImageOrientation[3].c_str());
				fImageOrientationColY = atof(sImageOrientation[4].c_str());
				fImageOrientationColZ = atof(sImageOrientation[5].c_str());
			}
			std::vector<std::string> sPixelSpacing = splitPath(OFStrPixelSpacing.c_str());
			float fPixelSpacingX=0, fPixelSpacingY=0;
			if(sPixelSpacing.size() == 2) {
				fPixelSpacingX = atof(sPixelSpacing[0].c_str());
				fPixelSpacingY = atof(sPixelSpacing[1].c_str());
			}
			float fSliceThickness = atof(OFStrSliceThickeness.c_str());
			int nWL = atoi(OFStrWindowLevel.c_str());
			int nWW = atoi(OFStrWindowWidth.c_str());
			int nInstanceNumber = atoi(OFStrInstanceNumber.c_str());

			// 1. setImage //
			for(int idx=0, nIdx=images.size(); idx<nIdx; idx++) {
				short* psImage = images[idx];
				CImage<T>* pCiImage = new CImage<T>();

				pCiImage->setImagePath(sImagePath);
				pCiImage->setImageName(sImageName);
				pCiImage->setImageExtension(sImageExtension);
					
				pCiImage->setImagePosition(fImagePositionX, fImagePositionY, fImagePositionZ);
				pCiImage->setImageOrientation(fImageOrientationRowX, fImageOrientationRowY, fImageOrientationRowZ, fImageOrientationColX, fImageOrientationColY, fImageOrientationColZ);
				pCiImage->setPixelSpacing(fPixelSpacingX, fPixelSpacingY);
				pCiImage->setSliceThickness(fSliceThickness);
				pCiImage->setWLWW(nWL, nWW);
				pCiImage->setInstanceNumber(nInstanceNumber);

				// 자료형 확인 후, setImage
				if(typeid(T) == typeid(short)) {
					if(!LAZY_LOADING) {
						pCiImage->setImage((T*)psImage, nWidth, nHeight, nChannel);
					}
				}
				else {
					if(!LAZY_LOADING) {
						// loadDICOM_dcmtk는 short형만 반환하기 때문에, 다른 자료형이 들어오면 변환해야함. 자료형 차이로 인한 값 손실은 보장하지 않음!!
						int nSize = nWidth*nHeight*nChannel;
						T* temp = new T[nSize];
						for(int i=0, ni=nSize; i<ni; i++) {
							temp[i] = (T)psImage[i];
						}

						pCiImage->setImage(temp, nWidth, nHeight, nChannel);
						SAFE_DELETE_ARRAY(temp);
					}
				}

				ciImages.push_back(pCiImage);
				SAFE_DELETE_ARRAY(psImage);
			}

			// 2. Series //
			if(pCiSeries == NULL) {
				pCiSeries = new CSeries();
				pCiSeries->m_sPatientID = OFStrPatientID.c_str();
				pCiSeries->m_sPatientName = OFStrPatientName.c_str();

				pCiSeries->m_sStudyInstanceUID = OFStrStudyInstanceUID.c_str();
				pCiSeries->m_sStudyDescription = OFStrStudyDescription.c_str();
				pCiSeries->m_sStudyDate = OFStrStudyDate.c_str();

				pCiSeries->m_sSeriesInstanceUID = OFStrSeriesInstanceUID.c_str();
				pCiSeries->m_sFrameOfReferenceUID = OFStrFrameOfReferenceUID.c_str();
				pCiSeries->m_sSeriesDate = OFStrSeriesDate.c_str();
				pCiSeries->m_sSeriesDescription = OFStrSeriesDescription.c_str();
				pCiSeries->m_sModality = OFStrModality.c_str();
			}
		}

		// delete memory
		images.clear();
		SAFE_DELETE(dataSet);
	}
	else if(sImageExtension == "jpg" || sImageExtension == "png" || sImageExtension == "bmp") {
		unsigned char* pucImage = NULL;
		int nWidth, nHeight, nChannel;

		if(loadImage_opencv(pcImagePath, false, pucImage, nWidth, nHeight, nChannel, LAZY_LOADING)) {

			// 1. setImage //
			CImage<T>* pCiImage = new CImage<T>();

			pCiImage->setImagePath(sImagePath);
			pCiImage->setImageName(sImageName);
			pCiImage->setImageExtension(sImageExtension);

			pCiImage->setWLWW(256, 128);
			
			// 자료형 확인 후, setImage
			if(typeid(T) == typeid(unsigned char)) {
				if(!LAZY_LOADING) {
					pCiImage->setImage((T*)pucImage, nWidth, nHeight, nChannel);
				}
			}
			else {
				if(!LAZY_LOADING) {
					// loadImage_opencv는 unsigned char형만 반환하기 때문에, 다른 자료형이 들어오면 변환해야함. 자료형 차이로 인한 값 손실은 보장하지 않음!!
					int nSize = nWidth*nHeight*nChannel;
					T* temp = new T[nSize];
					for(int i=0, ni=nSize; i<ni; i++) {
						temp[i] = (T)pucImage[i];
					}
					pCiImage->setImage((T*)pucImage, nWidth, nHeight, nChannel);
		
					SAFE_DELETE_ARRAY(temp);
				}
			}

			// 2. Series //
			if(pCiSeries == NULL) {
				pCiSeries = new CSeries();
			}

			ciImages.push_back(pCiImage);
		}

		// delete memory
		SAFE_DELETE_ARRAY(pucImage);
	}
	else if(sImageExtension == "nii") {
		nifti_1_header nhdr;
		std::vector<short*> images;

		if(loadNII_libnii(pcImagePath, nhdr, images, LAZY_LOADING)) {
			
			// 1. setImage //
			int nWidth = nhdr.dim[1];
			int nHeight = nhdr.dim[2];
			int nChannel = 1;
			
			for(int idx=0, nIdx=images.size(); idx< nIdx; idx++) {
				short* psImage = images[idx];
				CImage<T>* pCiImage = new CImage<T>();

				pCiImage->setImagePath(sImagePath);
				pCiImage->setImageName(sImageName);
				pCiImage->setImageExtension(sImageExtension);

				pCiImage->setImagePosition(nhdr.qoffset_x, nhdr.qoffset_y, nhdr.qoffset_z);
				pCiImage->setImageOrientation(0, 0, 0, 0, 0, 0);	// 추후 확인 필요
				pCiImage->setPixelSpacing(nhdr.pixdim[1], nhdr.pixdim[2]);
				pCiImage->setSliceThickness(nhdr.dim[0]>=3? nhdr.pixdim[3] : 0);

				int nMaxIntensity = nhdr.cal_max;
				int nMinIntensity = nhdr.cal_min;
				int nWL = (nMaxIntensity + nMinIntensity) / 2.0;
				int nWW = (nMaxIntensity - nMinIntensity);
				pCiImage->setWLWW(nWL, nWW);
				pCiImage->setInstanceNumber(nhdr.slice_start + idx);

				// 자료형 확인 후, setImage
				if(typeid(T) == typeid(short)) {
					if(!LAZY_LOADING) {
						pCiImage->setImage((T*)psImage, nWidth, nHeight, nChannel);
					}
				}
				else {
					if(!LAZY_LOADING) {
						// loadDICOM_dcmtk는 short형만 반환하기 때문에, 다른 자료형이 들어오면 변환해야함. 자료형 차이로 인한 값 손실은 보장하지 않음!!
						int nSize = nWidth*nHeight*nChannel;
						T* temp = new T[nSize];
						for(int i=0, ni=nSize; i<ni; i++) {
							temp[i] = (T)psImage[i];
						}

						pCiImage->setImage(temp, nWidth, nHeight, nChannel);
						SAFE_DELETE_ARRAY(temp);
					}
				}

				ciImages.push_back(pCiImage);
				SAFE_DELETE_ARRAY(psImage);
			}

			// 2. Series //
			if(pCiSeries == NULL) {
				pCiSeries = new CSeries();
			}
		}

		images.clear();
	}
	else if(sImageExtension == "gz") {
		// todo
	}
	else if(sImageExtension == "tiff") {
		// todo
	}
	else {
		return false;
	}

	return true;
}
template <typename T>
bool CData::loadImage(CImage<T>* &pCiImage, bool isMask) {
	// image 정보를 이미 알고 있을 때 사용 (Lazy loading용)

	// exception
	if(pCiImage == NULL || pCiImage->getImagePath() == "") {
		return false;
	}

	// 
	std::string sImagePath = pCiImage->getImagePath();
	const char* pcImagePath = sImagePath.c_str();
	std::string sImageExtension = pCiImage->getImageExtension();
	if(sImageExtension == "dcm") {
		std::vector<short*> images;
		DcmDataset* dataSet = NULL;
		
		if(loadDICOM_dcmtk(pcImagePath, dataSet, images)) {
			std::vector<CImage<T>*> pCiTargetImages;
			
			// 1. 한 파일에 여러 image가 들어있는 경우, 매칭되는 image를 찾아줘야 함
			if(images.size() > 1) {
				CSeries* pCiTargetSeries = pCiImage->getSeries();
				int nImageCount = pCiTargetSeries->getImageCount();

				pCiTargetImages.reserve(nImageCount);
				for(int i=0, ni=nImageCount; i<ni; i++) {
					if(isMask) {
						pCiTargetImages.push_back((CImage<T>*)pCiTargetSeries->getMask(i));
					}
					else {
						pCiTargetImages.push_back((CImage<T>*)pCiTargetSeries->getImage(i));
					}
				}
			}
			else {
				pCiTargetImages.push_back(pCiImage);
			}

			// 2. setImage
			for(int s=0, ns=images.size(); s<ns; s++) {
				OFString OFStrRows, OFStrColumns, OFStrInstanceNumber;
				dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x0010), OFStrRows);
				dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x0011), OFStrColumns);
				dataSet->findAndGetOFString(DcmTagKey(0x0020, 0x0013), OFStrInstanceNumber);

				short* psImage = images[s];
				int nWidth = atoi(OFStrColumns.c_str());
				int nHeight = atoi(OFStrRows.c_str());
				int nChannel = 1;

				if(typeid(T) == typeid(short)) {
					pCiTargetImages[s]->setImage((T*)psImage, nWidth, nHeight, nChannel);
				}
				else {
					// loadDICOM_dcmtk는 short형만 반환하기 때문에, 다른 자료형이 들어오면 변환해야함. 자료형 차이로 인한 값 손실은 보장하지 않음!!
					int nSize = nWidth*nHeight*nChannel;
					T* temp = new T[nSize];
					for(int i=0, ni=nSize; i<ni; i++) {
						temp[i] = (T)psImage[i];
					}
					pCiTargetImages[s]->setImage(temp, nWidth, nHeight, nChannel);
					SAFE_DELETE_ARRAY(temp);
				}

				SAFE_DELETE_ARRAY(psImage);
			}

			//
			pCiTargetImages.clear();
		}

		images.clear();
		SAFE_DELETE(dataSet);
	}
	else if(sImageExtension == "jpg" || sImageExtension == "png" || sImageExtension == "bmp") {
		unsigned char* pucImage = NULL;
		int nWidth, nHeight, nChannel;

		if(loadImage_opencv(pcImagePath, false, pucImage, nWidth, nHeight, nChannel)) {
			// 1. setImage
			if(typeid(T) == typeid(unsigned char)) {
				pCiImage->setImage((T*)pucImage, nWidth, nHeight, nChannel);
			}
			else {
				// loadImage_opencv는 unsigned char형만 반환하기 때문에, 다른 자료형이 들어오면 변환해야함. 자료형 차이로 인한 값 손실은 보장하지 않음!!
				int nSize = nWidth*nHeight*nChannel;
				T* temp = new T[nSize];
				for(int i=0, ni=nSize; i<ni; i++) {
					temp[i] = (T)pucImage[i];
				}
				pCiImage->setImage((T*)pucImage, nWidth, nHeight, nChannel);
		
				SAFE_DELETE_ARRAY(temp);
			}

			SAFE_DELETE_ARRAY(pucImage);
		}
	}
	else if(sImageExtension == "nii") {
		std::vector<short*> images;
		nifti_1_header nhdr;

		if(loadNII_libnii(pcImagePath, nhdr, images)) {
			std::vector<CImage<T>*> pCiTargetImages;
			
			// 1. 한 파일에 여러 image가 들어있는 경우, 매칭되는 image를 찾아줘야 함
			if(images.size() > 1) {
				CSeries* pCiTargetSeries = pCiImage->getSeries();
				int nImageCount = pCiTargetSeries->getImageCount();

				pCiTargetImages.reserve(nImageCount);
				for(int i=0, ni=nImageCount; i<ni; i++) {
					if(isMask) {
						pCiTargetImages.push_back((CImage<T>*)pCiTargetSeries->getMask(i));
					}
					else {
						pCiTargetImages.push_back((CImage<T>*)pCiTargetSeries->getImage(i));
					}
				}
			}
			else {
				pCiTargetImages.push_back(pCiImage);
			}


			// 2. setImage
			int nWidth = nhdr.dim[1];
			int nHeight = nhdr.dim[2];
			int nChannel = 1;

			for(int s=0, ns=images.size(); s< ns; s++) {
				short* psImage = images[s];

				// 자료형 확인 후, setImage
				if(typeid(T) == typeid(short)) {
					pCiTargetImages[s]->setImage((T*)psImage, nWidth, nHeight, nChannel);
				}
				else {
					// loadNII_libnii는 short형만 반환하기 때문에, 다른 자료형이 들어오면 변환해야함. 자료형 차이로 인한 값 손실은 보장하지 않음!!
					int nSize = nWidth*nHeight*nChannel;
					T* temp = new T[nSize];
					for(int i=0, ni=nSize; i<ni; i++) {
						temp[i] = (T)psImage[i];
					}

					pCiTargetImages[s]->setImage(temp, nWidth, nHeight, nChannel);
					SAFE_DELETE_ARRAY(temp);
				}

				SAFE_DELETE_ARRAY(psImage);
			}

			pCiTargetImages.clear();
		}

		images.clear();
	}
	else if(sImageExtension == "gz") {

	}
	else if(sImageExtension == "tiff") {

	}
	else {
		return false;
	}

	return true;
}
bool CData::loadDICOM_dcmtk(const char* pcImagePath, DcmDataset* &dataSet, std::vector<short*> &images, bool bReadHeaderOnly) {
	// const
	int USHORT_MAX_VALUE = 65535;
	int SHORT_MIN_VALUE = -32768;
	int SHORT_MAX_VALUE = 32767;
	int DATA_MIN_HU_O = -3027;
	int DATA_MIN_HU_DEFAULT = -1024;

	// load dicom file
	DcmFileFormat* fileformat = NULL;
	try {
		// file format check
		fileformat = new DcmFileFormat();
		OFCondition status = fileformat->loadFile(pcImagePath);
		if (!status.good()) 
		{
			throw "file format";
		}
	}
	catch(char* err) {
		std::cerr << "DCMTK File error: " << err << std::endl;

		// memory clear
		if(fileformat != NULL) {
			delete fileformat;
		}
	}
	
	// load dicom image 준비
	dataSet = fileformat->getAndRemoveDataset();

	OFString OFStrNumberOfFrames, OFStrRows, OFStrColumns;
	dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x0008), OFStrNumberOfFrames);
	dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x0010), OFStrRows);
	dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x0011), OFStrColumns);

	int nImageCnt = atoi(OFStrNumberOfFrames.c_str());
	if(nImageCnt == 0) {
		nImageCnt = 1;
	}

	int nWidth = atoi(OFStrColumns.c_str());
	int nHeight = atoi(OFStrRows.c_str());
	int nSize = nWidth * nHeight;

	if(bReadHeaderOnly) {
		for(int idx=0, nIdx=nImageCnt; idx<nIdx; idx++) {
			short* psDCMImage = NULL;
			images.push_back(psDCMImage);
		}
	}
	else {
		for(int idx=0, nIdx=nImageCnt; idx<nIdx; idx++) {
			DicomImage* dcmImage = NULL;
			short* psDCMImage = NULL;

			try {
				dcmImage = new DicomImage(pcImagePath, 0, idx, 1);	// 확인 필요
				psDCMImage = new short[nSize];

				if(dcmImage->getStatus() == EIS_Normal) {
					if(dcmImage->isMonochrome()) {
						// image representation
						dcmImage->setMinMaxWindow();
						Uint8* uint8_PixelData = NULL;
						Sint8* sint8_PixelData = NULL;
						Uint16* uint16_PixelData = NULL;
						Sint16* sint16_PixelData = NULL;
						Uint32* uint32_PixelData = NULL;
						Sint32* sint32_PixelData = NULL;
						int iPixelDataMin = 999999, iPixelDataMax = -999999;

						switch(dcmImage->getInterData()->getRepresentation()) {
							case EPR_Uint8:
								uint8_PixelData = (Uint8*)(dcmImage->getInterData()->getData());
								if(uint8_PixelData != NULL)
								{
									for(int i=0; i<nWidth*nHeight; i++)
									{
										psDCMImage[i] = uint8_PixelData[i];

										if(iPixelDataMin > uint8_PixelData[i])
											iPixelDataMin = uint8_PixelData[i];
										if(iPixelDataMax < uint8_PixelData[i])
											iPixelDataMax = uint8_PixelData[i];
									}
								}
								break;
							case EPR_Sint8:
								sint8_PixelData = (Sint8*)(dcmImage->getInterData()->getData());
								if(sint8_PixelData != NULL)
								{
									for(int i=0; i<nWidth*nHeight; i++)
									{
										psDCMImage[i] = sint8_PixelData[i];

										if(iPixelDataMin > sint8_PixelData[i])
											iPixelDataMin = sint8_PixelData[i];
										if(iPixelDataMax < sint8_PixelData[i])
											iPixelDataMax = sint8_PixelData[i];
									}
								}
								break;
							case EPR_Uint16:
								uint16_PixelData = (Uint16*)(dcmImage->getInterData()->getData());
								if(uint16_PixelData != NULL)
								{
									for(int i=0; i<nWidth*nHeight; i++)
									{
										int iVal = uint16_PixelData[i];
										if(iVal > SHORT_MAX_VALUE)
										{
											//hu range overflow
											continue;
										}

										if(iPixelDataMin > iVal)
											iPixelDataMin = iVal;
										if(iPixelDataMax < iVal)
											iPixelDataMax = iVal;
									}

									if(iPixelDataMax!=-999999 && iPixelDataMin!=999999)
									{
										for(int i=0; i<nWidth*nHeight; i++)
										{
											int iVal = uint16_PixelData[i];

											if(iVal > SHORT_MAX_VALUE)
												iVal = iPixelDataMax;

											psDCMImage[i] = iVal;
										}
									}
								}
								break;
							case EPR_Sint16:
								sint16_PixelData = (Sint16*)(dcmImage->getInterData()->getData());
								if(sint16_PixelData != NULL)
								{
									for(int i=0; i<nWidth*nHeight; i++)
									{
										if(iPixelDataMin > sint16_PixelData[i])
											iPixelDataMin = sint16_PixelData[i];
										if(iPixelDataMax < sint16_PixelData[i])
											iPixelDataMax = sint16_PixelData[i];

										psDCMImage[i] = sint16_PixelData[i];
									}
								}
								break;
							case EPR_Uint32:
								uint32_PixelData = (Uint32*)(dcmImage->getInterData()->getData());
								if(uint32_PixelData != NULL)
								{
									for(int i=0; i<nWidth*nHeight; i++)
									{
										int iVal = uint32_PixelData[i];

										if(iVal > SHORT_MAX_VALUE)
										{
											//hu range overflow
											continue;
										}

										if(iPixelDataMin > iVal)
											iPixelDataMin = iVal;
										if(iPixelDataMax < iVal)
											iPixelDataMax = iVal;
									}

									if(iPixelDataMax!=-999999 && iPixelDataMin!=999999)
									{
										for(int i=0; i<nWidth*nHeight; i++)
										{
											int iVal = uint32_PixelData[i];

											if(iVal>SHORT_MAX_VALUE)
												iVal = iPixelDataMax;

											psDCMImage[i] = iVal;
										}
									}
								}
								break;
							case EPR_Sint32:
								sint32_PixelData = (Sint32*)(dcmImage->getInterData()->getData());
								if(sint32_PixelData != NULL)
								{
									for(int i=0; i<nWidth*nHeight; i++)
									{
										int iVal = sint32_PixelData[i];

										if(iVal>SHORT_MAX_VALUE || iVal<SHORT_MIN_VALUE)
										{
											//hu range overflow
											continue;
										}

										if(iPixelDataMin > iVal)
											iPixelDataMin = iVal;
										if(iPixelDataMax < iVal)
											iPixelDataMax = iVal;
									}

									if(iPixelDataMax!=-999999 && iPixelDataMin!=999999)
									{
										for(int i=0; i<nWidth*nHeight; i++)
										{
											int iVal = sint32_PixelData[i];

											if(iVal > SHORT_MAX_VALUE)
												iVal = iPixelDataMax;
											else if(iVal < SHORT_MIN_VALUE)
												iVal = iPixelDataMin;

											psDCMImage[i] = iVal;
										}
									}
								}
								break;
							default:
								throw "unknown representation error";
								break;
						}
					}
					else {
						// color image
					}
				
					// memory
					SAFE_DELETE(dcmImage);
					images.push_back(psDCMImage);
				}
			}
			catch(char* err) {
				std::cerr << "DCMTK Image error: " << err << std::endl;

				SAFE_DELETE(dataSet);
				SAFE_DELETE(dcmImage);
				SAFE_DELETE_ARRAY(psDCMImage);

				return false;
			}
		}
	}
	

	// delete memory
	fileformat->clear();
	delete fileformat;

	return true;
}
bool CData::loadImage_opencv(const char* pcImagePath, bool isColor, unsigned char* &pucImage, int &nWidth, int &nHeight, int &nChannel, bool bReadHeaderOnly) {
	if(pucImage != NULL) {
		return false;
	}

	// image 파일 로드
	cv::Mat image = cv::imread(pcImagePath, isColor);
	if(!image.data) {
		std::cerr << "Image load error" << std::endl;
		return false;
	}
	else {
		nWidth = image.cols;
		nHeight = image.rows;
		nChannel = image.channels();
		int nSize = nWidth * nHeight * nChannel;

		if(bReadHeaderOnly) {
			return true;
		}
		else {
			pucImage = new unsigned char[nSize];

			switch(nChannel) {
				case 1: 
				{
					for(int r=0; r< nHeight; r++) {
						unsigned char* pucRow = image.ptr<uchar>(r);

						for(int c=0; c< nWidth; c++) {
							int idx = r*nWidth + c;
							pucImage[idx] = pucRow[c];
						}
					}
					break;
				}
				case 3: 
				{
					for(int r=0; r< nHeight; r++) {
						unsigned char* pucRow = image.ptr<uchar>(r);

						for(int c=0; c< nWidth; c++) {
							int idx = (r*nWidth + c)*nChannel;
							pucImage[idx+0] = pucRow[c*3+0];
							pucImage[idx+1] = pucRow[c*3+1];
							pucImage[idx+2] = pucRow[c*3+2];
						}
					}
					break;
				}
			}
		}
	}

	return true;
}
bool CData::loadNII_libnii(const char* pcImagePath, nifti_1_header &nhdr, std::vector<short*> &images, bool bReadHeaderOnly) {
	//https://github.com/NIFTI-Imaging/nifti_clib

	nifti_image* nim = nifti_image_read(pcImagePath, 1);

	if(nim != NULL) {
		// copy header
		nhdr = nifti_convert_nim2nhdr(nim);	

		// 
		int nVoxOffset, nWidth, nHeight, nSliceCnt, nBitPix, nDataType;

		nVoxOffset = nhdr.vox_offset;
		nWidth = nhdr.dim[1];
		nHeight = nhdr.dim[2];
		nSliceCnt = nhdr.dim[0]>=3? nhdr.dim[3]: 1;
		nBitPix = nhdr.bitpix;
		nDataType = nhdr.datatype;

		// read image
		images.reserve(nSliceCnt);

		if(bReadHeaderOnly) {
			for(int s=0; s< nSliceCnt; s++) {
				short* psImage = NULL;
				images.push_back(psImage);
			}
		}
		else {
			for(int s=0; s< nSliceCnt; s++) {
				short* psImage = new short[nWidth*nHeight];
				memset(psImage, 0, sizeof(short)*nWidth*nHeight);

				for(int row=0; row< nHeight; row++) {
					for(int col=0; col < nWidth; col++) {
						int idx = col + row*nWidth;
						int niiIdx = (col + row*nWidth + s*nWidth*nHeight)*(nBitPix/8);

						switch(nBitPix) {
							case 8: 
							{
								char* data = (char*)nim->data + niiIdx;
								psImage[idx] = (short) *data;
								break;
							}
							case 16: 
							{
								short* data = (short*)nim->data + niiIdx;
								psImage[idx] = *data;
								break;
							}
						}
					}
				}

				images.push_back(psImage);
			}
		}
		
		
		nifti_image_free(nim);
	}
	else {
		return false;
	}
	
	return true;
}
bool CData::loadTIFF_libtiff(const char* pcImagePath, std::vector<unsigned char*> &images, bool bReadImageOnly) {
	TIFF *pMultiPageTiff = TIFFOpen(pcImagePath, "r");

	if(pMultiPageTiff != NULL) {
		int nWidth=0, nHeight=0, nDepth=0;

		TIFFGetField(pMultiPageTiff, TIFFTAG_IMAGEWIDTH, &nWidth);
		TIFFGetField(pMultiPageTiff, TIFFTAG_IMAGELENGTH, &nHeight);
		TIFFGetField(pMultiPageTiff, TIFFTAG_BITSPERSAMPLE, &nDepth); 

		//This should be Width*(depth / sizeof(char))
		tsize_t ScanlineSizeBytes = TIFFScanlineSize(pMultiPageTiff);
	
		if(pMultiPageTiff) {
			do {
				// copy
				unsigned char *pucBuffer = new unsigned char[nWidth*nHeight];
				for(int row = 0; row < nHeight; row++){
					TIFFReadScanline(pMultiPageTiff, &pucBuffer[row*nWidth], row, 0);
				}

				images.push_back(pucBuffer);

			} while(TIFFReadDirectory(pMultiPageTiff));

			TIFFClose(pMultiPageTiff);
		}
	}
	else {
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////
bool CData::isFile(std::string sPath) {
	struct _finddata_t fd;
	intptr_t handle;

	if ((handle = _findfirst(sPath.c_str(), &fd)) == -1L) {
		return false;
	}
	return isFile(fd);
}
bool CData::isFile(_finddata_t fd) {
	if (fd.attrib & _A_SUBDIR)
		return false;
	else
		return true;
}
void CData::searchingDir(std::string path, std::list<std::string>& sFilePaths) {
	int checkDirFile = 0;
	std::string dirPath = path + "/*.*";
	struct _finddata_t fd;
	intptr_t handle;

	if ((handle = _findfirst(dirPath.c_str(), &fd)) == -1L)
	{
		//파일이나 디렉토리가 없을 경우
		std::cout << "No file in directory!" << std::endl;
		return ;
	}

	// 파일 혹은 디렉토리 탐색
	do
	{
		checkDirFile = isFile(fd);
		if (checkDirFile == 0 && fd.name[0] != '.') {
			searchingDir(path + "/" + fd.name, sFilePaths);
		}
		else if (checkDirFile == 1 && fd.name[0] != '.') {
			sFilePaths.push_back(path + "/" + fd.name);
		}

	} while (_findnext(handle, &fd) == 0);
	_findclose(handle);
}
std::string CData::getFileExtension(std::string sFilePath) {
	int nFindFileExtensionIdx = sFilePath.rfind(".") + 1;

	return sFilePath.substr(nFindFileExtensionIdx);
}
std::string CData::getFileName(std::string sFilePath) {
	int nFindFileNameIdx = sFilePath.rfind("/") + 1;
	int nFindFileExtensionIdx = sFilePath.rfind(".");

	return(sFilePath.substr(nFindFileNameIdx, nFindFileExtensionIdx - nFindFileNameIdx));
}
std::vector<std::string> CData::splitPath(std::string sPath) {
	std::string d = "\\/";
	std::vector<std::string> out;
	std::vector<int> de;
	de.push_back(-1);

	for(int i=0, ni=sPath.size(); i<ni; i++) {
		if(d.find(sPath[i]) == -1)
			continue;
		de.push_back(i);
	}
	de.push_back(sPath.size());

	for(int i=1, ni=de.size(); i<ni; i++) {
		int s = de[i-1] + 1;
		int e = de[i] - 1;
		if(s>e)
			continue;
		out.push_back(sPath.substr(s, e-s+1));
	}

	return out;
}
std::string CData::replaceAll(std::string &str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}