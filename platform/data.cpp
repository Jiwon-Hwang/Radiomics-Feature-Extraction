#include "data.h"
#include "platform.h"

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

void CData::readDir(std::string sPath, bool doClear) {
	// 0. 메모리 초기화
	if(doClear) {
		clear();
	}

	// 1. image 파일 혹은 폴더를 순회하면서 읽음
	// Path에 있는 File 혹은 Dir를 순회하면서 읽어야 할 FIle 목록을 생성 //
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
		std::string sImagePath = *it;
		readImage(sPath);
	}

	// 2. 매칭되지 못했던 mask를 매칭시킴
	matchingImageAndMask();

	// 3. 전체 Series를 순회하면서, Image, Mask 순서 Sorting
	sortingImageAndMask();
}
void CData::readImage(std::string sImagePath) {	
	bool isLoad = false;
	bool isMask = false;
	CSeries* pCiSeries = NULL;
	CImage<short>* pCiImage = NULL;
	CImage<unsigned char>* pCiMask = NULL;

	// 1-1. 파일명 혹은 폴더명으로 mask여부 판별 //
	isMask = isMask_namingRule(sImagePath);

	// 1-2. 이미지를 Load 및 mask여부 판별, 파일명 혹은 폴더명으로 이미 구분된 경우, 별도 검사하지 않음 //
	if(isMask) {
		// 파일명, 확장자 등으로 Mask로 구분이 이미 된 경우
		isLoad = loadImage(sImagePath, pCiSeries, pCiMask);
	}
	else {
		// Image인지 Mask인지 모르는 경우
		isLoad = loadImage(sImagePath, pCiSeries, pCiImage);
		if(!isLoad) {
			// 읽을 수 없는 파일인 경우
			return;
		}
		isMask = isMask_imageRule(pCiImage);

		if(isMask) {
			// Mask인 경우, 자료형을 short->unsigned char로 바꿔줌 (자료형 변환으로 인한 값 손실이 발생할 수 있음)
			pCiImage->copyOtherType(pCiMask);
			SAFE_DELETE(pCiImage);
		}
	}

	// 1-3. Series에 load한 이미지 매칭 //
	if(isLoad) {
		if(!isMask) {
			// Image인 경우
			CSeries* pCiSeriesInList = NULL;
			for(int i=0, ni=getSeriesCount(); i<ni; i++) {
				if(isSameSeries(pCiSeries, getSeries(i))) {
					pCiSeriesInList = m_seriesList[i];
					break;
				}
			}

			if(pCiSeriesInList != NULL) {
				// Series가 중복되는 경우
				pCiSeriesInList->addImage(pCiImage);	// 

				pCiImage->copyOtherType(pCiMask);
				memset(pCiMask->m_image, 0, sizeof(unsigned char)*pCiMask->getSize());
				pCiSeriesInList->addMask(pCiMask);

				SAFE_DELETE(pCiSeries);
			}
			else {
				// Series가 중복되지 않는 경우
				pCiSeries->addImage(pCiImage);

				pCiImage->copyOtherType(pCiMask);
				memset(pCiMask->m_image, 0, sizeof(unsigned char)*pCiMask->getSize());
				pCiSeries->addMask(pCiMask);

				addSeries(pCiSeries);
			}
		}
		else {
			// Mask인 경우
			CSeries* pCiMatchedSeries = NULL;
			int nMatchedImageIdx = -1;

			for(int i=0, ni=getSeriesCount(); i<ni; i++) {
				for(int j=0, nj=getSeries(i)->getImageCount(); j<nj; j++) {
					if(isSameImage(pCiMask, getImagePtr(i, j))) {
						pCiMatchedSeries = getSeries(i);
						nMatchedImageIdx = j;
						break;
					}
				}
			}

			if(pCiMatchedSeries != NULL && nMatchedImageIdx != -1) {
				// 매칭되는 Series와 Image가 있는 경우
				pCiMatchedSeries->setMask(nMatchedImageIdx, pCiMask);
			}
			else {
				// 매칭되는 Image가 없는 경우 (나중에 매칭할 수 있도록 일단 저장)
				m_nonMatchedMasks.push_back(pCiMask);
			}
			SAFE_DELETE(pCiSeries);
		}
	}
}
void CData::matchingImageAndMask() {
	// Image와 매칭되지 못한 Mask들을 순회하면서 matching
	for(std::list<CImage<unsigned char>*>::iterator it=m_nonMatchedMasks.begin(); it!=m_nonMatchedMasks.end(); ++it) {
		CSeries* pCiMatchedSeries = NULL;
		int nMatchedImageIdx = -1;
		CImage<unsigned char>* pCiMask = *it;

		for(int i=0, ni=getSeriesCount(); i<ni; i++) {
			for(int j=0, nj=getSeries(i)->getImageCount(); j<nj; j++) {
				if(isSameImage(pCiMask, getImagePtr(i, j))) {
					pCiMatchedSeries = getSeries(i);
					nMatchedImageIdx = j;
					break;
				}
			}
		}

		// 일치하는 것 찾았으면 setMask, 못찾았으면 그냥 놔둠
		if(pCiMatchedSeries != NULL) {
			pCiMatchedSeries->setMask(nMatchedImageIdx, pCiMask);
			m_nonMatchedMasks.erase(it);
		}
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
bool CData::setSeries(CSeries* pCiSeries, int nTargetSeriesIdx) {
	if(pCiSeries == NULL) {
		return false;
	}

	if(nTargetSeriesIdx == -1) {
		// nTargetSeriesIdx가 명시되어 있지 않은 경우, 맨 뒤에 넣음
		addSeries(pCiSeries);
	}
	else {
		if(m_seriesList[nTargetSeriesIdx] != NULL) {
			SAFE_DELETE(m_seriesList[nTargetSeriesIdx]);
		}
		m_seriesList[nTargetSeriesIdx] = pCiSeries;
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
CImage<short>* CData::getImagePtr(int nSliceIdx) {
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
CImage<short>* CData::getImagePtr(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = NULL;
	
	if(nSeriesIdx >= 0 && nSeriesIdx < getSeriesCount()) {
		if(nImageIdx >= 0 && nImageIdx < getSeries(nSeriesIdx)->getImageCount()) {
			pTargetImage = getSeries(nSeriesIdx)->getImage(nImageIdx);
		}
	}

	return pTargetImage;
}
short* CData::getImage(int nSliceIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSliceIdx);
	return pTargetImage->getImage();
}

bool CData::setImage(int nSliceIdx, CImage<short>* pCiImage) {
	bool isSet = false;
	CImage<short>* pTargetImage = getImagePtr(nSliceIdx);

	if(pTargetImage) {
		isSet = pTargetImage->setImage(pCiImage);
	}

	return isSet;
}
bool CData::setImage(int nSliceIdx, short* psImage, int nWidth, int nHeight, int nChannel) {
	bool isSet = false;
	CImage<short>* pTargetImage = getImagePtr(nSliceIdx);

	if(pTargetImage) {
		isSet = pTargetImage->setImage(psImage, nWidth, nHeight);
	}

	return isSet;
}
bool CData::setImage(int nSeriesIdx, int nImageIdx, CImage<short>* pCiImage) {
	bool isSet = false;
	CImage<short>* pTargetImage = getSeries(nSeriesIdx)->getImage(nImageIdx);

	if(pTargetImage) {
		isSet = pTargetImage->setImage(pCiImage);
	}

	return isSet;
}
bool CData::setImage(int nSeriesIdx, int nImageIdx, short* psImage, int nWidth, int nHeight, int nChannel) {
	bool isSet = false;
	CImage<short>* pTargetImage = getSeries(nSeriesIdx)->getImage(nImageIdx);

	if(pTargetImage) {
		isSet = pTargetImage->setImage(psImage, nWidth, nHeight, nChannel);
	}

	return isSet;
}

bool CData::setImages(int nSeriesIdx, short** ppsImages, int nImageCnt, int nWidth, int nHeight, int nChannel) {
	bool isSet = false;
	CSeries* pTargetSeries = getSeries(nSeriesIdx);

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
	CImage<short>* pTargetImage = getImagePtr(nSliceIdx);

	if(pTargetImage) {
		isCopy = pTargetImage->copyImage(psImage, nWidth, nHeight);
	}

	return isCopy;
}
bool CData::copyImage(int nSliceIdx, short* &psImage, int &nWidth, int &nHeight, int &nChannel) {
	bool isCopy = false;
	CImage<short>* pTargetImage = getImagePtr(nSliceIdx);

	if(pTargetImage) {
		isCopy = pTargetImage->copyImage(psImage, nWidth, nHeight, nChannel);
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
			CImage<short>* pTargetImage = pTargetSeries->getImage(i);
			pTargetImage->copyImage(ppsImages[i], nWidth, nHeight);
		}
	}

	return isCopy;
}
bool CData::copyImages(int nSeriesIdx, short** &ppsImages, int &nImageCnt, int &nWidth, int &nHeight, int &nChannel) {
	bool isCopy = false;

	CSeries* pTargetSeries = getSeries(nSeriesIdx);

	if(ppsImages == NULL && pTargetSeries) {
		int nSeriesImageCnt = pTargetSeries->getImageCount();
		ppsImages = new short* [nSeriesImageCnt];
		memset(ppsImages, 0, sizeof(short*)*nSeriesImageCnt);

		for(int i=0; i< nSeriesImageCnt; i++) {
			CImage<short>* pTargetImage = pTargetSeries->getImage(i);
			isCopy = pTargetImage->copyImage(ppsImages[i], nWidth, nHeight);
			if(!isCopy) {
				return isCopy;
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
		if(!isCopy) {
			return isCopy;
		}
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
		if(!isCopy) {
			return isCopy;
		}
	}

	return isCopy;
}


CImage<unsigned char>* CData::getMaskPtr(int nSliceIdx) {
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
CImage<unsigned char>* CData::getMaskPtr(int nSeriesIdx, int nMaskIdx) {
	CImage<unsigned char>* pTargetMask = NULL;
	
	if(nSeriesIdx >= 0 && nSeriesIdx < getSeriesCount()) {
		if(nMaskIdx >= 0 && nMaskIdx < getSeries(nSeriesIdx)->getMaskCount()) {
			pTargetMask = getSeries(nSeriesIdx)->getMask(nMaskIdx);
		}
	}

	return pTargetMask;
}
unsigned char* CData::getMask(int nSliceIdx) {
	CImage<unsigned char>* pTargetMask = getMaskPtr(nSliceIdx);
	return pTargetMask->getImage();
}
bool CData::setMask(int nSliceIdx, CImage<unsigned char>* pCiMask) {
	bool isSet = false;
	CImage<unsigned char>* pTargetMask = getMaskPtr(nSliceIdx);

	if(pTargetMask) {
		isSet = pTargetMask->setImage(pCiMask);
	}

	return isSet;
}
bool CData::setMask(int nSliceIdx, unsigned char* pucMask, int nWidth, int nHeight, int nChannel) {
	bool isSet = false;
	CImage<unsigned char>* pTargetMask = getMaskPtr(nSliceIdx);

	if(pTargetMask) {
		isSet = pTargetMask->setImage(pucMask, nWidth, nHeight);
	}

	return isSet;
}
bool CData::setMask(int nSeriesIdx, int nImageIdx, CImage<unsigned char>* pCiMask) {
	bool isSet = false;
	CImage<unsigned char>* pTargetMask = getMaskPtr(nSeriesIdx, nImageIdx);

	if(pTargetMask) {
		isSet = pTargetMask->setImage(pCiMask);
	}

	return isSet;
}
bool CData::setMask(int nSeriesIdx, int nImageIdx, unsigned char* pucMask, int nWidth, int nHeight, int nChannel) {
	bool isSet = false;
	CImage<unsigned char>* pTargetMask = getMaskPtr(nSeriesIdx, nImageIdx);

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
	CImage<unsigned char>* pTargetMask = getMaskPtr(nSliceIdx);

	if(pTargetMask) {
		isCopy = pTargetMask->copyImage(pucMask, nWidth, nHeight);
	}

	return isCopy;
}
bool CData::copyMask(int nSliceIdx, unsigned char* &pucMask, int &nWidth, int &nHeight, int &nChannel) {
	bool isCopy = false;
	CImage<unsigned char>* pTargetMask = getMaskPtr(nSliceIdx);

	if(pTargetMask) {
		isCopy = pTargetMask->copyImage(pucMask, nWidth, nHeight, nChannel);
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
			CImage<unsigned char>* pTargetMask = pTargetSeries->getMask(i);
			isCopy = pTargetMask->copyImage(ppucMasks[i], nWidth, nHeight);
			if(!isCopy) {
				return isCopy;
			}
		}
	}

	return isCopy;
}
bool CData::copyMasks(int nSeriesIdx, unsigned char** &ppucMasks, int &nMaskCnt, int &nWidth, int &nHeight, int &nChannel) {
	bool isCopy = false;
	CSeries* pTargetSeries = getSeries(nSeriesIdx);

	if(ppucMasks == NULL && pTargetSeries) {
		int nSeriesImageCnt = pTargetSeries->getImageCount();
		ppucMasks = new unsigned char* [nSeriesImageCnt];
		memset(ppucMasks, 0, sizeof(unsigned char*)*nSeriesImageCnt);

		for(int i=0; i< nSeriesImageCnt; i++) {
			CImage<unsigned char>* pTargetMask = pTargetSeries->getMask(i);
			isCopy = pTargetMask->copyImage(ppucMasks[i], nWidth, nHeight, nChannel);
			if(!isCopy) {
				return isCopy;
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
		if(!isCopy) {
			return isCopy;
		}
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
		if(!isCopy) {
			return isCopy;
		}
	}

	return isCopy;
}

int CData::getWidth(int nSliceIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getWidth();
	}
	return 0;
}
int CData::getWidth(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getWidth();
	}
	return 0;
}
int CData::getHeight(int nSliceIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getHeight();
	}
	return 0;
}
int CData::getHeight(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getHeight();
	}
	return 0;
}
int CData::getChannel(int nSliceIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getChannel();
	}
	return 0;
}
int CData::getChannel(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getChannel();
	}
	return 0;
}

std::string CData::getImagePath(int nSliceIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getImagePath();
	}
	return "";
}
std::string CData::getImagePath(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getImagePath();
	}
	return "";
}
std::string CData::getImageName(int nSliceIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getImageName();
	}
	return "";
}
std::string CData::getImageName(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getImageName();
	}
	return "";
}
std::string CData::getImageExtension(int nSliceIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getImageExtension();
	}
	return "";
}
std::string CData::getImageExtension(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getImageExtension();
	}
	return "";
}

void CData::getImagePosition(int nSliceIdx, float& fImagePositionX, float& fImagePositionY, float& fImagePositionZ) {
	CImage<short>* pTargetImage = getImagePtr(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getImagePosition(fImagePositionX, fImagePositionY, fImagePositionZ);
	}
}
void CData::getImagePosition(int nSeriesIdx, int nImageIdx, float& fImagePositionX, float& fImagePositionY, float& fImagePositionZ) {
	CImage<short>* pTargetImage = getImagePtr(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getImagePosition(fImagePositionX, fImagePositionY, fImagePositionZ);
	}
}
void CData::getImageOrientation(int nSliceIdx, float& fImageOrientationRowX, float& fImageOrientationRowY, float& fImageOrientationRowZ, float& fImageOrientationColX, float& fImageOrientationColY, float& fImageOrientationColZ) {
	CImage<short>* pTargetImage = getImagePtr(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getImageOrientation(fImageOrientationRowX, fImageOrientationRowY, fImageOrientationRowZ, fImageOrientationColX, fImageOrientationColY, fImageOrientationColZ);
	}
}
void CData::getImageOrientation(int nSeriesIdx, int nImageIdx, float& fImageOrientationRowX, float& fImageOrientationRowY, float& fImageOrientationRowZ, float& fImageOrientationColX, float& fImageOrientationColY, float& fImageOrientationColZ) {
	CImage<short>* pTargetImage = getImagePtr(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getImageOrientation(fImageOrientationRowX, fImageOrientationRowY, fImageOrientationRowZ, fImageOrientationColX, fImageOrientationColY, fImageOrientationColZ);
	}
}
float CData::getPixelSpacing(int nSliceIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSliceIdx);

	float fPixelSpacingX=0, fPixelSpacingY=0;
	if(pTargetImage) {
		pTargetImage->getPixelSpacing(fPixelSpacingX, fPixelSpacingY);
	}
	
	return fPixelSpacingX;		// 대부분 X,Y가 동일하니....
}
float CData::getPixelSpacing(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSeriesIdx, nImageIdx);

	float fPixelSpacingX=0, fPixelSpacingY=0;
	if(pTargetImage) {
		pTargetImage->getPixelSpacing(fPixelSpacingX, fPixelSpacingY);
	}

	return fPixelSpacingX;		// 대부분 X,Y가 동일하니....
}
void CData::getPixelSpacing(int nSliceIdx, float& fPixelSpacingX, float& fPixelSpacingY) {
	CImage<short>* pTargetImage = getImagePtr(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getPixelSpacing(fPixelSpacingX, fPixelSpacingY);
	}
	else {
		fPixelSpacingX = 0;
		fPixelSpacingY = 0;
	}
}
void CData::getPixelSpacing(int nSeriesIdx, int nImageIdx, float& fPixelSpacingX, float& fPixelSpacingY) {
	CImage<short>* pTargetImage = getImagePtr(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getPixelSpacing(fPixelSpacingX, fPixelSpacingY);
	}
	else {
		fPixelSpacingX = 0;
		fPixelSpacingY = 0;
	}
}
float CData::getSliceThickness(int nSliceIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getSliceThickness();
	}
	return 0;
}
float CData::getSliceThickness(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getSliceThickness();
	}
	return 0;
}
void CData::getWLWW(int nSliceIdx, int& nWL, int& nWW) {
	CImage<short>* pTargetImage = getImagePtr(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getWLWW(nWL, nWW);
	}
	else {
		nWL = 0;
		nWW = 0;
	}
}
void CData::getWLWW(int nSeriesIdx, int nImageIdx, int& nWL, int& nWW) {
	CImage<short>* pTargetImage = getImagePtr(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getWLWW(nWL, nWW);
	}
	else {
		nWL = 0;
		nWW = 0;
	}
}
int CData::getInstanceNumber(int nSliceIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getInstanceNumber();
	}
	return -1;
}
int CData::getInstanceNumber(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getImagePtr(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getInstanceNumber();
	}
	return -1;
}

////////////////////////////////////////////////////////////////////
template <typename T>
bool CData::loadImage(std::string sImagePath, CSeries* &pCiSeries, CImage<T>* &pCiImage) {
	// exception
	if(pCiSeries != NULL || pCiImage != NULL) {
		return false;
	}

	std::string sImageName = getFileName(sImagePath);
	std::string sImageExtention = getFileExtension(sImagePath);

	// 이미지 load, 이미지 정보로 일치하는 Series를 찾고, 없으면 새로운 Series를 열어서 반환
	if(sImageExtention == "dcm") {
		const char* pcImagePath = sImagePath.c_str();
		DcmDataset* dataSet = NULL;
		short* psImage = NULL;
		
		if(loadDICOM_dcmtk(pcImagePath, dataSet, psImage)) {

			// 1. 이미지 Load //
			OFString OFStrModality, OFStrSeriesInstanceUID, OFStrFrameOfReferenceUID;
			OFString OFStrPatientID, OFStrPatientName, OFStrSeriesDate, OFStrDescription;
			OFString OFStrInstanceNumber, OFStrRows, OFStrColumns, OFStrPixelSpacing, OFStrSliceThickeness;
			OFString OFStrImagePosition, OFStrImageOrientation;
			OFString OFStrWindowLevel, OFStrWindowWidth;

			dataSet->findAndGetOFString(DcmTagKey(0x0008, 0x0060), OFStrModality);
			dataSet->findAndGetOFString(DcmTagKey(0x0020, 0x000E), OFStrSeriesInstanceUID);
			dataSet->findAndGetOFString(DcmTagKey(0x0020, 0x0052), OFStrFrameOfReferenceUID);

			dataSet->findAndGetOFString(DcmTagKey(0x0010, 0x0020), OFStrPatientID);
			dataSet->findAndGetOFString(DcmTagKey(0x0010, 0x0010), OFStrPatientName);
			dataSet->findAndGetOFString(DcmTagKey(0x0008, 0x0021), OFStrSeriesDate);
			dataSet->findAndGetOFString(DcmTagKey(0x0008, 0x103E), OFStrDescription);

			dataSet->findAndGetOFString(DcmTagKey(0x0020, 0x0013), OFStrInstanceNumber);
			dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x0010), OFStrRows);
			dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x0011), OFStrColumns);
			dataSet->findAndGetOFStringArray(DcmTagKey(0x0028, 0x0030), OFStrPixelSpacing);
			dataSet->findAndGetOFString(DcmTagKey(0x0018, 0x0050), OFStrSliceThickeness);

			dataSet->findAndGetOFStringArray(DcmTagKey(0x0020, 0x0032), OFStrImagePosition);
			dataSet->findAndGetOFStringArray(DcmTagKey(0x0020, 0x0037), OFStrImageOrientation);

			dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x1050), OFStrWindowLevel);
			dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x1051), OFStrWindowWidth);

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

			// 자료형 확인 후, setImage
			if(typeid(T) == typeid(short)) {
				pCiImage = new CImage<T>();
				pCiImage->setImagePath(sImagePath);
				pCiImage->setImageName(sImageName);
				pCiImage->setImageExtension(sImageExtention);
				pCiImage->setImage((T*)psImage, nWidth, nHeight, nChannel);
				pCiImage->setImagePosition(fImagePositionX, fImagePositionY, fImagePositionZ);
				pCiImage->setImageOrientation(fImageOrientationRowX, fImageOrientationRowY, fImageOrientationRowZ, fImageOrientationColX, fImageOrientationColY, fImageOrientationColZ);
				pCiImage->setPixelSpacing(fPixelSpacingX, fPixelSpacingY);
				pCiImage->setSliceThickness(fSliceThickness);
				pCiImage->setWLWW(nWL, nWW);
				pCiImage->setInstanceNumber(nInstanceNumber);
			}
			else {
				// loadDICOM_dcmtk는 short형만 반환하기 때문에, 다른 자료형이 들어오면 변환해야함. 자료형 차이로 인한 값 손실은 보장하지 않음!!
				int nSize = nWidth*nHeight*nChannel;
				T* temp = new T[nSize];
				for(int i=0, ni=nSize; i<ni; i++) {
					temp[i] = (T)psImage[i];
				}

				pCiImage = new CImage<T>();
				pCiImage->setImagePath(sImagePath);
				pCiImage->setImageName(sImageName);
				pCiImage->setImageExtension(sImageExtention);
				pCiImage->setImage(temp, nWidth, nHeight, nChannel);
				pCiImage->setImagePosition(fImagePositionX, fImagePositionY, fImagePositionZ);
				pCiImage->setImageOrientation(fImageOrientationRowX, fImageOrientationRowY, fImageOrientationRowZ, fImageOrientationColX, fImageOrientationColY, fImageOrientationColZ);
				pCiImage->setPixelSpacing(fPixelSpacingX, fPixelSpacingY);
				pCiImage->setSliceThickness(fSliceThickness);
				pCiImage->setWLWW(nWL, nWW);
				pCiImage->setInstanceNumber(nInstanceNumber);

				SAFE_DELETE_ARRAY(temp);
			}


			// 2. Series //
			if(pCiSeries == NULL) {
				pCiSeries = new CSeries();
				pCiSeries->m_sSeriesInstanceUID = OFStrSeriesInstanceUID.c_str();
				pCiSeries->m_sFrameOfReferenceUID = OFStrFrameOfReferenceUID.c_str();
				pCiSeries->m_sPatientID = OFStrPatientID.c_str();
				pCiSeries->m_sPatientName = OFStrPatientName.c_str();
				pCiSeries->m_sSeriesDate = OFStrSeriesDate.c_str();
				pCiSeries->m_sDescription = OFStrDescription.c_str();
				pCiSeries->m_sModality = OFStrModality.c_str();
			}
		}

		// delete memory
		SAFE_DELETE(dataSet);
		SAFE_DELETE_ARRAY(psImage);
	}
	else if(sImageExtention == "jpg" || sImageExtention == "png" || sImageExtention == "bmp") {
		const char* pcImagePath = sImagePath.c_str();
		unsigned char* pucImage = NULL;
		int nWidth, nHeight, nChannel;

		if(loadImage_opencv(pcImagePath, false, pucImage, nWidth, nHeight, nChannel)) {
			// 자료형 확인 후, setImage
			if(typeid(T) == typeid(unsigned char)) {
				pCiImage = new CImage<T>();
				pCiImage->setImagePath(sImagePath);
				pCiImage->setImageName(sImageName);
				pCiImage->setImageExtension(sImageExtention);
				pCiImage->setImage((T*)pucImage, nWidth, nHeight, nChannel);
				pCiImage->setWLWW(256, 128);
			}
			else {
				// loadImage_opencv는 unsigned char형만 반환하기 때문에, 다른 자료형이 들어오면 변환해야함. 자료형 차이로 인한 값 손실은 보장하지 않음!!
				int nSize = nWidth*nHeight*nChannel;
				T* temp = new T[nSize];
				for(int i=0, ni=nSize; i<ni; i++) {
					temp[i] = (T)pucImage[i];
				}

				pCiImage = new CImage<T>();
				pCiImage->setImagePath(sImagePath);
				pCiImage->setImageName(sImageName);
				pCiImage->setImageExtension(sImageExtention);
				pCiImage->setImage((T*)pucImage, nWidth, nHeight, nChannel);
				pCiImage->setWLWW(256, 128);
				
				SAFE_DELETE_ARRAY(temp);
			}

			// Series
			if(pCiSeries == NULL) {
				pCiSeries = new CSeries();
			}

		}

		// delete memory
		SAFE_DELETE_ARRAY(pucImage);
	}
	else {
		return false;
	}

	return true;
}
bool CData::loadDICOM_dcmtk(const char* pcImagePath, DcmDataset* &dataSet, short* &psDCMImage) {
	if(psDCMImage != NULL) {
		return false;
	}

	// const
	int USHORT_MAX_VALUE = 65535;
	int SHORT_MIN_VALUE = -32768;
	int SHORT_MAX_VALUE = 32767;
	int DATA_MIN_HU_O = -3027;
	int DATA_MIN_HU_DEFAULT = -1024;

	// load dicom file
	DcmFileFormat* fileformat = NULL;
	DicomImage* dcmImage = NULL;

	try {
		// file format check
		fileformat = new DcmFileFormat();
		OFCondition status = fileformat->loadFile(pcImagePath);
		if (!status.good()) 
		{
			throw "file format";
		}

		dcmImage = new DicomImage(pcImagePath);
		if(dcmImage == NULL) {
			throw "DCMTK-pixelData";
		}
	}
	catch(char* err) {
		std::cerr << "DCMTK File error: " << err << std::endl;

		// memory clear
		if(fileformat != NULL) {
			delete fileformat;
		}
		if(dcmImage != NULL) {
			delete dcmImage;
		}
	}
	
	// load dicom image 준비
	OFString OFStrRows, OFStrColumns;
	dataSet = fileformat->getAndRemoveDataset();
	dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x0010), OFStrRows);
	dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x0011), OFStrColumns);

	int nWidth = atoi(OFStrColumns.c_str());
	int nHeight = atoi(OFStrRows.c_str());
	int nSize = nWidth * nHeight;
	psDCMImage = new short[nSize];

	// load dicom image
	try {
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
		}
	}
	catch(char* err) {
		std::cerr << "DCMTK Image error: " << err << std::endl;

		if(dataSet != NULL) {
			delete dataSet;
		}
		SAFE_DELETE_ARRAY(psDCMImage);

		return false;
	}

	// delete memory
	fileformat->clear();
	delete fileformat;
	delete dcmImage;

	return true;
}
bool CData::loadImage_opencv(const char* pcImagePath, bool isColor, unsigned char* &pucImage, int &nWidth, int &nHeight, int &nChannel) {
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

		pucImage = new unsigned char[nSize];

		if(nChannel == 1) {
			for(int r=0; r< nHeight; r++) {
				unsigned char* pucRow = image.ptr<uchar>(r);

				for(int c=0; c< nWidth; c++) {
					int idx = r*nWidth + c;
					pucImage[idx] = pucRow[c];
				}
			}
		}
		else if(nChannel == 3) {
			for(int r=0; r< nHeight; r++) {
				unsigned char* pucRow = image.ptr<uchar>(r);

				for(int c=0; c< nWidth; c++) {
					int idx = (r*nWidth + c)*nChannel;
					pucImage[idx+0] = pucRow[c*3+0];
					pucImage[idx+1] = pucRow[c*3+1];
					pucImage[idx+2] = pucRow[c*3+2];
				}
			}
		}
	}

	return true;
}

bool CData::isMask_namingRule(std::string sPath) {
	std::string sFileName = getFileName(sPath);

	// 1. 파일명의 첫 두글자가 "m_"으로 시작하는 경우
	if(sFileName.substr(0, 2).compare("m_") == 0) {
		return true;
	}
	// 2. 파일명에 "_m_" 문자열이 포함된 경우
	if(sFileName.find("_m_") != string::npos) {
		return true;
	}
	// 3. 상위 디렉토리 명이 mask 또는 label 단어를 포함하는 경우
	std::vector<std::string> paths = splitPath(sPath);
	if(paths.size() >= 2) {
		std::string sParentFolderName = paths[paths.size()-2];
		if(sParentFolderName.find("mask") != string::npos || sParentFolderName.find("label") != string::npos) {
			return true;
		}
	}

	return false;
}
template<typename T>
bool CData::isMask_imageRule(CImage<T>* pCiImage) {
	
	// pixel 값이 binary mask거나, 1~10까지의 label을 갖는 이미지 (0~10, 255)
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

template<typename T1, typename T2>
bool CData::isSameImage(CImage<T1>* pCiImage1, CImage<T2>* pCiImage2) {
	bool isSame = false;

	// 같은 Image로 판별하는 기준을 아래에 나열
	if(pCiImage1 != NULL && pCiImage2 != NULL) {

		// 1. 이미지의 가로, 세로 크기가 같고, 이름이 동일할 때
		if(pCiImage1->m_nWidth == pCiImage2->m_nWidth && pCiImage1->m_nHeight == pCiImage2->m_nHeight) {
			if(pCiImage1->getImageName().compare(pCiImage2->getImageName()) == 0) {
				isSame = true;
			}
		}
	}

	return isSame;
}
bool CData::isSameSeries(CSeries* pCiSeries1, CSeries* pCiSeries2) {
	bool isSame = false;

	// 같은 series로 판별하는 기준을 아래에 나열
	if(pCiSeries1 != NULL && pCiSeries2 != NULL) {

		// 1. Series Instance UID가 같은 경우
		if(pCiSeries1->m_sSeriesInstanceUID.compare(pCiSeries2->m_sSeriesInstanceUID) == 0) {
			isSame = true;
		}
		else if(pCiSeries1->m_sFrameOfReferenceUID.compare(pCiSeries2->m_sFrameOfReferenceUID) == 0) {
			// FrameOfReferenceUID가 같은 경우
			isSame = true;
		}
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
		cout << "No file in directory!" << endl;
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