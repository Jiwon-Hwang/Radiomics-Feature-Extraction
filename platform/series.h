#pragma once
#include <string>
#include <vector>

#include "image.h"

class CSeries {
// variable
public:
	std::string m_sPatientID;
	std::string m_sPatientName;

	std::string m_sStudyInstanceUID;
	std::string m_sStudyDescription;
	std::string m_sStudyDate;
	std::string m_sStudyName;

	std::string m_sSeriesInstanceUID;
	std::string m_sFrameOfReferenceUID;
	std::string m_sSeriesDate;
	std::string m_sSeriesDescription;
	std::string m_sSeriesName;
	std::string m_sModality;

	std::string m_sSeriesPath;

	std::vector<CImage<short>*> m_images;				// image

// function
public:
	CSeries();
	~CSeries();
	void init();
	void clear();
	CSeries* copy();

	// operator
	friend std::ostream& operator<< (std::ostream& stream, const CSeries& obj);

	void addImage(CImage<short>* pCiImage);
	void setImage(int nIdx, CImage<short>* pCiImage);
	CImage<short>* getImage(int nIdx);
	int getImageCount(void);
};