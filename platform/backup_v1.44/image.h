#pragma once
#include <string>

#define SAFE_DELETE(p) {if(p) delete(p); p=NULL;}
#define SAFE_DELETE_ARRAY(p)	{ if(p) delete[](p); p = NULL;}
#define SAFE_DELETE_VOLUME(p, depth)	{ if(p) {for (int i=0;i<depth;i++)	if(p[i]) delete[](p[i]); } delete[] p; p=NULL;};

class CSeries;
template <typename T>
class CImage {
public:
	enum LOAD_STATUS {
		SUCCESS=1, UNREAD=0, UNREADBLE_FILE=-1
	};

// variable
public:
	// image
	std::string m_sImagePath;			// image file path
	std::string m_sImageName;			// image file name
	std::string m_sImageNameRemoveTag;	// imagename에서 tag를 제거한 이름
	std::string m_sImageExtension;		// image file extension
	int m_nImageFileStatus;				// image file status
	bool m_isMultipleImages;			// image single/multiple slice in one file
	
	T* m_image;							// image
	int m_nWidth;						// image width
	int m_nHeight;						// image height
	int m_nChannel;						// image channel

	float m_fImagePositionX;			// image position X
	float m_fImagePositionY;			// image position Y
	float m_fImagePositionZ;			// image position Z

	float m_fImageOrientationRowX;		// image orientaiton row X
	float m_fImageOrientationRowY;		// image orientaiton row Y
	float m_fImageOrientationRowZ;		// image orientaiton row Z
	float m_fImageOrientationColX;		// image orientation col X
	float m_fImageOrientationColY;		// image orientation col Y
	float m_fImageOrientationColZ;		// image orientation col Z

	float m_fPixelSpacingX;				// pixel spacing
	float m_fPixelSpacingY;				// pixel spacing
	float m_fSliceThickness;			// slice thickness

	int m_nWL;							// window level
	int m_nWW;							// window width
	int m_nInstanceNumber;				// instance number

	// mask
	std::string m_sMaskPath;			// mask file path
	std::string m_sMaskName;			// mask file name
	std::string m_sMaskNameRemoveTag;	// maskname에서 tag를 제거한 이름
	std::string m_sMaskExtension;		// mask file extension
	int m_nMaskFileStatus;				// mask file status
	bool m_isMultipleMasks;				// mask single/multiple slice in one file
	
	unsigned char* m_pucMask;			// mask
	int m_nMaskChannel;					// mask channel

	// series
	CSeries* m_series;					// series

// function
public:
	CImage();
	~CImage();
	void init();
	void clear();
	CImage<T>* copy();
	template <typename T2>
	void convertTo(CImage<T2>* &pCiOtherTypeImage);

	// image ///////////////////////////////////////////////////
	// image path, name, extension
	std::string getImagePath();
	void setImagePath(std::string sImagePath);
	std::string getImageName();
	void setImageName(std::string sImageName);
	std::string getImageNameRemoveTag();
	void setImageNameRemoveTag(std::string sImageNameRemoveTag);
	std::string getImageExtension();
	void setImageExtension(std::string sImageExtension);
	int getImageFileStatus();
	void setImageFileStatus(int nFileStatus);
	bool getIsMultipleImages();
	void setIsMultipleImages(bool isMultipleImages);

	// image, width, height, channel, size
	T* getImage();
	void clearImage();
	bool setImage(CImage<T>* pCiImage);
	bool setImage(T* pImage, int nWidth, int nHeight, int nChannel=1);
	bool copyImage(T* &pImage, int& nWidth, int& nHeight);
	bool copyImage(T* &pImage, int& nWidth, int& nHeight, int& nChannel);
	
	int getWidth();
	int getHeight();
	int getChannel();
	int getSize();

	// image position
	void getImagePosition(float& fImagePositionX, float& fImagePositionY, float& fImagePositionZ);
	void setImagePosition(float fImagePositionX, float fImagePositionY, float fImagePositionZ);

	// image orientation
	void getImageOrientation(float& fImageOrientationRowX, float& fImageOrientationRowY, float& fImageOrientationRowZ, float& fImageOrientationColX, float& fImageOrientationColY, float& fImageOrientationColZ);
	void setImageOrientation(float fImageOrientationRowX, float fImageOrientationRowY, float fImageOrientationRowZ, float fImageOrientationColX, float fImageOrientationColY, float fImageOrientationColZ);

	// image pixel spacing
	void getPixelSpacing(float& fPixelSpacingX, float& fPixelSpacingY);
	void setPixelSpacing(float fPixelSpacingX, float fPixelSpacingY);

	// slice thickness
	float getSliceThickness();
	void setSliceThickness(float fSliceThickness);

	// window level, window width
	void getWLWW(int &nWL, int &nWW);
	void setWLWW(int nWL, int nWW);

	// instance number
	int getInstanceNumber();
	void setInstanceNumber(int nInstanceNumber);

	// mask path, name, extension
	std::string getMaskPath();
	void setMaskPath(std::string sMaskPath);
	std::string getMaskName();
	void setMaskName(std::string sMaskName);
	std::string getMaskNameRemoveTag();
	void setMaskNameRemoveTag(std::string sMaskNameRemoveTag);
	std::string getMaskExtension();
	void setMaskExtension(std::string sMaskExtension);
	int getMaskFileStatus();
	void setMaskFileStatus(int nFileStatus);
	bool getIsMultipleMasks();
	void setIsMultipleMasks(bool isMultipleMasks);

	unsigned char* getMask();
	bool setMask(CImage<T>* pCiImage);
	template <typename T2>
	bool setMask(CImage<T2>* pCiImage);
	bool setMask(unsigned char* pucMask, int nWidth, int nHeight, int nChannel=1);
	bool copyMask(unsigned char* &pucMask, int &nWidth, int &nHeight);
	bool copyMask(unsigned char* &pucMask, int &nWidth, int &nHeight, int &nChannel);

	// series 정보
	CSeries* getSeries();
	void setSeries(CSeries* pCiSeries);
};


// image.cpp
template <typename T>
CImage<T>::CImage() {
	init();
}
template <typename T>
CImage<T>::~CImage() {
	clear();
}

template <typename T>
void CImage<T>::init() {
	m_sImagePath = "";
	m_sImageName = "";
	m_sImageNameRemoveTag = "";
	m_sImageExtension = "";
	m_nImageFileStatus = UNREAD;
	m_isMultipleImages = false;

	m_image = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nChannel = 0;

	m_fImagePositionX = 0;
	m_fImagePositionY = 0;
	m_fImagePositionZ = 0;

	m_fImageOrientationRowX = 0;
	m_fImageOrientationRowY = 0;
	m_fImageOrientationRowZ = 0;
	m_fImageOrientationColX = 0;
	m_fImageOrientationColY = 0;
	m_fImageOrientationColZ = 0;

	m_fPixelSpacingX = 0;
	m_fPixelSpacingY = 0;
	m_fSliceThickness = 0;

	m_nWL = 0;
	m_nWW = 0;
	m_nInstanceNumber = 0;

	m_sMaskPath = "";
	m_sMaskName = "";
	m_sMaskNameRemoveTag = "";
	m_sMaskExtension = "";
	m_nMaskFileStatus = UNREAD;
	m_isMultipleMasks = false;

	m_pucMask = NULL;
	m_nMaskChannel = 0;

	m_series = NULL;
}
template <typename T>
void CImage<T>::clear() {
	clearImage();
}
template <typename T>
CImage<T>* CImage<T>::copy() {
	CImage<T>* pCiImage = new CImage<T>();

	pCiImage->m_sImagePath = m_sImagePath;
	pCiImage->m_sImageName = m_sImageName;
	pCiImage->m_sImageNameRemoveTag = m_sImageNameRemoveTag;
	pCiImage->m_sImageExtension = m_sImageExtension;
	pCiImage->m_nImageFileStatus = m_nImageFileStatus;
	pCiImage->m_isMultipleImages = m_isMultipleImages;

	copyImage(pCiImage->m_image, pCiImage->m_nWidth, pCiImage->m_nHeight, pCiImage->m_nChannel);

	pCiImage->m_fImagePositionX = m_fImagePositionX;
	pCiImage->m_fImagePositionY = m_fImagePositionY;
	pCiImage->m_fImagePositionZ = m_fImagePositionZ;

	pCiImage->m_fImageOrientationRowX = m_fImageOrientationRowX;
	pCiImage->m_fImageOrientationRowY = m_fImageOrientationRowY;
	pCiImage->m_fImageOrientationRowZ = m_fImageOrientationRowZ;
	pCiImage->m_fImageOrientationColX = m_fImageOrientationColX;
	pCiImage->m_fImageOrientationColY = m_fImageOrientationColY;
	pCiImage->m_fImageOrientationColZ = m_fImageOrientationColZ;

	pCiImage->m_fPixelSpacingX = m_fPixelSpacingX;
	pCiImage->m_fPixelSpacingY = m_fPixelSpacingY;
	pCiImage->m_fSliceThickness = m_fSliceThickness;

	pCiImage->m_nWL = m_nWL;
	pCiImage->m_nWW = m_nWW;
	pCiImage->m_nInstanceNumber = m_nInstanceNumber;

	pCiImage->m_sMaskPath = m_sMaskPath;
	pCiImage->m_sMaskName = m_sMaskName;
	pCiImage->m_sMaskNameRemoveTag = m_sMaskNameRemoveTag;
	pCiImage->m_sMaskExtension = m_sMaskExtension;
	pCiImage->m_nMaskFileStatus = m_nMaskFileStatus;
	pCiImage->m_isMultipleMasks = m_isMultipleMasks;

	copyMask(pCiImage->m_pucMask, pCiImage->m_nWidth, pCiImage->m_nHeight, pCiImage->m_nMaskChannel);

	pCiImage->m_series = NULL;

	return pCiImage;
}
template <typename T> template <typename T2>
void CImage<T>::convertTo(CImage<T2>* &pCiOtherTypeImage) {
	if(pCiOtherTypeImage != NULL) {
		return;
	}
	pCiOtherTypeImage = new CImage<T2>();
	pCiOtherTypeImage->m_sImagePath = m_sImagePath;
	pCiOtherTypeImage->m_sImageName = m_sImageName;
	pCiOtherTypeImage->m_sImageNameRemoveTag = m_sImageNameRemoveTag;
	pCiOtherTypeImage->m_sImageExtension = m_sImageExtension;
	pCiOtherTypeImage->m_nImageFileStatus = m_nImageFileStatus;
	pCiOtherTypeImage->m_isMultipleImages = m_isMultipleImages;
	
	int nSize = m_nWidth * m_nHeight * m_nChannel;
	if(m_image == NULL || m_nWidth == 0 || m_nHeight == 0 || m_nChannel == 0) {
		pCiOtherTypeImage->m_image = NULL;
		pCiOtherTypeImage->m_nWidth = 0;
		pCiOtherTypeImage->m_nHeight = 0;
		pCiOtherTypeImage->m_nChannel = 1;
	}
	else {
		T2* image = new T2[nSize];
		for(int i=0, ni=nSize; i<ni; i++) {
			image[i] = (T2)m_image[i];
		}
		pCiOtherTypeImage->setImage(image, m_nWidth, m_nHeight, m_nChannel);
		SAFE_DELETE_ARRAY(image);
	}
	
	pCiOtherTypeImage->m_fImagePositionX = m_fImagePositionX;
	pCiOtherTypeImage->m_fImagePositionY = m_fImagePositionY;
	pCiOtherTypeImage->m_fImagePositionZ = m_fImagePositionZ;

	pCiOtherTypeImage->m_fImageOrientationRowX = m_fImageOrientationRowX;
	pCiOtherTypeImage->m_fImageOrientationRowY = m_fImageOrientationRowY;
	pCiOtherTypeImage->m_fImageOrientationRowZ = m_fImageOrientationRowZ;
	pCiOtherTypeImage->m_fImageOrientationColX = m_fImageOrientationColX;
	pCiOtherTypeImage->m_fImageOrientationColY = m_fImageOrientationColY;
	pCiOtherTypeImage->m_fImageOrientationColZ = m_fImageOrientationColZ;

	pCiOtherTypeImage->m_fPixelSpacingX = m_fPixelSpacingX;
	pCiOtherTypeImage->m_fPixelSpacingY = m_fPixelSpacingY;
	pCiOtherTypeImage->m_fSliceThickness = m_fSliceThickness;

	pCiOtherTypeImage->m_nWL = m_nWL;
	pCiOtherTypeImage->m_nWW = m_nWW;
	pCiOtherTypeImage->m_nInstanceNumber = m_nInstanceNumber;

	pCiOtherTypeImage->m_sMaskPath = m_sMaskPath;
	pCiOtherTypeImage->m_sMaskName = m_sMaskName;
	pCiOtherTypeImage->m_sMaskNameRemoveTag = m_sMaskNameRemoveTag;
	pCiOtherTypeImage->m_sMaskExtension = m_sMaskExtension;
	pCiOtherTypeImage->m_nMaskFileStatus = m_nMaskFileStatus;
	pCiOtherTypeImage->m_isMultipleMasks = m_isMultipleMasks;

	int nMaskSize = m_nWidth * m_nHeight * m_nMaskChannel;
	if(m_pucMask == NULL || m_nWidth == 0 || m_nHeight == 0 || m_nMaskChannel == 0) {
		pCiOtherTypeImage->m_pucMask = NULL;
		pCiOtherTypeImage->m_nWidth = 0;
		pCiOtherTypeImage->m_nHeight = 0;
		pCiOtherTypeImage->m_nMaskChannel = 1;
	}
	else {
		unsigned char* pucMask = new unsigned char[nMaskSize];
		for(int i=0, ni=nMaskSize; i<ni; i++) {
			pucMask[i] = m_pucMask[i];
		}
		pCiOtherTypeImage->setMask(pucMask, m_nWidth, m_nHeight, m_nMaskChannel);
		SAFE_DELETE_ARRAY(pucMask);
	}

	pCiOtherTypeImage->m_series = NULL;
}

template <typename T>
std::string CImage<T>::getImagePath() {
	return m_sImagePath;
}
template <typename T>
void CImage<T>::setImagePath(std::string sImagePath) {
	// filepath
	m_sImagePath = sImagePath;
}
template <typename T>
std::string CImage<T>::getImageName() {
	return m_sImageName;
}
template <typename T>
void CImage<T>::setImageName(std::string sImageName) {
	m_sImageName = sImageName;
}
template <typename T>
std::string CImage<T>::getImageNameRemoveTag() {
	return m_sImageNameRemoveTag;
}
template <typename T>
void CImage<T>::setImageNameRemoveTag(std::string sImageNameRemoveTag) {
	m_sImageNameRemoveTag = sImageNameRemoveTag;
}
template <typename T>
std::string CImage<T>::getImageExtension() {
	return m_sImageExtension;
}
template <typename T>
void CImage<T>::setImageExtension(std::string sImageExtension) {
	m_sImageExtension = sImageExtension;
}
template <typename T>
int CImage<T>::getImageFileStatus() {
	return m_nImageFileStatus;
}
template <typename T>
void CImage<T>::setImageFileStatus(int nFileStatus) {
	m_nImageFileStatus = nFileStatus;
}
template <typename T>
bool CImage<T>::getIsMultipleImages() {
	return m_isMultipleImages;
}
template <typename T>
void CImage<T>::setIsMultipleImages(bool isMultipleImages) {
	m_isMultipleImages = isMultipleImages;
}

template<typename T>
void CImage<T>::clearImage() {
	// image
	m_nWidth = 0;
	m_nHeight = 0;
	m_nChannel = 0;
	if(m_image != NULL) {
		SAFE_DELETE_ARRAY(m_image);
	}	
	m_image = NULL;

	// mask
	m_nMaskChannel = 0;
	if(m_pucMask != NULL) {
		SAFE_DELETE_ARRAY(m_pucMask);
	}
	m_pucMask = NULL;
}
template<typename T>
bool CImage<T>::setImage(CImage<T>* pCiImage) {
	// exception
	if(pCiImage == NULL) {
		return false;
	}
	if(pCiImage->m_image == NULL || pCiImage->m_nWidth <= 0 || pCiImage->m_nHeight <= 0 || pCiImage->m_nChannel <= 0) {
		return false;
	}

	m_sImagePath = pCiImage->m_sImagePath;
	m_sImageName = pCiImage->m_sImageName;
	m_sImageNameRemoveTag = pCiImage->m_sImageNameRemoveTag;
	m_sImageExtension = pCiImage->m_sImageExtension;
	m_nImageFileStatus = pCiImage->m_nImageFileStatus;
	m_isMultipleImages = pCiImage->m_isMultipleImages;

	setImage(pCiImage->m_image, pCiImage->m_nWidth, pCiImage->m_nHeight, pCiImage->m_nChannel);

	m_fImagePositionX = pCiImage->m_fImagePositionX;
	m_fImagePositionY = pCiImage->m_fImagePositionY;
	m_fImagePositionZ = pCiImage->m_fImagePositionZ;

	m_fImageOrientationRowX = pCiImage->m_fImageOrientationRowX;
	m_fImageOrientationRowY = pCiImage->m_fImageOrientationRowY;
	m_fImageOrientationRowZ = pCiImage->m_fImageOrientationRowZ;
	m_fImageOrientationColX = pCiImage->m_fImageOrientationColX;
	m_fImageOrientationColY = pCiImage->m_fImageOrientationColY;
	m_fImageOrientationColZ = pCiImage->m_fImageOrientationColZ;

	m_fPixelSpacingX = pCiImage->m_fPixelSpacingX;
	m_fPixelSpacingY = pCiImage->m_fPixelSpacingY;
	m_fSliceThickness = pCiImage->m_fSliceThickness;

	m_nWL = pCiImage->m_nWL;
	m_nWW = pCiImage->m_nWW;
	m_nInstanceNumber = pCiImage->m_nInstanceNumber;

	return true;
}
template <typename T>
bool CImage<T>::setImage(T* pImage, int nWidth, int nHeight, int nChannel=1) {
	// exception
	if(pImage == NULL || nWidth <= 0 || nHeight <= 0 || nChannel <= 0) {
		return false;
	}

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nChannel = nChannel;
	int nSize = nWidth * nHeight* nChannel;

	if(m_image != NULL) {
		SAFE_DELETE_ARRAY(m_image);
	}
	m_image = new T[nSize];
	for(int i=0; i< nSize; i++) {
		m_image[i] = pImage[i];
	}

	return true;
}
template <typename T>
T* CImage<T>::getImage() {
	return m_image;
}
template <typename T>
bool CImage<T>::copyImage(T* &pImage, int& nWidth, int& nHeight) {
	int nChannel = 1;

	return copyImage(pImage, nWidth, nHeight, nChannel);
}
template <typename T>
bool CImage<T>::copyImage(T* &pImage, int& nWidth, int& nHeight, int& nChannel) {
	//
	if(pImage != NULL) {
		return false;
	}

	if(m_image == NULL) {
		pImage = NULL;
		nWidth = 0;
		nHeight = 0;
		nChannel = 0;

		return false;
	}
	else {
		int nSize = m_nWidth * m_nHeight* m_nChannel;

		pImage = new T[nSize];
		for(int i=0; i< nSize; i++) {
			pImage[i] = m_image[i];
		}

		nWidth = m_nWidth;
		nHeight = m_nHeight;
		nChannel = m_nChannel;

		return true;
	}
}
template <typename T>
int CImage<T>::getWidth() {
	return m_nWidth;
}
template <typename T>
int CImage<T>::getHeight() {
	return m_nHeight;
}
template <typename T>
int CImage<T>::getChannel() {
	return m_nChannel;
}
template <typename T>
int CImage<T>::getSize() {
	return m_nWidth * m_nHeight * m_nChannel;
}

template <typename T>
void CImage<T>::getImagePosition(float& fImagePositionX, float& fImagePositionY, float& fImagePositionZ) {
	fImagePositionX = m_fImagePositionX;
	fImagePositionY = m_fImagePositionY;
	fImagePositionZ = m_fImagePositionZ;
}
template <typename T>
void CImage<T>::setImagePosition(float fImagePositionX, float fImagePositionY, float fImagePositionZ) {
	m_fImagePositionX = fImagePositionX;
	m_fImagePositionY = fImagePositionY;
	m_fImagePositionZ = fImagePositionZ;
}

template <typename T>
void CImage<T>::getImageOrientation(float& fImageOrientationRowX, float& fImageOrientationRowY, float& fImageOrientationRowZ, float& fImageOrientationColX, float& fImageOrientationColY, float& fImageOrientationColZ) {
	fImageOrientationRowX = m_fImageOrientationRowX;
	fImageOrientationRowY = m_fImageOrientationRowY;
	fImageOrientationRowZ = m_fImageOrientationRowZ;
	fImageOrientationColX = m_fImageOrientationColX;
	fImageOrientationColY = m_fImageOrientationColY;
	fImageOrientationColZ = m_fImageOrientationColZ;
}
template <typename T>
void CImage<T>::setImageOrientation(float fImageOrientationRowX, float fImageOrientationRowY, float fImageOrientationRowZ, float fImageOrientationColX, float fImageOrientationColY, float fImageOrientationColZ) {
	m_fImageOrientationRowX = fImageOrientationRowX;
	m_fImageOrientationRowY = fImageOrientationRowY;
	m_fImageOrientationRowZ = fImageOrientationRowZ;
	m_fImageOrientationColX = fImageOrientationColX;
	m_fImageOrientationColY = fImageOrientationColY;
	m_fImageOrientationColZ = fImageOrientationColZ;
}

template <typename T>
void CImage<T>::getPixelSpacing(float& fPixelSpacingX, float& fPixelSpacingY) {
	fPixelSpacingX = m_fPixelSpacingX;
	fPixelSpacingY = m_fPixelSpacingY;
}
template <typename T>
void CImage<T>::setPixelSpacing(float fPixelSpacingX, float fPixelSpacingY) {
	m_fPixelSpacingX = fPixelSpacingX;
	m_fPixelSpacingY = fPixelSpacingY;
}

template <typename T>
float CImage<T>::getSliceThickness() {
	return m_fSliceThickness;
}
template <typename T>
void CImage<T>::setSliceThickness(float fSliceThickness) {
	m_fSliceThickness = fSliceThickness;
}

template <typename T>
void CImage<T>::getWLWW(int &nWL, int &nWW) {
	nWL = m_nWL;
	nWW = m_nWW;
}
template <typename T>
void CImage<T>::setWLWW(int nWL, int nWW) {
	m_nWL = nWL;
	m_nWW = nWW;
}

template <typename T>
int CImage<T>::getInstanceNumber() {
	return m_nInstanceNumber;
}
template <typename T>
void CImage<T>::setInstanceNumber(int nInstanceNumber) {
	m_nInstanceNumber = nInstanceNumber;
}

template <typename T>
std::string CImage<T>:: getMaskPath() {
	return m_sMaskPath;
}
template <typename T>
void CImage<T>::setMaskPath(std::string sMaskPath) {
	m_sMaskPath = sMaskPath;
}
template <typename T>
std::string CImage<T>::getMaskName() {
	return m_sMaskName;
}
template <typename T>
void CImage<T>::setMaskName(std::string sMaskName) {
	m_sMaskName = sMaskName;
}
template <typename T>
std::string CImage<T>::getMaskNameRemoveTag() {
	return m_sMaskNameRemoveTag;
}
template <typename T>
void CImage<T>::setMaskNameRemoveTag(std::string sMaskNameRemoveTag) {
	m_sMaskNameRemoveTag = sMaskNameRemoveTag;
}
template <typename T>
std::string CImage<T>::getMaskExtension() {
	return m_sMaskExtension;
}
template <typename T>
void CImage<T>::setMaskExtension(std::string sMaskExtension) {
	m_sMaskExtension = sMaskExtension;
}
template <typename T>
int CImage<T>::getMaskFileStatus() {
	return m_nMaskFileStatus;
}
template <typename T>
void CImage<T>::setMaskFileStatus(int nFileStatus) {
	m_nMaskFileStatus = nFileStatus;
}
template <typename T>
bool CImage<T>::getIsMultipleMasks() {
	return m_isMultipleMasks;
}
template <typename T>
void CImage<T>::setIsMultipleMasks(bool isMultipleMasks) {
	m_isMultipleMasks = isMultipleMasks;
}

template <typename T>
unsigned char* CImage<T>::getMask() {
	return m_pucMask;
}
template <typename T>
bool CImage<T>::setMask(CImage<T>* pCiImage) {
	// exception
	if(pCiImage == NULL) {
		return false;
	}
	if(pCiImage->m_image == NULL || pCiImage->m_nWidth <= 0 || pCiImage->m_nHeight <= 0 || pCiImage->m_nChannel <= 0) {
		return false;
	}

	m_sMaskPath = pCiImage->m_sMaskPath;
	m_sMaskName = pCiImage->m_sMaskName;
	m_sMaskNameRemoveTag = pCiImage->m_sMaskNameRemoveTag;
	m_sMaskExtension = pCiImage->m_sMaskExtension;
	m_nMaskFileStatus = pCiImage->m_nMaskFileStatus;
	m_isMultipleMasks = pCiImage->m_isMultipleMasks;

	setMask(pCiImage->m_pucMask, pCiImage->m_nWidth, pCiImage->m_nHeight, pCiImage->m_nChannel);
}
template <typename T> template <typename T2>
bool CImage<T>::setMask(CImage<T2>* pCiImage) {
	// exception
	if(pCiImage == NULL) {
		return false;
	}
	if(pCiImage->m_image == NULL || pCiImage->m_nWidth <= 0 || pCiImage->m_nHeight <= 0 || pCiImage->m_nChannel <= 0) {
		return false;
	}

	m_sMaskPath = pCiImage->m_sImagePath;
	m_sMaskName = pCiImage->m_sImageName;
	m_sMaskNameRemoveTag = pCiImage->m_sImageNameRemoveTag;
	m_sMaskExtension = pCiImage->m_sImageExtension;
	m_nMaskFileStatus = pCiImage->m_nImageFileStatus;
	m_isMultipleMasks = pCiImage->m_isMultipleImages;

	int nSize = pCiImage->m_nWidth * pCiImage->m_nHeight * pCiImage->m_nChannel;
	if(m_pucMask != NULL) {
		SAFE_DELETE_ARRAY(m_pucMask);
	}
	m_pucMask = new unsigned char[nSize];
	for(int i=0; i< nSize; i++) {
		m_pucMask[i] = pCiImage->m_image[i];
	}
	m_nMaskChannel = pCiImage->m_nChannel;

	return true;
}

template <typename T>
bool CImage<T>::setMask(unsigned char* pucMask, int nWidth, int nHeight, int nChannel) {
	// exception
	if(pucMask == NULL || nWidth != m_nWidth || nHeight != m_nHeight || nChannel <= 0) {
		return false;
	}

	int nSize = nWidth * nHeight * nChannel;

	if(m_pucMask != NULL) {
		SAFE_DELETE_ARRAY(m_pucMask);
	}
	m_pucMask = new unsigned char[nSize];
	for(int i=0; i< nSize; i++) {
		m_pucMask[i] = pucMask[i];
	}
	m_nMaskChannel = nChannel;

	return true;
}
template <typename T>
bool CImage<T>::copyMask(unsigned char* &pucMask, int &nWidth, int &nHeight) {
	int nChannel = 1;

	return copyMask(pucMask, nWidth, nHeight, nChannel);
}
template <typename T>
bool CImage<T>::copyMask(unsigned char* &pucMask, int &nWidth, int &nHeight, int &nChannel) {
	// exception
	if(pucMask != NULL) {
		return false;
	}

	if(m_pucMask == NULL) {
		pucMask = NULL;
		nWidth = 0;
		nHeight = 0;
		nChannel = 0;

		return false;
	}
	else {
		int nSize = m_nWidth * m_nHeight * m_nMaskChannel;

		pucMask = new unsigned char[nSize];
		for(int i=0; i< nSize; i++) {
			pucMask[i] = m_pucMask[i];
		}

		nWidth = m_nWidth;
		nHeight = m_nHeight;
		nChannel = m_nMaskChannel;

		return true;
	}
}

template <typename T>
CSeries* CImage<T>::getSeries() {
	return m_series;
}
template<typename T>
void CImage<T>::setSeries(CSeries* pCiSeries) {
	m_series = pCiSeries;
}