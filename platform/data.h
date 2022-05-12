#pragma once

// v 1.46 (20220511)

// config
#define DEBUG 0							// 0: false, 1: true (VLD)
#define THREAD 1						// 0: false, 1: true (Thread on/off)
#define QT 1							// 0: false, 1: true (QT에서 사용하는 경우)
#define PROGRESS_BAR 1					// 0: hide, 1: show (console)

/////////////////////////////////////////////////////////////////////////////

// standard lib
#include <vector>
#include <iostream>
#include <io.h>					// window
#include <algorithm>
#include <functional>
#include <direct.h>				// mkdir
#include <Windows.h>


// thread lib
#if THREAD
#include <concurrent_vector.h>	// thread safe vector
#include <thread>
#include <mutex>
#include <future>
#endif

// qt lib (https://doc.qt.io/qt-5/qthread.html)
// (http://greenday96.blogspot.com/2020/11/qt-movetothread-qthread-wait.html)
// https://evileg.com/en/post/152/
#if QT
#include <QObject>
#include <qthread.h>
#endif

// 3rdparty lib
#include <opencv.hpp>
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/config/osconfig.h" 
#include "dcmtk/dcmdata/dctk.h"
#include "tiff.h"
#include "tiffio.h"
#include "nifti1_io.h"

//
#include "image.h"
#include "series.h"

//
#if DEBUG
#define DEBUG_LOG(x) std::cerr << x
#define DEBUG_LOG_DETAIL(function, line, x) std::cerr << function << "[" << line << "]" << std::endl << x
#include <vld.h>
#else
#define DEBUG_LOG(x)
#define DEBUG_LOG_DETAIL(function, line, x)
#endif

/////////////////////////////////////////////////////////////////////////////

#if QT
class CData: public QObject
#else
class CData
#endif
{
#if QT
	Q_OBJECT
public:
	QThread* m_pThread;
	std::vector<std::string> readFileList;
#endif

public:
	enum READIMAGE_CLEAR_OPTION {KEEP_PREV_DATAS, CLEAR_PREV_DATAS_CONDITIONAL, CLEAR_PREV_DATAS_FORCE};
	enum SAVE_TYPE {SAVE_IMAGE_ONLY, SAVE_MASK_ONLY, SAVE_OVERLAY};
	enum SAVE_OPTION {SAVE_DEFAULT, SAVE_IMAGE_WINDOW, SAVE_MASK_BINARY};
	enum LOAD_STATUS {
		SUCCESS=1, UNREAD=0, UNREADBLE_FILE=-1, UNREADBLE_FILE_FORMAT=-2, INPUT_PARAM_ERR=-3, DUPLICATE_FILE=-4, 
		DCM_FILE_FORMAT_ERR=-5, DCM_IMG_ERR=-6, EXCEPT_FILE=-7,
		BAD_ALLOC=-999
	};

// variable
public:
	bool m_isLoading;

private:
	// preference
	std::string m_sLogPath;
	std::ofstream m_sLog;
	std::vector<std::string> m_sReadableExtensions;
	std::vector<std::string> m_sMaskKeywords;
	std::vector<std::string> m_sExceptKeywords;

#if THREAD
	Concurrency::concurrent_vector<CSeries*> m_seriesList;
#else
	std::vector<CSeries*> m_seriesList;
#endif

	// Scan용 (Image, Image 정보가 mapping 되어 있지 않은 상태)
	struct ScanContainer {
		int isLoad;
		bool isMask;
		std::string sFilePath;
		std::string sFileName;
		std::string sFileExtension;
	};
	std::vector<ScanContainer*> m_scan;

// function

#if QT
public:
	explicit CData(QObject* parent = nullptr);
	virtual ~CData();
	void setQThread(QThread* pThread);
signals:
	void signalReadImage(std::vector<std::string> sPaths, bool bReadRecursive, bool bScanOnly, int nClearOption, std::function<void(int, int)>* pCallback);
	void signalDataScanFinish();
	void signalDataProgress(int, int);
public slots:
	void slotReadImage(void);
	void slotReadImage(std::vector<std::string> sPaths, bool bReadRecursive, bool bScanOnly, int nClearOption, std::function<void(int, int)>* pCallback);

#else
public:
	CData();
	~CData();
#endif

public:
	void init(void);
	void clear(void);
	void clearImages(int nSeriesIdx=-1);		// image class의 image 메모리만 소멸

	// operator
	friend std::ostream& operator<< (std::ostream& stream, const CData& obj);

	// readImage
	void readImage(std::string sPath, bool bReadRecursive=true, bool bScanOnly=true, int nClearOption=KEEP_PREV_DATAS, std::function<void(int, int)>* pCallback=NULL);
	void readImage(std::vector<std::string> sPaths, bool bReadRecursive=true, bool bScanOnly=true, int nClearOption=KEEP_PREV_DATAS, std::function<void(int, int)>* pCallback=NULL);

private:
	void scanImage(ScanContainer* psc, int &nFileCount, int nTotal, std::function<void(int, int)>* pCallback=NULL);

	// load
public:
	int loadImages(int nSliceIdx);
private:
	template <typename T>
	int loadImage(std::string sImagePath, CSeries* &pCiSeries, std::vector<CImage<T>*> &pCiImages, bool bReadHeaderOnly=false);
	template <typename T>
	int loadImage_lazyLoading(CImage<T>* &pCiImage, bool isMask);
	int loadImage_lazyLoading_image_thread(CSeries* pCiSeries, int nImageIdx, int &nFileCount, std::function<void(int, int)>* pCallback=NULL);
	int loadImage_lazyLoading_mask_thread(CSeries* pCiSeries, int nImageIdx, int &nFileCount, std::function<void(int, int)>* pCallback=NULL);
	int loadImages_lazyLoading_thread(CSeries* pCiSeries, bool isMask, std::function<void(int, int)>* pCallback=NULL);
public:
	static int loadDICOM_dcmtk(const char* pcFilePath, DcmDataset* &dataSet, std::vector<short*> &images, bool bReadHeaderOnly=false);
	static int loadImage_opencv(const char* pcFilePath, bool isColor, unsigned char* &pucImage, int &nWidth, int &nHeight, int &nChannel, bool bReadHeaderOnly=false);
	static int loadNII_libnii(const char* pcImagePath, nifti_1_header &nhdr, std::vector<short*> &images, bool bReadHeaderOnly=false);
	static int loadTIFF_libtiff(const char* pcImagePath, TIFF* &pTiff, std::vector<unsigned char*> &images, bool bReadHeaderOnly=false);
private:
	void loadStart();
	void loadFinish();

	// save
public:
	bool saveImage(int nSliceIdx, const char* pcImagePath, int nSaveType=SAVE_IMAGE_ONLY, int nSaveOption=SAVE_DEFAULT);
	bool saveImage(int nSeriesIdx, int nImageIdx, const char* pcImagePath, int nSaveType=SAVE_IMAGE_ONLY, int nSaveOption=SAVE_DEFAULT);
	bool saveImages(int nSeriesIdx, const char* pcSaveDir, const char* pcExtension="png", int nSaveType=SAVE_IMAGE_ONLY, int nSaveOption=SAVE_DEFAULT);
private:
	bool saveImage_opencv(int nSeriesIdx, int nImageIdx, const char* pcImagePath, int nSaveType=SAVE_IMAGE_ONLY, int nSaveOption=SAVE_DEFAULT);
	bool saveImage_libtiff(int nSeriesIdx, int nImageIdx, const char* pcImagePath, int nSaveType=SAVE_IMAGE_ONLY, int nSaveOption=SAVE_DEFAULT);	// nImageIdx이 -1이면 multiple page
	bool saveImage_dcmtk(int nSeriesIdx, int nImageIdx, const char* pcImagePath, int nSaveType=SAVE_IMAGE_ONLY, int nSaveOption=SAVE_DEFAULT);

public:
	// series
	CSeries* getSeries(int nSeriesIdx);
	int getSeriesCount(void);
	bool setSeries(CSeries* pCiSeries, int nSeriesIdx=-1);
	bool addSeries(CSeries* pCiSeries);

	// log file
	void setLogPath(std::string sPath);
	void checkIsEmptyLog(std::string sFinalLogPath);

	// 아래 함수는 개발 편의성을 위해서 제공
	// nSliceIdx: image를 Series와 관계없이 순서대로 나열했을 때, image의 순서
	int convertToSliceIdx(int nSeriesIdx, int nImageIdx);

	CImage<short>* getCImage(int nSliceIdx);
	CImage<short>* getCImage(int nSeriesIdx, int nImageIdx);
	short* getImage(int nSliceIdx);
	short* getImage(int nSeriesIdx, int nImageIdx);
	bool setImage(int nSliceIdx, short* psImage, int nWidth, int nHeight, int nChannel=1);
	bool setImage(int nSeriesIdx, int nImageIdx, short* psImage, int nWidth, int nHeight, int nChannel=1);
	bool setImages(int nSeriesIdx, short** ppsImages, int nImageCnt, int nWidth, int nHeight, int nChannel=1);
	bool copyImage(int nSliceIdx, short* &psImage, int &nWidth, int &nHeight);
	bool copyImage(int nSliceIdx, short* &psImage, int &nWidth, int &nHeight, int &nChannel);
	bool copyImage(int nSeriesIdx, int nImageIdx, short* &psImage, int &nWidth, int &nHeight);
	bool copyImage(int nSeriesIdx, int nImageIdx, short* &psImage, int &nWidth, int &nHeight, int &nChannel);
	bool copyImages(int nSeriesIdx, short** &ppsImages, int &nImageCnt, int &nWidth, int &nHeight);
	bool copyImages(int nSeriesIdx, short** &ppsImages, int &nImageCnt, int &nWidth, int &nHeight, int &nChannel);

	unsigned char* getMask(int nSliceIdx);
	unsigned char* getMask(int nSeriesIdx, int nMaskIdx);
	bool setMask(int nSliceIdx, unsigned char* pucMask, int nWidth, int nHeight, int nChannel=1);
	bool setMask(int nSeriesIdx, int nImageIdx, unsigned char* pucMask, int nWidth, int nHeight, int nChannel=1);
	bool setMasks(int nSeriesIdx, unsigned char** ppucMasks, int nMaskCnt, int nWidth, int nHeight, int nChannel=1);
	bool copyMask(int nSliceIdx, unsigned char* &pucMask, int &nWidth, int &nHeight);
	bool copyMask(int nSliceIdx, unsigned char* &pucMask, int &nWidth, int &nHeight, int &nChannel);
	bool copyMask(int nSeriesIdx, int nImageIdx, unsigned char* &pucMask, int &nWidth, int &nHeight);
	bool copyMask(int nSeriesIdx, int nImageIdx, unsigned char* &pucMask, int &nWidth, int &nHeight, int &nChannel);
	bool copyMasks(int nSeriesIdx, unsigned char** &ppucMasks, int &nMaskCnt, int &nWidth, int &nHeight);
	bool copyMasks(int nSeriesIdx, unsigned char** &ppucMasks, int &nMaskCnt, int &nWidth, int &nHeight, int &nChannel);

	std::string getImagePath(int nSliceIdx);
	std::string getImagePath(int nSeriesIdx, int nImageIdx);
	std::string getImageName(int nSliceIdx);
	std::string getImageName(int nSeriesIdx, int nImageIdx);
	std::string getImageExtension(int nSliceIdx);
	std::string getImageExtension(int nSeriesIdx, int nImageIdx);

	std::string getMaskPath(int nSliceIdx);
	std::string getMaskPath(int nSeriesIdx, int nImageIdx);
	std::string getMaskImageName(int nSliceIdx);
	std::string getMaskImageName(int nSeriesIdx, int nImageIdx);
	std::string getMaskExtension(int nSliceIdx);
	std::string getMaskExtension(int nSeriesIdx, int nImageIdx);

	int getWidth(int nSliceIdx);
	int getWidth(int nSeriesIdx, int nImageIdx);
	int getHeight(int nSliceIdx);
	int getHeight(int nSeriesIdx, int nImageIdx);
	int getChannel(int nSliceIdx);
	int getChannel(int nSeriesIdx, int nImageIdx);
	int getMaskChannel(int nSliceIdx);
	int getMaskChannel(int nSeriesIdx, int nImageIdx);

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

//////////////////////////////////////////////////////////////////////////////////////////
	// file
public:
	static int find(std::string string, std::string value, int n=1);
	static int rfind(std::string string, std::string value, int n=1);
	static std::vector<std::string> splitPath(std::string sPath);
	static std::vector<std::string> splitString(std::string string, char separator);
	static std::string replaceAll(std::string &str, const std::string& from, const std::string& to);
	static void makeDir(std::string sPath);
	static std::string getFileExtension(std::string sFilePath);
	static std::string getFileName(std::string sFilePath);

private:
	bool isFile(std::string sPath);
	bool isFile(_finddata_t fd);
	void searchingDir(std::string path, std::vector<std::string>& sFilePaths);
	
	// rescale
public:
	template<typename T>
	static bool convertDefaultImage (T* &pImage, int nWidth, int nHeight);
	template<typename T>
	static bool convertWindowImage (T* &pImage, int nWidth, int nHeight, int nWL, int nWW);

	// progressbar
private: 
	void progressBar(int step, int total);

//////////////////////////////////////////////////////////////////////////////////////////
	// matching, compare rule
	template<typename T1, typename T2>
	bool isSameImage(CImage<T1>* pCiImage1, CImage<T2>* pCiImage2);
	bool isSameSeries(CSeries* pCiSeries1, CSeries* pCiSeries2);
	bool isMask_namingRule(std::string sPath);
	bool isExcludeFile(std::string sPath);

	// sorting rule
	template <typename T>
	static bool sorting(CImage<T>* &pCiImage1, CImage<T>* &pCiImage2);
	template <typename T>
	static bool sorting_fileName(CImage<T>* &pCiImage1, CImage<T>* &pCiImage2);

	// series, image 내용 변경 rule
	bool modifySeries(CSeries* pCiSeries);

	std::string renameImage(std::string sFileName);
	std::string renameSeries(std::string sSeriesName);

	// 저장 경로 rule
	std::string convertImageSavePath(int nSeriesIdx, int nImageIdx, const char* pcSaveDir, const char* pcExtension, int nSaveType, int nSaveOption);
};