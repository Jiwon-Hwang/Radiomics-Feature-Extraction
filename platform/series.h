#pragma once
#include <string>
#include <vector>
#include "image.h"

class CSeries {
// version 1.1
// variable
public:
	std::string m_sSeriesInstanceUID;
	std::string m_sFrameOfReferenceUID;
	std::string m_sPatientID;
	std::string m_sPatientName;
	std::string m_sSeriesDate;
	std::string m_sDescription;
	std::string m_sModality;

	std::vector<CImage<short>*> m_images;				// image
	std::vector<CImage<unsigned char>*> m_masks;		// mask

// function
public:
	CSeries();
	~CSeries();
	void init();
	void clear();
	CSeries* copy();

	void addImage(CImage<short>* pCiImage);
	void setImage(int nIdx, CImage<short>* pCiImage);
	CImage<short>* getImage(int nIdx);
	int getImageCount(void);
	
	void addMask(CImage<unsigned char>* pCiMask);
	void setMask(int nIdx, CImage<unsigned char>* pCiMask);
	CImage<unsigned char>* getMask(int nIdx);
	int getMaskCount(void);
};