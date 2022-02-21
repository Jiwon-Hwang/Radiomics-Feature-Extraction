#include "series.h"

CSeries::CSeries() {
	init();
}
CSeries::~CSeries() {
	clear();
}
void CSeries::init() {
	m_sPatientID = "";
	m_sPatientName = "";

	m_sStudyInstanceUID = "";
	m_sStudyDescription = "";
	m_sStudyDate = "";
	m_sStudyName = "";

	m_sSeriesInstanceUID = "";
	m_sFrameOfReferenceUID = "";
	m_sSeriesDate = "";
	m_sSeriesDescription = "";
	m_sSeriesName = "";
	m_sModality = "";

	m_images.clear();
	m_masks.clear();
}
void CSeries::clear() {
	for(int i=0, ni=m_images.size(); i<ni; i++) {
		SAFE_DELETE(m_images[i]);
	}
	for(int i=0, ni=m_masks.size(); i<ni; i++) {
		SAFE_DELETE(m_masks[i]);
	}

	m_images.clear();
	m_masks.clear();
}
CSeries* CSeries::copy() {
	CSeries* pSeries = new CSeries();

	pSeries->m_sPatientID = m_sPatientID;
	pSeries->m_sPatientName = m_sPatientName;

	pSeries->m_sStudyInstanceUID = m_sStudyInstanceUID;
	pSeries->m_sStudyDescription = m_sStudyDescription;
	pSeries->m_sStudyDate = m_sStudyDate;
	pSeries->m_sStudyName = m_sStudyName;

	pSeries->m_sSeriesInstanceUID = m_sSeriesInstanceUID;
	pSeries->m_sFrameOfReferenceUID = m_sFrameOfReferenceUID;
	pSeries->m_sSeriesDate = m_sSeriesDate;
	pSeries->m_sSeriesDescription = m_sSeriesDescription;
	pSeries->m_sSeriesName = m_sSeriesName;
	pSeries->m_sModality = m_sModality;

	int nImageCount = m_images.size();
	pSeries->m_images.reserve(nImageCount);
	for(int i=0, ni=nImageCount; i<ni; i++) {
		pSeries->m_images[i] = m_images[i]->copy();
	}

	int nMaskCount = m_masks.size();
	pSeries->m_masks.reserve(nMaskCount);
	for(int i=0, ni=nMaskCount; i<ni; i++) {
		pSeries->m_masks[i] = m_masks[i]->copy();
	}

	return pSeries;
}

std::ostream& operator<<(std::ostream& stream, const CSeries& obj) {
	int nImageCount = obj.m_images.size();
	int nMaskCount = obj.m_masks.size();

	stream << "---------------------------------------" << "\n";
	stream << obj.m_sPatientName << " | " << obj.m_sSeriesName << 
		" (Image: " << nImageCount << ", " << "Mask: " << nMaskCount << ")\n";

	return stream;
}

void CSeries::addImage(CImage<short>* pCiImage) {
	pCiImage->setSeries(this);
	m_images.push_back(pCiImage);
}
void CSeries::setImage(int nIdx, CImage<short>* image) {
	// exception
	if(m_images[nIdx] != NULL) {
		SAFE_DELETE_ARRAY(m_images[nIdx]);
	}

	image->setSeries(this);
	m_images[nIdx] = image;
}
CImage<short>* CSeries::getImage(int nIdx) {
	if(m_images[nIdx] != NULL) {
		return m_images[nIdx];
	}
	return NULL;
}
int CSeries::getImageCount(void) {
	return m_images.size();
}

void CSeries::addMask(CImage<unsigned char>* pCiMask) {
	pCiMask->setSeries(this);
	m_masks.push_back(pCiMask);
}
void CSeries::setMask(int nIdx, CImage<unsigned char>* mask) {
	if(m_masks[nIdx]->m_image != NULL) {
		SAFE_DELETE(m_masks[nIdx]);
	}

	mask->setSeries(this);
	m_masks[nIdx] = mask;
}
CImage<unsigned char>* CSeries::getMask(int nIdx) {
	return m_masks[nIdx];
}
int CSeries::getMaskCount(void) {
	return m_masks.size();
}