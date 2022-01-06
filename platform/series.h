#pragma once
#include <string>
#include <vector>
#include "image.h"

class CSeries {
// variable
public:
	std::string m_sPatientID;			// dcm header
	std::string m_sPatientName;			// folder

	std::string m_sStudyInstanceUID;
	std::string m_sStudyDescription;	// dcm header
	std::string m_sStudyDate;
	std::string m_sStudyName;			// folder

	std::string m_sSeriesInstanceUID;
	std::string m_sFrameOfReferenceUID;
	std::string m_sSeriesDate;
	std::string m_sSeriesDescription;	// dcm header
	std::string m_sSeriesName;			// folder
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