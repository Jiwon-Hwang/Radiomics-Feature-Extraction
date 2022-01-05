#pragma once
#include <vector>
#include <list>
#include <iostream>
#include <io.h>	// window
#include <algorithm>

#include <opencv.hpp>
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/config/osconfig.h" 
#include "dcmtk/dcmdata/dctk.h"
#include "tiff.h"
#include "tiffio.h"
#include "nifti1_io.h"

#include "series.h"
#include "image.h"

#define LAZY_LOADING true

class CData
{
// variable
private:
	// Series(Image, Mask) 정보, 단, image와 Mask는 항상 같은 짝으로 유지되어야 한다
	std::vector<CSeries*> m_seriesList;		

	// Image와 매칭되지 못한 Mask가 들어있음
	std::vector<std::string> m_nonMatchedMaskPaths;

// function
public:
	CData();
	~CData();
	void init(void);
	void clear(void);
	void clearImages(int nSeriesIdx=-1);		// image class의 image 메모리만 소멸

	void readDir(std::string sPath);			// image 파일 혹은 폴더를 순회하면서 읽음
	void readImage(std::string sImagePath);		// image 파일을 읽음
	void matchingImageAndMask();				
	void sortingImageAndMask();

	// series
	CSeries* getSeries(int nSeriesIdx);
	int getSeriesCount(void);
	bool setSeries(CSeries* pCiSeries, int nSeriesIdx=-1);
	bool addSeries(CSeries* pCiSeries);

	// 아래 함수는 개발 편의성을 위해서 제공
	// nSliceIdx: image를 Series와 관계없이 순서대로 나열했을 때, image의 순서
	CImage<short>* getCImage(int nSliceIdx);
	CImage<short>* getCImage(int nSeriesIdx, int nImageIdx);
	short* getImage(int nSliceIdx);
	bool setImage(int nSliceIdx, CImage<short>* pCiImage);
	bool setImage(int nSliceIdx, short* psImage, int nWidth, int nHeight, int nChannel=1);
	bool setImage(int nSeriesIdx, int nImageIdx, CImage<short>* pCiImage);
	bool setImage(int nSeriesIdx, int nImageIdx, short* psImage, int nWidth, int nHeight, int nChannel=1);
	bool setImages(int nSeriesIdx, short** ppsImages, int nImageCnt, int nWidth, int nHeight, int nChannel=1);
	bool setImages(int nStartSliceIdx, int nEndSliceIdx, short** ppsImages, int nImageCnt, int nWidth, int nHeight, int nChannel=1);
	bool copyImage(int nSliceIdx, short* &psImage, int &nWidth, int &nHeight);
	bool copyImage(int nSliceIdx, short* &psImage, int &nWidth, int &nHeight, int &nChannel);
	bool copyImages(int nSeriesIdx, short** &ppsImages, int &nImageCnt, int &nWidth, int &nHeight);
	bool copyImages(int nSeriesIdx, short** &ppsImages, int &nImageCnt, int &nWidth, int &nHeight, int &nChannel);
	bool copyImages(int nStartSliceIdx, int nEndSliceIdx, short** &ppsImages, int &nImageCnt, int &nWidth, int &nHeight);
	bool copyImages(int nStartSliceIdx, int nEndSliceIdx, short** &ppsImages, int &nImageCnt, int &nWidth, int &nHeight, int &nChannel);

	CImage<unsigned char>* getCMask(int nSliceIdx);
	CImage<unsigned char>* getCMask(int nSeriesIdx, int nMaskIdx);
	unsigned char* getMask(int nSliceIdx);
	bool setMask(int nSliceIdx, CImage<unsigned char>* pCiImage);
	bool setMask(int nSliceIdx, unsigned char* pucMask, int nWidth, int nHeight, int nChannel=1);
	bool setMask(int nSeriesIdx, int nImageIdx, CImage<unsigned char>* pCiMask);
	bool setMask(int nSeriesIdx, int nImageIdx, unsigned char* pucMask, int nWidth, int nHeight, int nChannel=1);
	bool setMasks(int nSeriesIdx, unsigned char** ppucMasks, int nMaskCnt, int nWidth, int nHeight, int nChannel=1);
	bool setMasks(int nStartSliceIdx, int nEndSliceIdx, unsigned char** ppucMasks, int nMaskCnt, int nWidth, int nHeight, int nChannel=1);
	bool copyMask(int nSliceIdx, unsigned char* &pucMask, int &nWidth, int &nHeight);
	bool copyMask(int nSliceIdx, unsigned char* &pucMask, int &nWidth, int &nHeight, int &nChannel);
	bool copyMasks(int nSeriesIdx, unsigned char** &ppucMasks, int &nMaskCnt, int &nWidth, int &nHeight);
	bool copyMasks(int nSeriesIdx, unsigned char** &ppucMasks, int &nMaskCnt, int &nWidth, int &nHeight, int &nChannel);
	bool copyMasks(int nStartSliceIdx, int nEndSliceIdx, unsigned char** &ppucMasks, int &nMaskCnt, int &nWidth, int &nHeight);
	bool copyMasks(int nStartSliceIdx, int nEndSliceIdx, unsigned char** &ppucMasks, int &nMaskCnt, int &nWidth, int &nHeight, int &nChannel);

	int getWidth(int nSliceIdx);
	int getWidth(int nSeriesIdx, int nImageIdx);
	int getHeight(int nSliceIdx);
	int getHeight(int nSeriesIdx, int nImageIdx);
	int getChannel(int nSliceIdx);
	int getChannel(int nSeriesIdx, int nImageIdx);

	// image information
	std::string getImagePath(int nSliceIdx);
	std::string getImagePath(int nSeriesIdx, int nImageIdx);
	std::string getImageName(int nSliceIdx);
	std::string getImageName(int nSeriesIdx, int nImageIdx);
	std::string getImageExtension(int nSliceIdx);
	std::string getImageExtension(int nSeriesIdx, int nImageIdx);

	void getImagePosition(int nSliceIdx, float& fImagePositionX, float& fImagePositionY, float& fImagePositionZ);
	void getImagePosition(int nSeriesIdx, int nImageIdx, float& fImagePositionX, float& fImagePositionY, float& fImagePositionZ);
	void getImageOrientation(int nSliceIdx, float& fImageOrientationRowX, float& fImageOrientationRowY, float& fImageOrientationRowZ, float& fImageOrientationColX, float& fImageOrientationColY, float& fImageOrientationColZ);
	void getImageOrientation(int nSeriesIdx, int nImageIdx, float& fImageOrientationRowX, float& fImageOrientationRowY, float& fImageOrientationRowZ, float& fImageOrientationColX, float& fImageOrientationColY, float& fImageOrientationColZ);
	float getPixelSpacing(int nSliceIdx);
	float getPixelSpacing(int nSeriesIdx, int nImageIdx);
	void getPixelSpacing(int nSliceIdx, float& fPixelSpacingX, float& fPixelSpacingY);
	void getPixelSpacing(int nSeriesIdx, int nImageIdx, float& fPixelSpacingX, float& fPixelSpacingY);
	float getSliceThickness(int nSliceIdx);
	float getSliceThickness(int nSeriesIdx, int nImageIdx);
	void getWLWW(int nSliceIdx, int& nWL, int& nWW);
	void getWLWW(int nSeriesIdx, int nImageIdx, int& nWL, int& nWW);
	int getInstanceNumber(int nSliceIdx);
	int getInstanceNumber(int nSeriesIdx, int nImageIdx);
	
private:
	// mask file check rule
	bool isMask_namingRule(std::string sPath);
	template<typename T>
	bool isMask_imageRule(CImage<T>* pCiImage);

	// patient rule
	template<typename T>
	void setPatientName(CSeries* pCiSeries, std::vector<CImage<T>*> pCiImages);
	template<typename T>
	void setStudyName(CSeries* pCiSeries, std::vector<CImage<T>*> pCiImages);
	template<typename T>
	void setSeriesName(CSeries* pCiSeries, std::vector<CImage<T>*> pCiImages);

	// matching, compare rule
	template<typename T1, typename T2>
	bool isSameImage(CImage<T1>* pCiImage1, CImage<T2>* pCiImage2);
	bool isSameSeries(CSeries* pCiSeries1, CSeries* pCiSeries2);

	// sorting rule
	template <typename T>
	static bool sorting_fileName(CImage<T>* &pCiImage1, CImage<T>* &pCiImage2);
	template <typename T>
	static bool sorting_imagePosition(CImage<T>* &pCiImage1, CImage<T>* &pCiImage2);
	
	// load
	template <typename T>
	bool loadImage(std::string sImagePath, CSeries* &pCiSeries, std::vector<CImage<T>*> &ciImages);
	template <typename T>
	bool loadImage(CImage<T>* &pCiImage, bool isMask);
	bool loadDICOM_dcmtk(const char* pcFilePath, DcmDataset* &dataSet, std::vector<short*> &images, bool bReadHeaderOnly=false);
	bool loadImage_opencv(const char* pcFilePath, bool isColor, unsigned char* &pucImage, int &nWidth, int &nHeight, int &nChannel, bool bReadHeaderOnly=false);
	bool loadNII_libnii(const char* pcImagePath, nifti_1_header &nhdr, std::vector<short*> &images, bool bReadHeaderOnly=false);
	bool loadTIFF_libtiff(const char* pcImagePath, std::vector<unsigned char*> &images, bool bReadImageOnly=false);

	// file
	bool isFile(std::string sPath);
	bool isFile(_finddata_t fd);
	void searchingDir(std::string path, std::list<std::string>& sFilePaths);
	std::string getFileExtension(std::string sFilePath);
	std::string getFileName(std::string sFilePath);
	std::vector<std::string> splitPath(std::string sPath);
	std::string replaceAll(std::string &str, const std::string& from, const std::string& to);
};