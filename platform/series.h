#pragma once
#include <string>
#include <vector>

#include "image.h"

class CSeries {
// variable
public:
	std::string m_sPatientID;				// (0010,0020)
	std::string m_sPatientName;				// (0010,0010) (규칙에 의해 변경될 수 있음)

	std::string m_sStudyInstanceUID;		// (0020,000D)
	std::string m_sStudyDescription;		// (0008,1030)
	std::string m_sStudyDate;				// (0008,0020)
	std::string m_sStudyName;				// custom tag (규칙에 의해 결정됨)

	std::string m_sSeriesInstanceUID;		// (0020,000E)
	std::string m_sFrameOfReferenceUID;		// (0020,0052)
	std::string m_sSeriesDate;				// (0008,0021)
	std::string m_sSeriesDescription;		// (0008,103E)
	std::string m_sSeriesName;				// custom tag (규칙에 의해 결정됨)
	std::string m_sModality;				// (0008,0060)

	std::string m_sSeriesPath;

	std::vector<CImage<short>*> m_images;	// image

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