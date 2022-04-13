#include "data.h"

#if QT
CData::CData() {
	init();
}
CData::CData(QThread* pThread) {
	init();
	setQThread(pThread);
}
CData::~CData() {
	clear();

	m_sPaths.size();
	SAFE_DELETE(m_pThread);
}
void CData::setQThread(QThread* pThread) {
	m_pThread = pThread;
}
void CData::setQThreadParam(std::vector<std::string> sPaths, bool bReadRecursive) {
	m_sPaths = sPaths;
	m_bReadRecursive = bReadRecursive;
}
void CData::doWork() {
	if(m_sPaths.size()) {
		printf("start\n");
		readImage(m_sPaths, m_bReadRecursive, NULL);
		printf("finish\n");
	}
	m_pThread->exit();
}
#else
CData::CData() {
	init();
}
CData::~CData() {
	clear();
}
#endif

void CData::init() {
	// preference
	m_sLogPath = "";

#if QT
	m_pThread = NULL;
#endif
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
				pTargetImage->clearImage();
			}
		}
	}
	else {
		CSeries* pTargetSeries = getSeries(nSeriesIdx);

		for(int i=0, ni=pTargetSeries->getImageCount(); i<ni; i++) {
			CImage<short>* pTargetImage = pTargetSeries->getImage(i);
			pTargetImage->clearImage();
			
		}
	}
}

std::ostream& operator<<(std::ostream& stream, const CData& obj) {
	int nSeriesCount = obj.m_seriesList.size();

	stream << "\n\n" << "[Data] (" << "Series: " << nSeriesCount << ")\n";
	
	for(int i=0, ni=nSeriesCount; i<ni; i++) {
		stream << *obj.m_seriesList[i] << "\n";
	}

	return stream;
}

void CData::readImage(std::string sPath, bool bReadRecursive, std::function<void(int, int)>* pCallback) {	
	std::vector<std::string> sFilePaths;

	if(isFile(sPath)) {
		// sPath가 파일인 경우
		sFilePaths.push_back(sPath);
	}
	else {
		if(bReadRecursive) {
			// sPath가 폴더인 경우 하위 디렉토리까지 순회하면서, 읽어야 할 파일의 목록을 생성
			searchingDir(sPath, sFilePaths);
		}
	}

	readImage(sFilePaths, bReadRecursive, pCallback);
}
void CData::readImage(std::vector<std::string> sPaths, bool bReadRecursive, std::function<void(int, int)>* pCallback) {
	std::vector<std::string> sFilePaths;

	for(std::vector<std::string>::iterator it=sPaths.begin(); it!=sPaths.end(); it++) {
		if(bReadRecursive) {
			if(isFile(*it)) {
				// sPath가 파일인 경우
				sFilePaths.push_back(*it);
			}
			else {
				// sPath가 폴더인 경우 하위 디렉토리까지 순회하면서, 읽어야 할 파일의 목록을 생성
				searchingDir(*it, sFilePaths);
			}
		}
		else {
			sFilePaths.push_back(*it);
		}
	}

	int nFileCount = 0;
	int nFileTotalCount = sFilePaths.size();

	// log file
	if(m_sLogPath != "") {
		m_sLog.open(m_sLogPath.c_str());
	}

	if(nFileTotalCount > 0) {
		m_isLoading = true;
		// 2. analyzeFile
		m_preLoad.resize(sFilePaths.size());	// 파일 수 만큼 공간 확보
		for(int i=0, ni=m_preLoad.size(); i<ni; i++) {
			m_preLoad[i] = new PreLoadContainer;
			m_preLoad[i]->sFilePath = replaceAll(sFilePaths[i], "/", "\\");
			m_preLoad[i]->isLoad = isExcludeFile(m_preLoad[i]->sFilePath)? EXCEPT_FILE: false;
			m_preLoad[i]->isMask = isMask_namingRule(m_preLoad[i]->sFilePath);
			m_preLoad[i]->pCiSeries = NULL;
		}

		// thread
		#if THREAD
			std::vector<std::future<void>> v_async;
			// image
			for(int i=0, ni=m_preLoad.size(); i<ni; i++) {
				if(!m_preLoad[i]->isMask) {
					v_async.emplace_back(std::async(std::launch::async, &CData::analyzeFile, this, m_preLoad.begin()+i, m_preLoad.begin()+(i+1), std::ref(nFileCount), pCallback));
				}
			}
			for(int i=0, ni=v_async.size(); i<ni; i++) {
				v_async[i].wait();
			}
			v_async.clear();

			// mask
			for(int i=0, ni=m_preLoad.size(); i<ni; i++) {
				if(m_preLoad[i]->isMask) {
					v_async.emplace_back(std::async(std::launch::async, &CData::analyzeFile, this, m_preLoad.begin()+i, m_preLoad.begin()+(i+1), std::ref(nFileCount), pCallback));
				}
			}
			for(int i=0, ni=v_async.size(); i<ni; i++) {
				v_async[i].wait();
			}
			
		#else
			// image
			for(int i=0, ni=m_preLoad.size(); i<ni; i++) {
				if(!m_preLoad[i]->isMask) {
					analyzeFile(m_preLoad.begin()+i, m_preLoad.begin()+(i+1), nFileCount, pCallback);
				}
			}
			// mask
			for(int i=0, ni=m_preLoad.size(); i<ni; i++) {
				if(m_preLoad[i]->isMask) {
					analyzeFile(m_preLoad.begin()+i, m_preLoad.begin()+(i+1), nFileCount, pCallback);
				}
			}
		#endif

		// clear memory
		for(int i=0, ni=m_preLoad.size(); i<ni; i++) {
			SAFE_DELETE(m_preLoad[i]);
		}
		m_preLoad.clear();

		m_isLoading = false;
	}

	if(m_sLog.is_open()) {
		m_sLog.close();
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

void CData::setLogPath(std::string sPath) {
	m_sLogPath = sPath;
}
void CData::checkIsEmptyLog(std::string sFinalLogPath) {

	if (m_sLogPath != "") {
		m_sLog.open(m_sLogPath.c_str(), std::ofstream::out | std::ofstream::app);
	}

	if (m_sLog.is_open()) {
		m_sLog.seekp(0, std::ios_base::end);
		size_t size = (size_t)m_sLog.tellp();
		if (size == 0) {
			// 삭제
			std::cout << "error log file is empty" << std::endl;
			m_sLog.close();
			remove(m_sLogPath.c_str()); // 먼저 닫고 삭제
			std::cout << "deleted empty file" << std::endl;
		}
		else {
			// 파일명 변경
			rename(m_sLogPath.c_str(), sFinalLogPath.c_str());
			m_sLog.close();
		}
	}

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
	if(pTargetImage == NULL) {
		return NULL;
	}

	short* psImage = pTargetImage->getImage();
	if(LAZY_LOADING && psImage == NULL) {
		if(loadImage(pTargetImage, false)) {
			psImage = pTargetImage->getImage();
		}
	}
	return psImage;
}
short* CData::getImage(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);
	short* psImage = pTargetImage->getImage();

	if(LAZY_LOADING && psImage == NULL) {
		if(loadImage(pTargetImage, false)) {
			psImage = pTargetImage->getImage();
		}
	}
	return psImage;
}

bool CData::setImage(int nSliceIdx, short* psImage, int nWidth, int nHeight, int nChannel) {
	bool isSet = false;
	CImage<short>* pTargetImage = getCImage(nSliceIdx);

	if(pTargetImage) {
		isSet = pTargetImage->setImage(psImage, nWidth, nHeight);
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
bool CData::copyImage(int nSeriesIdx, int nImageIdx, short* &psImage, int &nWidth, int &nHeight) {
	bool isCopy = false;
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);

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
bool CData::copyImage(int nSeriesIdx, int nImageIdx, short* &psImage, int &nWidth, int &nHeight, int &nChannel) {
	bool isCopy = false;
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);

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

unsigned char* CData::getMask(int nSliceIdx) {
	CImage<short>* pTargetImage = getCImage(nSliceIdx);
	if(pTargetImage == NULL) {
		return NULL;
	}

	unsigned char* pucMask = pTargetImage->getMask();
	if(LAZY_LOADING && pucMask == NULL) {
		if(loadImage(pTargetImage, true)) {
			pucMask = pTargetImage->getMask();
		}
	}

	return pucMask;
}
unsigned char* CData::getMask(int nSeriesIdx, int nMaskIdx) {
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nMaskIdx);
	unsigned char* pucMask = pTargetImage->getMask();

	if(LAZY_LOADING && pucMask == NULL) {
		if(loadImage(pTargetImage, true)) {
			pucMask = pTargetImage->getMask();
		}
	}

	return pucMask;
}

bool CData::setMask(int nSliceIdx, unsigned char* pucMask, int nWidth, int nHeight, int nChannel) {
	bool isSet = false;
	CImage<short>* pTargetImage = getCImage(nSliceIdx);

	if(pTargetImage) {
		isSet = pTargetImage->setMask(pucMask, nWidth, nHeight);
	}

	return isSet;
}
bool CData::setMask(int nSeriesIdx, int nImageIdx, unsigned char* pucMask, int nWidth, int nHeight, int nChannel) {
	bool isSet = false;
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);

	if(pTargetImage) {
		isSet = pTargetImage->setMask(pucMask, nWidth, nHeight);
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

bool CData::copyMask(int nSliceIdx, unsigned char* &pucMask, int &nWidth, int &nHeight) {
	bool isCopy = false;
	CImage<short>* pTargetImage = getCImage(nSliceIdx);

	if(pTargetImage) {
		isCopy = pTargetImage->copyMask(pucMask, nWidth, nHeight);
		if(LAZY_LOADING && !isCopy) {
			if(loadImage(pTargetImage, true)) {
				isCopy = pTargetImage->copyMask(pucMask, nWidth, nHeight);
			}
		}
	}

	return isCopy;
}
bool CData::copyMask(int nSliceIdx, unsigned char* &pucMask, int &nWidth, int &nHeight, int &nChannel) {
	bool isCopy = false;
	CImage<short>* pTargetImage = getCImage(nSliceIdx);

	if(pTargetImage) {
		isCopy = pTargetImage->copyMask(pucMask, nWidth, nHeight, nChannel);
		if(LAZY_LOADING && !isCopy) {
			if(loadImage(pTargetImage, true)) {
				isCopy = pTargetImage->copyMask(pucMask, nWidth, nChannel);
			}
		}
	}

	return isCopy;
}
bool CData::copyMask(int nSeriesIdx, int nImageIdx, unsigned char* &pucMask, int &nWidth, int &nHeight) {
	bool isCopy = false;
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);

	if(pTargetImage) {
		isCopy = pTargetImage->copyMask(pucMask, nWidth, nHeight);
		if(LAZY_LOADING && !isCopy) {
			if(loadImage(pTargetImage, true)) {
				isCopy = pTargetImage->copyMask(pucMask, nWidth, nHeight);
			}
		}
	}

	return isCopy;
}
bool CData::copyMask(int nSeriesIdx, int nImageIdx, unsigned char* &pucMask, int &nWidth, int &nHeight, int &nChannel) {
	bool isCopy = false;
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);

	if(pTargetImage) {
		isCopy = pTargetImage->copyMask(pucMask, nWidth, nHeight, nChannel);
		if(LAZY_LOADING && !isCopy) {
			if(loadImage(pTargetImage, true)) {
				isCopy = pTargetImage->copyMask(pucMask, nWidth, nChannel);
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
			isCopy = copyMask(nSeriesIdx, i, ppucMasks[i], nWidth, nHeight);
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
			isCopy = copyMask(nSeriesIdx, i, ppucMasks[i], nWidth, nHeight, nChannel);
		}
	}

	return isCopy;
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

std::string CData::getMaskPath(int nSliceIdx) {
	CImage<short>* pTargetImage = getCImage(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getMaskPath();
	}
	return "";
}
std::string CData::getMaskPath(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getMaskPath();
	}
	return "";
}
std::string CData::getMaskImage(int nSliceIdx) {
	CImage<short>* pTargetImage = getCImage(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getMaskName();
	}
	return "";
}
std::string CData::getMaskImage(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getMaskName();
	}
	return "";
}
std::string CData::getMaskExtension(int nSliceIdx) {
	CImage<short>* pTargetImage = getCImage(nSliceIdx);
	if(pTargetImage) {
		return pTargetImage->getMaskExtension();
	}
	return "";
}
std::string CData::getMaskExtension(int nSeriesIdx, int nImageIdx) {
	CImage<short>* pTargetImage = getCImage(nSeriesIdx, nImageIdx);
	if(pTargetImage) {
		return pTargetImage->getMaskExtension();
	}
	return "";
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
void CData::analyzeFile(std::vector<PreLoadContainer*>::iterator start, std::vector<PreLoadContainer*>::iterator end, 
						int &nFileCount, std::function<void(int, int)>* pCallback) 
{
#if THREAD
	static std::mutex mutex_fileCount;
	static std::mutex mutex_addImage;
#endif

	for(auto iter=start; iter< end; iter++) {
		PreLoadContainer* plc = *iter;

		if(plc->isLoad == EXCEPT_FILE) {
			// do nothing
		}
		else {
			// 1-1. 이미지를 Load 및 mask여부 판별, 파일명 혹은 폴더명으로 이미 구분된 경우, 별도 검사하지 않음
			if(plc->isMask) {
				// 파일명, 확장자 등으로 Mask로 구분이 이미 된 경우
				plc->isLoad = loadImage(plc->sFilePath, plc->pCiSeries, plc->pCiMasks);
			}
			else {
				plc->isLoad = loadImage(plc->sFilePath, plc->pCiSeries, plc->pCiImages);
			}

			// 1-2. 파일에서 읽은 Series 정보(이미지 정보)를 폴더명, 이미지명을 이용하여 임의로 수정
			if(plc->isLoad && plc->pCiSeries) {
				if(!plc->isMask) {
					// 1-2-1. 이미지명에 있는 이미지 태그 제거
					for(int i=0, ni=plc->pCiImages.size(); i< ni; i++) {
						std::string sRemovedTag = removeNameTag(plc->pCiImages[i]->getImageName());
						plc->pCiImages[i]->setImageNameRemoveTag(sRemovedTag);
					}

					// 1-2-2. series 수정
					modifySeries(plc->pCiSeries, plc->pCiImages);
				}
				else {
					// 1-2-1. 이미지명에 있는 mask 태그 제거
					for(int i=0, ni=plc->pCiMasks.size(); i< ni; i++) {
						std::string sRemovedTag = removeNameTag(plc->pCiMasks[i]->getImageName());
						plc->pCiMasks[i]->setImageNameRemoveTag(sRemovedTag);
					}

					// 1-2-2. series 수정
					modifySeries(plc->pCiSeries, plc->pCiMasks);
				}
			}
			else {
				if(m_sLog.is_open()) {
					switch(plc->isLoad) {
						case UNREADBLE_FILE: 
						{
							m_sLog << "읽을 수 없는 파일 => " << plc->sFilePath << "\n";
							break;
						}
						case DCM_FILE_FORMAT_ERR: 
						{
							m_sLog << "DCM Header에 문제가 있음 => " << plc->sFilePath << "\n";
							break;
						}
						case DCM_IMG_ERR: 
						{
							m_sLog << "DCM img에 문제가 있음 => " << plc->sFilePath << "\n";
							break;
						}
					}
				}
			}

			// 1-4. Series 중복 검사, Image 매칭
			int nSeriesIdx = -1, nImageIdx = -1;
			if(!plc->isMask) {
				CSeries* pCiSeries = plc->pCiSeries;
				CSeries* pCiMatchedSeries = NULL;

				for(int s=0, ns=getSeriesCount(); s<ns; s++) {
					if(isSameSeries(pCiSeries, getSeries(s))) {
						pCiMatchedSeries = m_seriesList[s];
						nSeriesIdx = s;
						break;
					}
				}
				if(pCiMatchedSeries != NULL) {
					// Series가 중복되는 경우
					plc->pCiSeries->clear();
					SAFE_DELETE(plc->pCiSeries);
					plc->pCiSeries = NULL;
				}
				else {
					// Series가 중복되지 않는 경우 (= 새로운 Series인 경우 Series를 추가)
					addSeries(pCiSeries);

					nSeriesIdx = getSeriesCount()-1;
					pCiMatchedSeries = pCiSeries;
				}

				#if THREAD
					mutex_addImage.lock();
				#endif
					// Series와 Image 매칭
					for(int idx=0, nIdx=plc->pCiImages.size(); idx<nIdx; idx++) {
						// Image
						CImage<short>* pCiImage = plc->pCiImages[idx];

						// 중복 Image 검사
						bool isDup = false;
						for(int j=0, nj=pCiMatchedSeries->getImageCount(); j<nj; j++) {
							CImage<short>* pCiTargetImage = pCiMatchedSeries->getImage(j);

							if(isSameImage(pCiImage, pCiTargetImage)) {
								isDup = true;
								nImageIdx = -1;
								break;
							}
						}

						if(isDup) {
							// 중복되면 버림
							if(m_sLog.is_open()) {
								m_sLog << "파일 중복 ==> " << pCiImage->getImagePath() << "\n";
							}
							pCiImage->clear();
							SAFE_DELETE(pCiImage);
							plc->pCiImages[idx] = NULL;
							plc->isLoad = DUPLICATE_FILE;
						}
						else {
							pCiMatchedSeries->addImage(pCiImage);
							std::sort(pCiMatchedSeries->m_images.begin(), pCiMatchedSeries->m_images.end(), sorting<short>);
							for(int i=0, ni=pCiMatchedSeries->m_images.size(); i< ni; i++) {
								if(pCiImage == pCiMatchedSeries->m_images[i]) {
									nImageIdx = i;
									break;
								}
							}
						}
					}

					plc->pCiImages.clear();
				#if THREAD
					mutex_addImage.unlock();
				#endif
			}
			else {
				// 3-1. Series 중복 검사
				CSeries* pCiSeries = plc->pCiSeries;
				CSeries* pCiMatchedSeries = NULL;

				if(pCiSeries == NULL) {continue;}
				for(int s=0, ns=getSeriesCount(); s<ns; s++) {
					if(isSameSeries(pCiSeries, getSeries(s))) {
						pCiMatchedSeries = m_seriesList[s];
						break;
					}
				}

				if(pCiMatchedSeries) {
					// Matching 되는 Series가 있는 경우
				
					if(plc->pCiMasks.size() == 1) {
						// (1) 1개 파일에 mask가 1개만 들어있는 경우
						CImage<unsigned char>* pCiMask = plc->pCiMasks[0];
						int nMatchedImageIdx = -1;

						// 매칭되는 Image 검사
						for(int i=0, ni=pCiMatchedSeries->getImageCount(); i<ni; i++) {
							if(isSameImage(pCiMask, pCiMatchedSeries->getImage(i))) {
								nMatchedImageIdx = i;
								break;
							}
						}

						if(nMatchedImageIdx != -1) {
							// 매칭되는 Image가 있는 경우
							pCiMatchedSeries->getImage(nMatchedImageIdx)->setMaskName(pCiMask->getImageName());
							pCiMatchedSeries->getImage(nMatchedImageIdx)->setMaskPath(pCiMask->getImagePath());
							pCiMatchedSeries->getImage(nMatchedImageIdx)->setMaskExtension(pCiMask->getImageExtension());
							pCiMatchedSeries->getImage(nMatchedImageIdx)->setMaskNameRemoveTag(pCiMask->getImageNameRemoveTag());

							if(!LAZY_LOADING) {
								pCiMatchedSeries->getImage(nMatchedImageIdx)->setMask(pCiMask->getImage(), pCiMask->getWidth(), pCiMask->getHeight());
							}
						
						}
						else {
							// 매칭되는 Image가 없는 경우, 버림
							if(m_sLog.is_open()) {
								m_sLog << "mask 파일만 존재 ==> " << pCiMask->getImagePath() << "\n";
							}
						}

						pCiMask->clear();
						SAFE_DELETE(pCiMask);
					}
					else {
						// (2) 1개 파일에 mask가 여러개 들어있는 경우
						if(plc->pCiMasks.size() == pCiMatchedSeries->getImageCount()) {
							// (2-1) image수와 mask 수가 일치하면, 비교하지 않고 순서대로 매칭
							for(int idx=0, nIdx=plc->pCiMasks.size(); idx<nIdx; idx++) {
								// Image
								CImage<unsigned char>* pCiMask = plc->pCiMasks[idx];
								int nMatchedImageIdx = -1;

								// 매칭되는 Image 검사
								for(int i=0, ni=pCiMatchedSeries->getImageCount(); i<ni; i++) {
									pCiMatchedSeries->getImage(i)->setMaskName(pCiMask->getImageName());
									pCiMatchedSeries->getImage(i)->setMaskPath(pCiMask->getImagePath());
									pCiMatchedSeries->getImage(i)->setMaskExtension(pCiMask->getImageExtension());
									pCiMatchedSeries->getImage(i)->setMaskNameRemoveTag(pCiMask->getImageNameRemoveTag());

									if(!LAZY_LOADING) {
										pCiMatchedSeries->getImage(nMatchedImageIdx)->setMask(pCiMask->getImage(), pCiMask->getWidth(), pCiMask->getHeight());
									}
								}

								pCiMask->clear();
								SAFE_DELETE(pCiMask);
							}
						}
						else {
							// (2-2) image수와 mask 수가 일치하지 않으면, 비교
							for(int idx=0, nIdx=plc->pCiMasks.size(); idx<nIdx; idx++) {
								// Image
								CImage<unsigned char>* pCiMask = plc->pCiMasks[idx];
								int nMatchedImageIdx = -1;

								// 매칭되는 Image 검사
								for(int i=0, ni=pCiMatchedSeries->getImageCount(); i<ni; i++) {
									if(isSameImage(pCiMask, pCiMatchedSeries->getImage(i))) {
										nMatchedImageIdx = i;
										break;
									}
								}

								if(nMatchedImageIdx != -1) {
									// 매칭되는 Image가 있는 경우
									pCiMatchedSeries->getImage(nMatchedImageIdx)->setMask(pCiMask->getMask(), pCiMask->getWidth(), pCiMask->getHeight());
									pCiMatchedSeries->getImage(nMatchedImageIdx)->setMaskName(pCiMask->getImageName());
									pCiMatchedSeries->getImage(nMatchedImageIdx)->setMaskPath(pCiMask->getImagePath());
									pCiMatchedSeries->getImage(nMatchedImageIdx)->setMaskExtension(pCiMask->getImageExtension());
								}
								else {
									// 매칭되는 Image가 없는 경우, 버림
									if(m_sLog.is_open()) {
										m_sLog << "mask 파일만 존재 ==> " << pCiMask->getImagePath() << "\n";
									}
								}

								pCiMask->clear();
								SAFE_DELETE(pCiMask);
							}
						}
					}
				
					plc->pCiMasks.clear();
				}
				else {
					// Matching 되는 Series가 없는 경우
					for(int idx=0, nIdx=plc->pCiMasks.size(); idx<nIdx; idx++) {
						SAFE_DELETE(plc->pCiMasks[idx]);
					}
				}

				plc->pCiSeries->clear();
				SAFE_DELETE(plc->pCiSeries);
				plc->pCiSeries = NULL;
			}
		}
		
		// 1-5. callback
		#if THREAD
			mutex_fileCount.lock();
			nFileCount++;
			if(pCallback != NULL) {
				(*pCallback)(nFileCount, m_preLoad.size());
			}

			#if QT
				emit sendDataProgress(nSeriesIdx, nImageIdx, nFileCount, m_preLoad.size());
			#endif
			mutex_fileCount.unlock();
		#else
			nFileCount++;
			if(pCallback != NULL) {
				(*pCallback)(nFileCount, m_preLoad.size());
			}
			#if QT
				emit sendDataProgress(nSeriesIdx, nImageIdx, nFileCount, m_preLoad.size());
			#endif
		#endif
	}
}

////////////////////////////////////////////////////////////////////
template <typename T>
int CData::loadImage(std::string sImagePath, CSeries* &pCiSeries, std::vector<CImage<T>*> &pCiImages) {
	// series header 정보, image header 정보까지 모두 읽어야 할 때 사용

	// exception
	if(pCiSeries != NULL) {
		return INPUT_PARAM_ERR;
	}

	int nReturn = -1;
	const char* pcImagePath = sImagePath.c_str();
	std::string sImageName = getFileName(sImagePath);
	std::string sImageExtension = getFileExtension(sImagePath);

	// 이미지 load, 이미지 정보로 일치하는 Series를 찾고, 없으면 새로운 Series를 열어서 반환
	if(sImageExtension == "dcm") {
		DcmDataset* dataSet = NULL;
		std::vector<short*> images;
		
		if(nReturn = loadDICOM_dcmtk(pcImagePath, dataSet, images, LAZY_LOADING)) {
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
				fImagePositionX = (float)atof(sImagePosition[0].c_str());
				fImagePositionY = (float)atof(sImagePosition[1].c_str());
				fImagePositionZ = (float)atof(sImagePosition[2].c_str());
			}
			std::vector<std::string> sImageOrientation = splitPath(OFStrImageOrientation.c_str());
			float fImageOrientationRowX=0, fImageOrientationRowY=0, fImageOrientationRowZ=0;
			float fImageOrientationColX=0, fImageOrientationColY=0, fImageOrientationColZ=0;
			if(sImageOrientation.size() == 6) {
				fImageOrientationRowX = (float)atof(sImageOrientation[0].c_str());
				fImageOrientationRowY = (float)atof(sImageOrientation[1].c_str());
				fImageOrientationRowZ = (float)atof(sImageOrientation[2].c_str());
				fImageOrientationColX = (float)atof(sImageOrientation[3].c_str());
				fImageOrientationColY = (float)atof(sImageOrientation[4].c_str());
				fImageOrientationColZ = (float)atof(sImageOrientation[5].c_str());
			}
			std::vector<std::string> sPixelSpacing = splitPath(OFStrPixelSpacing.c_str());
			float fPixelSpacingX=0, fPixelSpacingY=0;
			if(sPixelSpacing.size() == 2) {
				fPixelSpacingX = (float)atof(sPixelSpacing[0].c_str());
				fPixelSpacingY = (float)atof(sPixelSpacing[1].c_str());
			}
			float fSliceThickness = (float)atof(OFStrSliceThickeness.c_str());
			int nWL = atoi(OFStrWindowLevel.c_str());
			int nWW = atoi(OFStrWindowWidth.c_str());
			int nInstanceNumber = atoi(OFStrInstanceNumber.c_str());

			// 1. setImage //
			pCiImages.resize(images.size());
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

				pCiImages[idx] = pCiImage;
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

		if(nReturn = loadImage_opencv(pcImagePath, false, pucImage, nWidth, nHeight, nChannel, LAZY_LOADING)) {
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

			pCiImages.push_back(pCiImage);
		}

		// delete memory
		SAFE_DELETE_ARRAY(pucImage);
	}
	else if(sImageExtension == "nii") {
		nifti_1_header nhdr;
		std::vector<short*> images;

		if(nReturn = loadNII_libnii(pcImagePath, nhdr, images, LAZY_LOADING)) {
			
			// 1. setImage //
			int nWidth = nhdr.dim[1];
			int nHeight = nhdr.dim[2];
			int nChannel = 1;

			pCiImages.resize(images.size());
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

				int nMaxIntensity = (int)nhdr.cal_max;
				int nMinIntensity = (int)nhdr.cal_min;
				int nWL = (int)((nMaxIntensity + nMinIntensity) / 2.0);
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

				pCiImages[idx] = pCiImage;
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
		TIFF* pTiff = NULL;
		std::vector<unsigned char*> images;

		if(nReturn = loadTIFF_libtiff(pcImagePath, pTiff, images, LAZY_LOADING)) {
			int nWidth = 0, nHeight = 0, nChannel = 1;
			TIFFGetField(pTiff, TIFFTAG_IMAGEWIDTH, &nWidth);
			TIFFGetField(pTiff, TIFFTAG_IMAGELENGTH, &nHeight);

			pCiImages.resize(images.size());
			for(int idx=0, nIdx=images.size(); idx< nIdx; idx++) {
				unsigned char* pucImage = images[idx];
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
						// loadImage_libtiff는 unsigned char형만 반환하기 때문에, 다른 자료형이 들어오면 변환해야함. 자료형 차이로 인한 값 손실은 보장하지 않음!!
						int nSize = nWidth*nHeight*nChannel;
						T* temp = new T[nSize];
						for(int i=0, ni=nSize; i<ni; i++) {
							temp[i] = (T)pucImage[i];
						}
						pCiImage->setImage((T*)pucImage, nWidth, nHeight, nChannel);
						SAFE_DELETE_ARRAY(temp);
					}
				}

				pCiImages[idx] = pCiImage;
				SAFE_DELETE_ARRAY(pucImage);
			}

			// 2. Series //
			if(pCiSeries == NULL) {
				pCiSeries = new CSeries();
			}
		}
		
		// delete memory //
		images.clear();
		if(pTiff) {
			TIFFClose(pTiff);
		}
	}
	else {
		nReturn = UNREADBLE_FILE_FORMAT;
	}

	return nReturn;
}
template <typename T>
int CData::loadImage(CImage<T>* pCiImage, bool isMask) {
	// image 정보를 이미 알고 있을 때 사용 (Lazy loading용)

	// exception
	if(pCiImage == NULL) {
		return false;
	}

	int nReturn = -1;
	if(!isMask) {
		std::string sImagePath = pCiImage->getImagePath();
		const char* pcImagePath = sImagePath.c_str();
		std::string sImageExtension = pCiImage->getImageExtension();

		if(sImageExtension == "dcm") {
			std::vector<short*> images;
			DcmDataset* dataSet = NULL;

			if(nReturn = loadDICOM_dcmtk(pcImagePath, dataSet, images)) {
				std::vector<CImage<T>*> pCiTargetImages;
			
				// 1. 한 파일에 여러 image가 들어있는 경우, 매칭되는 image를 찾아줘야 함
				if(images.size() > 1) {
					CSeries* pCiTargetSeries = pCiImage->getSeries();
					int nImageCount = pCiTargetSeries->getImageCount();

					pCiTargetImages.resize(nImageCount);
					for(int i=0, ni=nImageCount; i<ni; i++) {
						pCiTargetImages[i] = (CImage<T>*)pCiTargetSeries->getImage(i);
					}
				}
				else {
					pCiTargetImages.push_back(pCiImage);
				}

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
			}
		}
		else if(sImageExtension == "jpg" || sImageExtension == "png" || sImageExtension == "bmp") {
			unsigned char* pucImage = NULL;
			int nWidth, nHeight, nChannel;

			if(nReturn = loadImage_opencv(pcImagePath, false, pucImage, nWidth, nHeight, nChannel)) {
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

			if(nReturn = loadNII_libnii(pcImagePath, nhdr, images)) {
				std::vector<CImage<T>*> pCiTargetImages;
			
				// 1. 한 파일에 여러 image가 들어있는 경우, 매칭되는 image를 찾아줘야 함
				if(images.size() > 1) {
					CSeries* pCiTargetSeries = pCiImage->getSeries();
					int nImageCount = pCiTargetSeries->getImageCount();

					pCiTargetImages.resize(nImageCount);
					for(int i=0, ni=nImageCount; i<ni; i++) {
						pCiTargetImages[i] = (CImage<T>*)pCiTargetSeries->getImage(i);
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
			TIFF* pTiff = NULL;
			std::vector<unsigned char*> images;

			if(nReturn = loadTIFF_libtiff(pcImagePath, pTiff, images, LAZY_LOADING)) {
				std::vector<CImage<T>*> pCiTargetImages;
				int nWidth = 0, nHeight = 0, nChannel = 1;
				TIFFGetField(pTiff, TIFFTAG_IMAGEWIDTH, &nWidth);
				TIFFGetField(pTiff, TIFFTAG_IMAGELENGTH, &nHeight);

				pCiTargetImages.resize(images.size());
				for(int idx=0, nIdx=images.size(); idx< nIdx; idx++) {
					unsigned char* pucImage = images[idx];
					CImage<T>* pCiImage = new CImage<T>();
			
					// 자료형 확인 후, setImage
					if(typeid(T) == typeid(unsigned char)) {
						if(!LAZY_LOADING) {
							pCiImage->setImage((T*)pucImage, nWidth, nHeight, nChannel);
						}
					}
					else {
						if(!LAZY_LOADING) {
							// loadImage_libtiff는 unsigned char형만 반환하기 때문에, 다른 자료형이 들어오면 변환해야함. 자료형 차이로 인한 값 손실은 보장하지 않음!!
							int nSize = nWidth*nHeight*nChannel;
							T* temp = new T[nSize];
							for(int i=0, ni=nSize; i<ni; i++) {
								temp[i] = (T)pucImage[i];
							}
							pCiImage->setImage((T*)pucImage, nWidth, nHeight, nChannel);
							SAFE_DELETE_ARRAY(temp);
						}
					}

					pCiTargetImages[idx] = pCiImage;
					SAFE_DELETE_ARRAY(pucImage);
				}
			}

			// delete memory //
			images.clear();
			if(pTiff) {
				TIFFClose(pTiff);
			}
		}
		else {
			return false;
		}
	}
	else {
		std::string sMaskPath = pCiImage->getMaskPath();
		const char* pcMaskPath = sMaskPath.c_str();
		std::string sMaskExtension = pCiImage->getMaskExtension();

		if(sMaskExtension == "dcm") {
			std::vector<short*> masks;
			DcmDataset* dataSet = NULL;

			if(nReturn = loadDICOM_dcmtk(pcMaskPath, dataSet, masks)) {
				std::vector<CImage<T>*> pCiTargetMasks;

				// 1. 한 파일에 여러 mask가 들어있는 경우, 매칭되는 mask를 찾아줘야 함
				if(masks.size() > 1) {
					CSeries* pCiTargetSeries = pCiImage->getSeries();
					int nImageCount = pCiTargetSeries->getImageCount();

					pCiTargetMasks.resize(nImageCount);
					for(int i=0, ni=nImageCount; i<ni; i++) {
						pCiTargetMasks[i] = (CImage<T>*)pCiTargetSeries->getImage(i);
					}
				}
				else {
					pCiTargetMasks.push_back(pCiImage);
				}

				for(int s=0, ns=masks.size(); s<ns; s++) {
					OFString OFStrRows, OFStrColumns, OFStrInstanceNumber;
					dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x0010), OFStrRows);
					dataSet->findAndGetOFString(DcmTagKey(0x0028, 0x0011), OFStrColumns);
					dataSet->findAndGetOFString(DcmTagKey(0x0020, 0x0013), OFStrInstanceNumber);

					short* psMask = masks[s];
					int nWidth = atoi(OFStrColumns.c_str());
					int nHeight = atoi(OFStrRows.c_str());
					int nChannel = 1;

					// loadDICOM_dcmtk는 short형만 반환하기 때문에, 다른 자료형이 들어오면 변환해야함. 자료형 차이로 인한 값 손실은 보장하지 않음!!
					int nSize = nWidth*nHeight*nChannel;
					unsigned char* temp = new unsigned char[nSize];
					for(int i=0, ni=nSize; i<ni; i++) {
						temp[i] = (unsigned char)psMask[i];
					}
					pCiTargetMasks[s]->setMask(temp, nWidth, nHeight, nChannel);

					SAFE_DELETE_ARRAY(temp);
					SAFE_DELETE_ARRAY(psMask);
				}
			}
		}
		else if(sMaskExtension == "jpg" || sMaskExtension == "png" || sMaskExtension == "bmp") {
			unsigned char* pucImage = NULL;
			int nWidth, nHeight, nChannel;

			if(nReturn = loadImage_opencv(pcMaskPath, false, pucImage, nWidth, nHeight, nChannel)) {
				pCiImage->setMask(pucImage, nWidth, nHeight, nChannel);
				SAFE_DELETE_ARRAY(pucImage);
			}
		}
		else if(sMaskExtension == "nii") {
			std::vector<short*> masks;
			nifti_1_header nhdr;

			if(nReturn = loadNII_libnii(pcMaskPath, nhdr, masks)) {
				std::vector<CImage<T>*> pCiTargetMasks;
			
				// 1. 한 파일에 여러 mask가 들어있는 경우, 매칭되는 mask를 찾아줘야 함
				if(masks.size() > 1) {
					CSeries* pCiTargetSeries = pCiImage->getSeries();
					int nImageCount = pCiTargetSeries->getImageCount();

					pCiTargetMasks.resize(nImageCount);
					for(int i=0, ni=nImageCount; i<ni; i++) {
						pCiTargetMasks[i] = (CImage<T>*)pCiTargetSeries->getImage(i);
					}
				}
				else {
					pCiTargetMasks.push_back(pCiImage);
				}


				// 2. setMask
				int nWidth = nhdr.dim[1];
				int nHeight = nhdr.dim[2];
				int nChannel = 1;

				for(int s=0, ns=masks.size(); s< ns; s++) {
					short* psMask = masks[s];

					// loadNII_libnii는 short형만 반환하기 때문에, 다른 자료형이 들어오면 변환해야함. 자료형 차이로 인한 값 손실은 보장하지 않음!!
					int nSize = nWidth*nHeight*nChannel;
					unsigned char* temp = new unsigned char[nSize];
					for(int i=0, ni=nSize; i<ni; i++) {
						temp[i] = (unsigned char)psMask[i];
					}

					pCiTargetMasks[s]->setMask(temp, nWidth, nHeight, nChannel);

					SAFE_DELETE_ARRAY(temp);
					SAFE_DELETE_ARRAY(psMask);
				}

				pCiTargetMasks.clear();
			}

			masks.clear();
		}
		else if(sMaskExtension == "gz") {

		}
		else if(sMaskExtension == "tiff") {
			TIFF* pTiff = NULL;
			std::vector<unsigned char*> masks;

			if(nReturn = loadTIFF_libtiff(pcMaskPath, pTiff, masks, LAZY_LOADING)) {
				int nWidth = 0, nHeight = 0, nChannel = 1;
				TIFFGetField(pTiff, TIFFTAG_IMAGEWIDTH, &nWidth);
				TIFFGetField(pTiff, TIFFTAG_IMAGELENGTH, &nHeight);

				for(int idx=0, nIdx=masks.size(); idx< nIdx; idx++) {
					unsigned char* pucMask = masks[idx];

					// loadImage_libtiff는 unsigned char형만 반환하기 때문에, 다른 자료형이 들어오면 변환해야함. 자료형 차이로 인한 값 손실은 보장하지 않음!!
					int nSize = nWidth*nHeight*nChannel;
					pCiImage->setMask(pucMask, nWidth, nHeight, nChannel);
					SAFE_DELETE_ARRAY(pucMask);
				}
			}

			// delete memory //
			masks.clear();
			if(pTiff) {
				TIFFClose(pTiff);
			}
		}
		else {
			return false;
		}
	}

	return true;
}
int CData::loadDICOM_dcmtk(const char* pcImagePath, DcmDataset* &dataSet, std::vector<short*> &images, bool bReadHeaderOnly) {
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
			throw DCM_FILE_FORMAT_ERR;
		}
	}
	catch(int nErrCode) {
		SAFE_DELETE(fileformat);
		return nErrCode;
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
		try {
			images.resize(nImageCnt);
			for(int idx=0, nIdx=nImageCnt; idx<nIdx; idx++) {
				short* psDCMImage = NULL;
				images[idx] = psDCMImage;
			}
		}
		catch(std::bad_alloc& ba) {
			SAFE_DELETE(fileformat);
			SAFE_DELETE(dataSet);
			return BAD_ALLOC;
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
								throw DCM_IMG_ERR;
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
				else {
					throw DCM_IMG_ERR;
				}
			}
			catch(int nErrCode) {
				SAFE_DELETE(fileformat);
				SAFE_DELETE(dataSet);
				SAFE_DELETE(dcmImage);
				SAFE_DELETE_ARRAY(psDCMImage);

				return nErrCode;
			}
			catch(std::bad_alloc& ba) {
				SAFE_DELETE(fileformat);
				SAFE_DELETE(dataSet);
				SAFE_DELETE(dcmImage);
				SAFE_DELETE_ARRAY(psDCMImage);

				return BAD_ALLOC;
			}
		}
	}
	
	// delete memory
	SAFE_DELETE(fileformat);

	return true;
}
int CData::loadImage_opencv(const char* pcImagePath, bool isColor, unsigned char* &pucImage, int &nWidth, int &nHeight, int &nChannel, bool bReadHeaderOnly) {
	if(pucImage != NULL) {
		return INPUT_PARAM_ERR;
	}

	IplImage* image = cvLoadImage(pcImagePath, isColor);
	if(image) {
		nWidth = image->width;
		nHeight = image->height;
		nChannel = image->nChannels;
		int nSize = nWidth * nHeight * nChannel;

		if(bReadHeaderOnly) {
			return SUCCESS;
		}
		else {
			try {
				pucImage = new unsigned char[nSize];

				switch(nChannel) {
					case 1: 
					{
						for(int r=0; r< nHeight; r++) {
							for(int c=0; c< nWidth; c++) {
								int idx = r*nWidth + c;
								pucImage[idx] = (unsigned char)image->imageData[image->widthStep*r + c];
							}
						}
						return SUCCESS;
					}
				}
			}
			catch(std::bad_alloc& ba) {
				SAFE_DELETE_ARRAY(pucImage);
				return BAD_ALLOC;
			}
		}
	}

	return UNREADBLE_FILE;
}
int CData::loadNII_libnii(const char* pcImagePath, nifti_1_header &nhdr, std::vector<short*> &images, bool bReadHeaderOnly) {
	//https://github.com/NIFTI-Imaging/nifti_clib

	nifti_image* nim = nifti_image_read(pcImagePath, 1);

	if(nim) {
		// copy header
		nhdr = nifti_convert_nim2nhdr(nim);	

		// 
		int nWidth, nHeight, nSliceCnt, nBitPix, nDataType;
		float nVoxOffset;

		nVoxOffset = nhdr.vox_offset;
		nWidth = nhdr.dim[1];
		nHeight = nhdr.dim[2];
		nSliceCnt = nhdr.dim[0]>=3? nhdr.dim[3]: 1;
		nBitPix = nhdr.bitpix;
		nDataType = nhdr.datatype;

		// read image
		images.resize(nSliceCnt);

		if(bReadHeaderOnly) {
			for(int s=0; s< nSliceCnt; s++) {
				short* psImage = NULL;
				images[s] = psImage;
			}
		}
		else {
			for(int s=0; s< nSliceCnt; s++) {
				try {
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

					images[s] = psImage;
				}
				catch(std::bad_alloc& ba) {
					for(int i=0; i< nSliceCnt; i++) {
						SAFE_DELETE_ARRAY(images[i]);
					}
					images.clear();
					nifti_image_free(nim);
					return BAD_ALLOC;
				}
			}
		}
		
		nifti_image_free(nim);
		return SUCCESS;
	}
	
	return UNREADBLE_FILE;
}
int CData::loadTIFF_libtiff(const char* pcImagePath, TIFF* &pTiff, std::vector<unsigned char*> &images, bool bReadHeaderOnly) {
	TIFF *pMultiPageTiff = TIFFOpen(pcImagePath, "r");

	if(pMultiPageTiff) {
		int nWidth=0, nHeight=0, nCurrentSliceIdx = 0, nSliceCnt=0;

		TIFFGetField(pMultiPageTiff, TIFFTAG_IMAGEWIDTH, &nWidth);
		TIFFGetField(pMultiPageTiff, TIFFTAG_IMAGELENGTH, &nHeight);
		TIFFGetField(pMultiPageTiff, TIFFTAG_PAGENUMBER, &nCurrentSliceIdx, &nSliceCnt);

		pTiff = pMultiPageTiff;
		images.resize(nSliceCnt);
		if(bReadHeaderOnly) {
			for(int s=0; s< nSliceCnt; s++) {
				unsigned char* pucImage = NULL;
				images[s] = pucImage;
			}
		}
		else {
			//This should be Width*(depth / sizeof(char))
			tsize_t ScanlineSizeBytes = TIFFScanlineSize(pMultiPageTiff);
	
			if(pMultiPageTiff) {
				int s=0;
				do {
					// copy
					unsigned char *pucBuffer = new unsigned char[nWidth*nHeight];
					for(int row = 0; row < nHeight; row++){
						TIFFReadScanline(pMultiPageTiff, &pucBuffer[row*nWidth], row, 0);
					}

					images[s] = pucBuffer;
					s++;

				} while(TIFFReadDirectory(pMultiPageTiff));
			}
		}

		return SUCCESS;
	}

	return UNREADBLE_FILE;
}

////////////////////////////////////////////////////////////////////
bool CData::saveImage(int nSliceIdx, const char* pcImagePath, int nSaveType, int nSaveOption) {
	int nTargetSeriesIdx = -1;
	int nTargetImageIdx = -1;
	
	int nIdx = 0;
	for(int i=0, ni=getSeriesCount(); i<ni; i++) {
		int nImageCnt = getSeries(i)->getImageCount();

		if(nSliceIdx >= nIdx) {
			nTargetSeriesIdx = i;
			nTargetImageIdx = nSliceIdx-nIdx;
			break;
		}

		nIdx = nIdx + nImageCnt-1;
	}

	return saveImage(nTargetSeriesIdx, nTargetImageIdx, pcImagePath, nSaveType, nSaveOption);
}
bool CData::saveImage(int nSeriesIdx, int nImageIdx, const char* pcImagePath, int nSaveType, int nSaveOption) {
	// exception
	if(nSeriesIdx < 0 || getSeriesCount() < nSeriesIdx) {
		return false;
	}
	if(nImageIdx < 0) {
		return false;
	}
	if(pcImagePath == NULL) {
		return false;
	}

	// save image
	std::string sImageExtension = getFileExtension(pcImagePath);

	if(sImageExtension == "dcm") {
		// todo
	}
	else if(sImageExtension == "jpg" || sImageExtension == "png" || sImageExtension == "bmp") {
		return saveImage_opencv(nSeriesIdx, nImageIdx, pcImagePath, nSaveType, nSaveOption);
	}
	else if(sImageExtension == "nii") {
		// todo
	}
	else if(sImageExtension == "gz") {
		// todo
	}
	else if(sImageExtension == "tiff") {
		return saveImage_libtiff(nSeriesIdx, nImageIdx, pcImagePath, nSaveType, nSaveOption);
	}

	return false;
}
bool CData::saveImages(int nSeriesIdx, const char* pcSaveDir, const char* pcExtension, int nSaveType, int nSaveOption) {
	if(nSeriesIdx < 0 || getSeriesCount() < nSeriesIdx) {
		return false;
	}

	// series info
	CSeries* pCiSeries = getSeries(nSeriesIdx);
	if(pCiSeries == NULL) {
		return false;
	}
	int nEndIdx = pCiSeries->getImageCount();

	// save image
	if(pcExtension == "dcm") {
		// todo
	}
	else if(pcExtension == "jpg" || pcExtension == "png" || pcExtension == "bmp") {
		for(int i=0; i< nEndIdx; i++) {
			std::string sImagePath = convertImageSavePath(nSeriesIdx, i, pcSaveDir, pcExtension, nSaveType, nSaveOption);
			std::vector<std::string> s = splitPath(sImagePath);
			std::string path = "";
			for(int j=0, nj=s.size()-1; j< nj; j++) {
				path += s[j] + "\\";
				mkdir(path.c_str());
			}

			saveImage_opencv(nSeriesIdx, i, sImagePath.c_str(), nSaveType, nSaveOption);
		}
	}
	else if(pcExtension == "nii") {
		// todo
	}
	else if(pcExtension == "gz") {
		// todo
	}
	else if(pcExtension == "tiff") {
		std::string sImagePath = convertImageSavePath(nSeriesIdx, -1, pcSaveDir, pcExtension, nSaveType, nSaveOption);
		return saveImage_libtiff(nSeriesIdx, -1, sImagePath.c_str(), nSaveType, nSaveOption);
	}


	return false;
}
bool CData::saveImage_opencv(int nSeriesIdx, int nImageIdx, const char* pcImagePath, int nSaveType, int nSaveOption) {
	CImage<short>* pCiImage = getCImage(nSeriesIdx, nImageIdx);
	if(pCiImage == NULL) {
		return false;
	}

	try {
		switch(nSaveType) {
			case SAVE_IMAGE_ONLY: 
			{
				short* psCopyImg = NULL;
				int nWidth = 0;
				int nHeight = 0;
				pCiImage->copyImage(psCopyImg, nWidth, nHeight);

				bool doClearImage = false;
				if(psCopyImg == NULL) {
					// lazy loading 된 경우, saveImage 하는 동안에만 읽고 메모리 해제
					doClearImage = copyImage(nSeriesIdx, nImageIdx, psCopyImg, nWidth, nHeight);

					if(psCopyImg == NULL) {
						return false;
					}
				}

				IplImage* image = cvCreateImage(cvSize(nWidth, nHeight), 8, 1);
				cvSet(image, cvScalar(0, 0, 0));
				
				if(nSaveOption == SAVE_DEFAULT) {
					convertDefaultImage(psCopyImg, nWidth, nHeight);
				}
				else if(nSaveOption == SAVE_IMAGE_WINDOW) {
					int nWL=0, nWW=0;
					pCiImage->getWLWW(nWL, nWW);

					if(nWL==0 && nWW==0) {
						convertDefaultImage(psCopyImg, nWidth, nHeight);
					}
					else {
						convertWindowImage(psCopyImg, nWidth, nHeight, nWL, nWW);
					}
				}
				
				for(int r=0; r< nHeight; r++) {
					for(int c=0; c< nWidth; c++) {
						image->imageData[r*image->widthStep + c] = (unsigned char) psCopyImg[r*nWidth + c];
					}
				}

				SAFE_DELETE_ARRAY(psCopyImg);
				cvSaveImage(pcImagePath, image);
				cvReleaseImage(&image);

				if(doClearImage) {
					pCiImage->clearImage();
				}

				return true;
			}
			case SAVE_MASK_ONLY:
			{
				unsigned char* pucCopyMask = NULL;
				int nWidth = 0, nHeight = 0;
				pCiImage->copyMask(pucCopyMask, nWidth, nHeight);

				bool doClearMask = false;
				if(pucCopyMask == NULL) {
					doClearMask = copyMask(nSeriesIdx, nImageIdx, pucCopyMask, nWidth, nHeight);

					if(pucCopyMask == NULL) {
						return false;
					}
				}
				
				IplImage* image = cvCreateImage(cvSize(nWidth, nHeight), 8, 1);
				cvSet(image, cvScalar(0, 0, 0));
				if(nSaveOption == SAVE_MASK_BINARY) {
					convertDefaultImage(pucCopyMask, nWidth, nHeight);
				}

				for(int r=0; r< nHeight; r++) {
					for(int c=0; c< nWidth; c++) {
						image->imageData[r*image->widthStep + c] = (unsigned char) pucCopyMask[r*nWidth + c];
					}
				}

				SAFE_DELETE_ARRAY(pucCopyMask);
				cvSaveImage(pcImagePath, image);
				cvReleaseImage(&image);

				return true;
			}
			case SAVE_OVERLAY:
			{
				short* psCopyImg = NULL;
				int nWidth = 0;
				int nHeight = 0;
				pCiImage->copyImage(psCopyImg, nWidth, nHeight);

				bool doClearImage = false;
				if(psCopyImg == NULL) {
					// lazy loading 된 경우, saveImage 하는 동안에만 읽고 메모리 해제
					doClearImage = copyImage(nSeriesIdx, nImageIdx, psCopyImg, nWidth, nHeight);

					if(psCopyImg == NULL) {
						return false;
					}
				}

				unsigned char* pucCopyMask = NULL;
				pCiImage->copyMask(pucCopyMask, nWidth, nHeight);

				bool doClearMask = false;
				if(pucCopyMask == NULL) {
					doClearMask = copyMask(nSeriesIdx, nImageIdx, pucCopyMask, nWidth, nHeight);

					if(pucCopyMask == NULL) {
						return false;
					}
				}


				IplImage* image = cvCreateImage(cvSize(nWidth, nHeight), 8, 3);
				cvSet(image, cvScalar(0, 0, 0));
				
				if(nSaveOption == SAVE_DEFAULT) {
					convertDefaultImage(psCopyImg, nWidth, nHeight);
				}
				else if(nSaveOption == SAVE_IMAGE_WINDOW) {
					int nWL=0, nWW=0;
					pCiImage->getWLWW(nWL, nWW);

					if(nWL==0 && nWW==0) {
						convertDefaultImage(psCopyImg, nWidth, nHeight);
					}
					else {
						convertWindowImage(psCopyImg, nWidth, nHeight, nWL, nWW);
					}
				}

				for(int row=0; row< nHeight; row++) {
					for(int col=0; col< nWidth; col++) {
						int idx = nWidth*row + col;
						int imageIdx = image->widthStep*row + image->nChannels*col;

						if(pucCopyMask[idx] == 0) {
							image->imageData[imageIdx+0] = (unsigned char)psCopyImg[idx];
							image->imageData[imageIdx+1] = (unsigned char)psCopyImg[idx];
							image->imageData[imageIdx+2] = (unsigned char)psCopyImg[idx];
						}
						else if(pucCopyMask[idx] == 1) {
							double r=255, g=0, b=0;
							double fOpacity = 0.5;
							image->imageData[imageIdx+0] = (unsigned char)(fOpacity*psCopyImg[idx] + (1-fOpacity)*b);
							image->imageData[imageIdx+1] = (unsigned char)(fOpacity*psCopyImg[idx] + (1-fOpacity)*g);
							image->imageData[imageIdx+2] = (unsigned char)(fOpacity*psCopyImg[idx] + (1-fOpacity)*r);
						}
						else if(pucCopyMask[idx] == 5) {
							double r=0, g=0, b=255;
							double fOpacity = 0.5;
							image->imageData[imageIdx+0] = (unsigned char)(fOpacity*psCopyImg[idx] + (1-fOpacity)*b);
							image->imageData[imageIdx+1] = (unsigned char)(fOpacity*psCopyImg[idx] + (1-fOpacity)*g);
							image->imageData[imageIdx+2] = (unsigned char)(fOpacity*psCopyImg[idx] + (1-fOpacity)*r);
						}
						else if(pucCopyMask[idx] == 3) {
							double r=0, g=255, b=255;
							double fOpacity = 0.5;
							image->imageData[imageIdx+0] = (unsigned char)(fOpacity*psCopyImg[idx] + (1-fOpacity)*b);
							image->imageData[imageIdx+1] = (unsigned char)(fOpacity*psCopyImg[idx] + (1-fOpacity)*g);
							image->imageData[imageIdx+2] = (unsigned char)(fOpacity*psCopyImg[idx] + (1-fOpacity)*r);
						}
						else {
							double r=0, g=255, b=0;
							double fOpacity = 0.5;
							image->imageData[imageIdx+0] = (unsigned char)(fOpacity*psCopyImg[idx] + (1-fOpacity)*b);
							image->imageData[imageIdx+1] = (unsigned char)(fOpacity*psCopyImg[idx] + (1-fOpacity)*g);
							image->imageData[imageIdx+2] = (unsigned char)(fOpacity*psCopyImg[idx] + (1-fOpacity)*r);
						}
					}
				}

				cvSaveImage(pcImagePath, image);
				cvReleaseImage(&image);

				SAFE_DELETE_ARRAY(psCopyImg);
				SAFE_DELETE_ARRAY(pucCopyMask);
			}
			default:
			{
				return false;
			}
		}
	}
	catch(char* err) {
		return false;
	}
	
	return false;
}
bool CData::saveImage_libtiff(int nSeriesIdx, int nImageIdx, const char* pcImagePath, int nSaveType, int nSaveOption) {
	CSeries* pCiSeries = getSeries(nSeriesIdx);

	if(pCiSeries == NULL) {
		return false;
	}

	try {
		switch(nSaveType) {
			case SAVE_IMAGE_ONLY: 
			{
				// todo
				break;
			}
			case SAVE_MASK_ONLY:
			{
				TIFF *out = TIFFOpen(pcImagePath, "w");
				if (out == NULL) {
					throw "can't open file";
				}

				int nEndIdx = pCiSeries->getImageCount();
				for (int idx = 0; idx < nEndIdx; idx++){
					int row, col;
					uint16 config, nsamples = 1;
					config = PLANARCONFIG_CONTIG;
			
					CImage<short>* pCiImage = pCiSeries->getImage(idx);
					if(pCiImage) {
						return false;
					}

					int nWidth = 0, nHeight = 0;
					unsigned char* pucMask = NULL;
					
					pCiImage->copyMask(pucMask, nWidth, nHeight);

					bool doClearMask = false;
					if(pucMask == NULL) {
						doClearMask = copyMask(nSeriesIdx, idx, pucMask, nWidth, nHeight);
						if(pucMask == NULL) {
							return false;
						}
					}

					unsigned char* buf = new unsigned char [nWidth];

					TIFFSetField(out, TIFFTAG_IMAGELENGTH, nHeight);
					TIFFSetField(out, TIFFTAG_IMAGEWIDTH, nWidth);
					TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, nsamples);
					TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);
					TIFFSetField(out, TIFFTAG_COMPRESSION, 1);
					TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
					TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
					TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
					TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(out, nWidth*nsamples));
					TIFFSetField(out, TIFFTAG_SUBFILETYPE, FILETYPE_PAGE);
					TIFFSetField(out, TIFFTAG_PAGENUMBER, idx, nEndIdx);

					for (row = 0; row < nHeight; row++){
						for(col=0; col < nWidth; col++){
							buf[col] = pucMask[row*nWidth + col];
						}
						if (TIFFWriteScanline(out, buf, row) != 1 )
						{
							printf("Unable to write a row\n") ;
							break ;
						}
					}

					TIFFWriteDirectory(out);
					SAFE_DELETE_ARRAY(buf);

					if(doClearMask == false) {
						
					}
				}
				TIFFClose(out);

				return true;
			}
		}
	}
	catch(char* err) {
		if(m_sLog.is_open()) {
			m_sLog << "save 실패 => " << "\n";
		}
	}

	return false;
}
bool CData::saveImage_dcmtk(int nSeriesIdx, int nImageIdx, const char* pcImagePath, int nSaveType, int nSaveOption) {
	// todo
	return false;
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
void CData::searchingDir(std::string path, std::vector<std::string>& sFilePaths) {
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
void CData::makeDir(std::string sPath) {
	std::vector<std::string> s = splitPath(sPath);

	if(getFileName(sPath) != "") {
		std::string newPath = "";
		for(int i=0, ni=s.size()-1; i<ni; i++) {
			newPath += s[i];
			if (0 != access(newPath.c_str(), 06)) {
				mkdir(newPath.c_str());
			}
			newPath += "\\";
		}
	}
	else {
		std::string newPath = "";
		for(int i=0, ni=s.size(); i<ni; i++) {
			newPath += s[i];
			if (0 != access(newPath.c_str(), 06)) {
				mkdir(newPath.c_str());
			}
			newPath += "\\";
		}
	}
}
std::string CData::getFileExtension(std::string sFilePath) {
	int nFindFileExtensionIdx = sFilePath.rfind(".") + 1;

	return sFilePath.substr(nFindFileExtensionIdx);
}
std::string CData::getFileName(std::string sFilePath) {
	int nFindFileNameIdx = sFilePath.rfind("\\") + 1;
	int nFindFileExtensionIdx = sFilePath.rfind(".");

	return(sFilePath.substr(nFindFileNameIdx, nFindFileExtensionIdx - nFindFileNameIdx));
}
std::vector<std::string> CData::splitPath(std::string sPath) {
	return splitString(sPath, '\\');
}
std::vector<std::string> CData::splitString(std::string string, char separator)
{
	std::string d(1, separator);
	std::vector<std::string> out;
	std::vector<int> de;
	de.push_back(-1);

	for(int i=0, ni=string.size(); i<ni; i++) {
		if(d.find(string[i]) == -1)
			continue;
		de.push_back(i);
	}
	de.push_back(string.size());

	for(int i=1, ni=de.size(); i<ni; i++) {
		int s = de[i-1] + 1;
		int e = de[i] - 1;
		if(s>e)
			continue;
		out.push_back(string.substr(s, e-s+1));
	}

	/*
	std::vector<std::string> out;

	int nTokenCnt = 0;
	for(int i=0, ni=string.length(); i< ni; i++) {
		if(string[i] == separator) {
			nTokenCnt++;
		}
	}

	if(nTokenCnt > 0) {
		char temp[2048] = {0};
		sprintf(temp, "%s", string.c_str());
		char* token = NULL;
		char* context = NULL;
		token = strtok_s(temp, &separator, &context);
		out.push_back(token);

		while(token) {
			token = strtok_s(NULL, &separator, &context);
			if(token) {
				out.push_back(token);
			}
		}
	}
	*/

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
int CData::find(std::string string, std::string value, int n) {
	int pos = 0;
	for(int i=0; i< n; i++) {
		pos = string.find(value, pos+1);
	}

	return pos;
}
int CData::rfind(std::string string, std::string value, int n) {
	int pos = 0;
	for(int i=0; i< n; i++) {
		pos = string.rfind(value, pos-1);
	}

	return pos;
}

////////////////////////////////////////////////////////////////////
template<typename T>
bool CData::convertDefaultImage (T* &pImage, int nWidth, int nHeight) {
	if(pImage == NULL) {
		return false;
	}

	int nSetMax = 255;
	int nSetMin = 0;
	int nMin = INT_MAX;
	int nMax = INT_MIN;

	for(int i=0; i<nWidth; i++) {
		for(int j=0; j<nHeight; j++) {
			if( pImage[ j*nWidth + i ] > nMax ) {
				nMax = pImage[ j*nWidth + i];
			}

			if( pImage[ j*nWidth + i ] < nMin ) {
				nMin = pImage[ j*nWidth + i ];
			}
		}
	}

	// normalization
	if(nMax-nMin != 0) {
		for(int i=0; i<nWidth; i++) {
			for(int j=0; j<nHeight; j++) {
				pImage[ j*nWidth + i] = ( pImage[ j*nWidth + i ] - nMin ) * (nSetMax-nSetMin) / ( nMax - nMin ) + nSetMin;
			}
		}
	}

	return true;
}
template<typename T>
bool CData::convertWindowImage (T* &pImage, int nWidth, int nHeight, int nWL, int nWW) {
	if(pImage == NULL) {
		return false;
	}

	int nWindowMinValue = nWL - nWW/2;
	int nWindowMaxValue = nWL + nWW/2;

	for(int row=0; row< nHeight; row++) {
		for(int col=0; col< nWidth; col++) {
			int index = row*nWidth + col;
			int value = (int)pImage[index];

			// 
			if(nWindowMinValue > value) {
				pImage[index] = 0;
			}
			else if(nWindowMaxValue < value) {
				pImage[index] = 255;
			}
			else {
				float fNormValue = (float)(pImage[index]-nWindowMinValue) / (float)(nWindowMaxValue-nWindowMinValue) * 255;
				pImage[index] = (int)fNormValue;
			}
		}
	}
	return true;
}

////////////////////////////////////////////////////////////////////
template<typename T1, typename T2>
bool CData::isSameImage(CImage<T1>* pCiImage1, CImage<T2>* pCiImage2) {
	try {
		if(pCiImage1 != NULL && pCiImage2 != NULL) {
			// 1. 이름이 동일할 때
			if(pCiImage1->getImageNameRemoveTag() == pCiImage2->getImageNameRemoveTag()) {
				return true;
			}

			/*
			// 2. 이름 뒤에 숫자가 동일할 때 (XXX_001)
			std::vector<std::string> s1 = splitString(pCiImage1->getImageName().c_str(), '_');
			std::vector<std::string> s2 = splitString(pCiImage2->getImageName().c_str(), '_');

			if(s1.size() > 1 && s2.size() > 1) {
				if(s1[s1.size()-1] == s2[s2.size()-1]) {
					return true;
				}
			}
			*/
		}
	}
	catch(int nErrCode) {
		
	}
	
	return false;
}
bool CData::isSameSeries(CSeries* pCiSeries1, CSeries* pCiSeries2) {
	if(pCiSeries1 != NULL && pCiSeries2 != NULL) {
		// 1. PatientName, StudyName, SeriesName이 같은 경우
		if ((pCiSeries1->m_sPatientName == pCiSeries2->m_sPatientName) && 
			(pCiSeries1->m_sStudyName == pCiSeries2->m_sStudyName) &&
			(pCiSeries1->m_sSeriesName == pCiSeries2->m_sSeriesName)) {
			return true;
		}

		// 2. Series Path가 같은 경우
		if (pCiSeries1->m_sSeriesPath == pCiSeries2->m_sSeriesPath) {
			return true;
		}

		/*
		if ((pCiSeries1->m_sPatientName == pCiSeries2->m_sPatientName) && 
			(pCiSeries1->m_sSeriesName == pCiSeries2->m_sSeriesName)) {
			return true;
		}
		*/
	}

	return false;
}

template <typename T>
bool CData::sorting(CImage<T>* &pCiImage1, CImage<T>* &pCiImage2) {
	float fImagePositionX1, fImagePositionY1, fImagePositionZ1;
	float fImagePositionX2, fImagePositionY2, fImagePositionZ2;

	pCiImage1->getImagePosition(fImagePositionX1, fImagePositionY1, fImagePositionZ1);
	pCiImage2->getImagePosition(fImagePositionX2, fImagePositionY2, fImagePositionZ2);

	if(fImagePositionZ1 != 0 && fImagePositionZ2 != 0) {
		return fImagePositionZ1 > fImagePositionZ2;
	}
	else {
		return sorting_fileName(pCiImage1, pCiImage2);
	}
}
template <typename T>
bool CData::sorting_fileName(CImage<T>* &pCiImage1, CImage<T>* &pCiImage2) {
	// 문자열 비교 //
	std::string s1 = pCiImage1->getImageName();
	std::string s2 = pCiImage2->getImageName();

	int n = strcmp(s1.c_str(), s2.c_str()); 

	return n< 0? true : false;
}

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
	// 3. 파일명에 "mask" 또는 "label" 또는 "segmentation" 단어를 포함하는 경우
	if(sFileName.find("mask") != std::string::npos || 
	   sFileName.find("label") != std::string::npos ||
	   sFileName.find("segmentation") != std::string::npos ||
	   sFileName.find("seg") != std::string::npos)
	{
		return true;
	}
	// 4. 상위 디렉토리 명이 mask 또는 label 단어를 포함하는 경우
	std::vector<std::string> paths = splitPath(sPath);
	if(paths.size() >= 2) {
		// case/mask/dcm
		std::string sParentFolderName = paths[paths.size()-2];
		if(sParentFolderName.find("mask") != std::string::npos || sParentFolderName.find("label") != std::string::npos) {
			return true;
		}
	}
	if(paths.size() >= 3) {
		// mask/case/dcm
		std::string sParentFolderName = paths[paths.size()-3];
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

bool CData::isExcludeFile(std::string sPath) {
	if(true) {
		std::vector<std::string> paths = splitPath(sPath);
		if(paths.size() >= 2) {
			std::string sParentFolderName = paths[paths.size()-2];
			if(sParentFolderName.find("img") != std::string::npos || sParentFolderName.find("overlay") != std::string::npos) {
				return true;
			}
		}
		if(paths.size() >= 3) {
			std::string sParentFolderName = paths[paths.size()-3];
			if(sParentFolderName.find("img") != std::string::npos || sParentFolderName.find("overlay") != std::string::npos) {
				return true;
			}
		}
	}

	return false;
}


std::string CData::removeNameTag(std::string sFileName) {
	std::string sRemovedTag = sFileName;

	// 1. 파일명의 첫 두글자가 "c_", "m_"으로 시작하는 경우
	if(sRemovedTag.substr(0, 2).compare("m_") == 0) {
		replaceAll(sRemovedTag, std::string("m_"), std::string(""));
	}
	if(sRemovedTag.substr(0, 2).compare("c_") == 0) {
		replaceAll(sRemovedTag, std::string("c_"), std::string(""));
	}

	// 2. 파일명에 _mask, _Mask가 포함되는 경우
	replaceAll(sRemovedTag, std::string("_mask"), std::string(""));
	replaceAll(sRemovedTag, std::string("_Mask"), std::string(""));

	return sRemovedTag;				
}
template<typename T>
bool CData::modifySeries(CSeries* pCiSeries, std::vector<CImage<T>*> pCiImages) {
	if(pCiSeries) {
		int nImageCnt = pCiImages.size();
		if(nImageCnt <= 0) {return false;}

		CImage<T>* pCiImage = pCiImages[0];
		std::string sImagePath = pCiImage->getImagePath();
		std::vector<std::string> paths = splitPath(sImagePath);

		if(true) // patient name /////////////////////////////////////////////////
		{
			// 1. imagePath 기준으로 상위 n단계 폴더명
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
		}
		if(true) // study name /////////////////////////////////////////////////
		{
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
		}
		if(true) // series name /////////////////////////////////////////////////
		{
			if (nImageCnt == 1) {
				// 한 파일에 이미지가 1개 들어있을 때
				if (paths.size() - 2 >= 0) {
					std::string seriesName = paths[paths.size()-2];
					
					if(seriesName.compare("dcm") == 0 || seriesName.compare("mask") == 0) {
						// Series 밑에 dcm,mask등으로 더 세분화 되어 있는 경우
						pCiSeries->m_sSeriesName = removeNameTag(paths[paths.size()-3]);
						std::string sExtension = getFileExtension(pCiSeries->m_sSeriesName);
						if(sExtension != pCiSeries->m_sSeriesName) {
							replaceAll(pCiSeries->m_sSeriesName, std::string(".") + sExtension, std::string(""));
						}

						int nFindFileNameIdx = rfind(sImagePath, "\\", 2) + 1;
						std::string sSeriesPath = sImagePath.substr(0, nFindFileNameIdx-1);
						pCiSeries->m_sSeriesPath = sSeriesPath;
					}
					else {
						pCiSeries->m_sSeriesName = removeNameTag(seriesName);
						std::string sExtension = getFileExtension(pCiSeries->m_sSeriesName);
						if(sExtension != pCiSeries->m_sSeriesName) {
							replaceAll(pCiSeries->m_sSeriesName, std::string(".") + sExtension, std::string(""));
						}

						int nFindFileNameIdx = rfind(sImagePath, "\\", 1) + 1;
						std::string sSeriesPath = sImagePath.substr(0, nFindFileNameIdx-1);
						pCiSeries->m_sSeriesPath = sSeriesPath;
					}
				}
			}
			else if (nImageCnt > 1) {
				// 한 파일에 이미지가 n개 들어있을 때
				if (paths.size() - 1 >= 0) {
					std::string seriesName = paths[paths.size() - 1]; // "Portal_Ax_Mask"
					
					if(seriesName.compare("dcm") == 0 || seriesName.compare("mask") == 0) {
						// Series 밑에 dcm,mask등으로 더 세분화 되어 있는 경우
						pCiSeries->m_sSeriesName = removeNameTag(paths[paths.size() - 2]);
						std::string sExtension = getFileExtension(pCiSeries->m_sSeriesName);
						if(sExtension != pCiSeries->m_sSeriesName) {
							replaceAll(pCiSeries->m_sSeriesName, std::string(".") + sExtension, std::string(""));
						}

						int nFindFileNameIdx = rfind(sImagePath, "\\", 1) + 1;
						std::string sSeriesPath = sImagePath.substr(0, nFindFileNameIdx-1);
						pCiSeries->m_sSeriesPath = sSeriesPath;
					}
					else {
						pCiSeries->m_sSeriesName = removeNameTag(seriesName);
						std::string sExtension = getFileExtension(pCiSeries->m_sSeriesName);
						if(sExtension != pCiSeries->m_sSeriesName) {
							replaceAll(pCiSeries->m_sSeriesName, std::string(".") + sExtension, std::string(""));
						}

						int nFindFileNameIdx = rfind(sImagePath, "\\", 1) + 1;
						std::string sSeriesPath = sImagePath.substr(0, nFindFileNameIdx-1);
						pCiSeries->m_sSeriesPath = sSeriesPath;
					}
				}
			}
		}
	}

	return false;
}

std::string CData::convertImageSavePath(int nSeriesIdx, int nImageIdx, const char* pcSaveDir, const char* pcExtension, int nSaveType, int nSaveOption) {
	CSeries* pCiSeries = getSeries(nSeriesIdx);

	if(pCiSeries) {
		std::string sSeriesName = pCiSeries->m_sSeriesName;
		std::string sStudyName = pCiSeries->m_sStudyName;
		std::string sPatientName = pCiSeries->m_sPatientName;

		if(nSaveType == SAVE_IMAGE_ONLY || nSaveType == SAVE_OVERLAY) {
			std::string sImageName = pCiSeries->getImage(nImageIdx)->getImageName();
			std::string sImagePath = pcSaveDir + std::string("\\") + sStudyName + std::string("\\") + sImageName + std::string(".") + pcExtension;

			return sImagePath;
		}
		else if(nSaveType == SAVE_MASK_ONLY) {
			std::string sMaskName = pCiSeries->getImage(nImageIdx)->getMaskName();
			std::string sMaskPath = "";

			// maskName에 조건이 이미 들어있는 경우
			// 1. 파일명의 첫 두글자가 "m_"으로 시작하는 경우
			if(sMaskName.substr(0, 2).compare("m_") == 0) {
				sMaskPath = pcSaveDir + std::string("\\") + sMaskName + std::string(".") + pcExtension;
			}
			// 2. 파일명에 "_m_" 문자열이 포함된 경우
			else if(sMaskName.find("_m_") != std::string::npos) {
				sMaskPath = pcSaveDir + std::string("\\") + sMaskName + std::string(".") + pcExtension;
			}
			else {
				// 아무것도 없는 경우
				sMaskPath = pcSaveDir + std::string("\\") + sStudyName + std::string("\\m_") + sMaskName + std::string(".") + pcExtension;
			}
			
			return sMaskPath;
		}
	}

	return "";
}
