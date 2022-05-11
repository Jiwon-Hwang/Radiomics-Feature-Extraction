#include "series.h"

CSeries::CSeries() {
	init();
}
CSeries::~CSeries() {
	clear();
}
void CSeries::init() {
	m_sPatientID = "";				// (0010,0020)
	m_sPatientName = "";			// (0010,0010)

	m_sStudyInstanceUID = "";		// (0020,000D)
	m_sStudyDescription = "";		// (0008,1030)
	m_sStudyDate = "";				// (0008,0020)
	m_sStudyName = "";				// none

	m_sSeriesInstanceUID = "";		// (0020,000E)
	m_sFrameOfReferenceUID = "";	// (0020,0052)
	m_sSeriesDate = "";				// (0008,0021)
	m_sSeriesDescription = "";		// (0008,103E)
	m_sSeriesName = "";				// none
	m_sModality = "";				// (0008,0060)

	m_sSeriesPath = "";

	m_images.clear();
}
void CSeries::clear() {
	for(int i=0, ni=m_images.size(); i<ni; i++) {
		SAFE_DELETE(m_images[i]);
	}

	m_sPatientID = "";				// (0010,0020)
	m_sPatientName = "";			// (0010,0010)

	m_sStudyInstanceUID = "";		// (0020,000D)
	m_sStudyDescription = "";		// (0008,1030)
	m_sStudyDate = "";				// (0008,0020)
	m_sStudyName = "";				// none

	m_sSeriesInstanceUID = "";		// (0020,000E)
	m_sFrameOfReferenceUID = "";	// (0020,0052)
	m_sSeriesDate = "";				// (0008,0021)
	m_sSeriesDescription = "";		// (0008,103E)
	m_sSeriesName = "";				// none
	m_sModality = "";				// (0008,0060)

	m_sSeriesPath = "";

	m_images.clear();
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

	pSeries->m_sSeriesPath = m_sSeriesPath;

	int nImageCount = m_images.size();
	pSeries->m_images.reserve(nImageCount);
	for(int i=0, ni=nImageCount; i<ni; i++) {
		pSeries->m_images[i] = m_images[i]->copy();
	}

	return pSeries;
}

std::ostream& operator<<(std::ostream& stream, const CSeries& obj) {
	int nImageCount = obj.m_images.size();
	int nImageLoad = 0;
	int nMaskCount = 0;
	int nMaskLoad = 0;

	for(int i=0; i< nImageCount; i++) {
		if(obj.m_images[i]->getImage()) {
			nImageLoad++;
		}
		if(obj.m_images[i]->getMask()) {
			nMaskLoad++;
		}
		if(obj.m_images[i]->getMaskPath() != "") {
			nMaskCount++;
		}
	}

	stream << "---------------------------------------" << "\n";
	stream << obj.m_sPatientName << " | " << obj.m_sStudyName << " | " << obj.m_sSeriesName << 
		" (Image: " << nImageCount << "(" << nImageLoad << "), " << "Mask: " << nMaskCount << "(" << nMaskLoad << "))\n";

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