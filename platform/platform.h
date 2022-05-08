#pragma once
#ifndef platform_H
#define platform_H

#include <QtWidgets/QMainWindow>
#include <qtreewidget.h>
#include <QMouseEvent>
#include <qfiledialog.h>
#include <qtoolbar.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtextcodec.h>
#include <QMimeData>
#include <qprogressbar.h>
#include <direct.h>
#include <vector>
#include <string>
#include <QDirIterator>
#include <functional>
#include <algorithm>
#include <numeric>
#include <QSettings>
#include <QDebug>
#include <QDialog>

#define _CRT_SECURE_NO_WARNINGS
#include <ctime>

// for M_PI, isnan()
#define _USE_MATH_DEFINES
#include <math.h>

#include <opencv2/opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>


#include "ui_platform.h"
#include "ui_popup_Histogram.h"
#include "popup_Histogram.h"
#include "ui_popup_GLCM.h"
#include "popup_GLCM.h"
/*
#include "ui_popup_Intensity.h"
#include "popup_Intensity.h"
#include "ui_popup_Morph.h"
#include "popup_Morph.h"
#include "ui_popup_GLRLM.h"
#include "popup_GLRLM.h"
*/

#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/config/osconfig.h" 
#include "dcmtk/dcmdata/dctk.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>


#include "imageView.h"
#include "data.h"
#include "intensityHistogram.h"
#include "glcm.h"


#define MAX_FILE_LENGTH 64
#define SAFE_DELETE_ARRAY(p)	{ if(p) delete[](p); p = NULL;}
#define SAFE_DELETE_VOLUME(p, depth)	{ if(p) {for (int i=0;i<depth;i++)	if(p[i]) delete[](p[i]); } delete[] p; p=NULL;};

#define FILTER_NONE 0
#define FILTER_GAUSSIAN 1 
#define FILTER_LAPLACIAN 2


class CPlatform : public QMainWindow
{
	Q_OBJECT

// 생성자, 소멸자, 초기화
public:
	CPlatform(QWidget *parent = 0);
	~CPlatform();
	void init();
	void clear();

	void readImage(QStringList list);

public:
	CImageView* m_ciImage;				// image
	CData m_ciData;						// data
	int m_nActivatedFrameIdx;			// 현재 화면에 가시화된 Frame 번호
	

public:	
	enum FAMILY { E_INTENSEHISTO, E_LOCALINTENSE, E_MORPHOLOGY, E_GLCM, E_GLRLM, FAMILY_COUNT };

	// Feature Family objects
	IntensityHistogram intenseHisto;
	IntensityHistogram localIntense;
	IntensityHistogram morphology;
	GLCM glcm;
	IntensityHistogram glrlm;
	/*
	LocalIntensity localIntense;
	Morphological morphology;
	GLRLM glrlm;
	*/

	// pop-up objects
	popup_Histogram *ppopup_Histogram;
	popup_Histogram *ppopup_Intensity;
	popup_Histogram *ppopup_Morph;
	popup_GLCM *ppopup_GLCM;
	popup_Histogram *ppopup_GLRLM;
	/*
	popup_Intensity *ppopup_Intensity;
	popup_Morph *ppopup_Morph;
	popup_GLRLM *ppopup_GLRLM;
	*/
		

// QT layout, action 변수
public:
	// layout
	Ui::platformClass ui;
	QToolBar* fileToolBar;
	QMenu* fileMenu;
	QAction* newFileAction;
	QProgressBar* progressBar;

	void createPopup();
	void createProgressBar();
	void setSignalSlot();
	void setProgressBarValue(int nCurrentIdx, int nMaximumIdx);
	
	// load and save GUI state //
	void loadSettings();
	void saveSettings();
	void initIsActivatedFamily(int idx);

	// feature extraction //
	void featureExtraction(short* psImage, unsigned char* pucMask, int nHeight, int nWidth);
	void averageAllSlices();

	short calcLocalIntensityPeak(short* pusImage, unsigned char* pucMask, int nHeight, int nWidth); // class 함수로 넣기, m_ciData.getPixelSpacing() call
					
	// write CSV //
	void presetCSVFile(string csvName);
	void writeCSVFile(int seriesIdx, string csvName);
	void writeCSVFeatureValue(string csvName);
	void writeCSVCaseName(int seriesIdx, string csvName);

	// clear all vector //
	void clearAll(int seriesIdx);

protected:
	void resizeEvent(QResizeEvent* event);
	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent * event);
	void keyPressEvent(QKeyEvent* event);
	void keyReleaseEvent(QKeyEvent* event);

public slots:
	// load + fileDirectory //
	void addFileDirectoryItem(int seriesIdx);	// tree widget
	void showImage(int nFrameIdx);
	void showImage(QTreeWidgetItem* item, int column);

	// scroll //
	void scrollChangeImage(int nValue);

	// check and set all btn's state //
	void setFilterMode();
	void setCheckedFamilyState();
	bool checkReadyToRun();
	void selectAll(bool checked);
	void checkFeatureBoxState();

	// pop-up //
	void showPopUp(QObject* sender);

	void run();
};


class BorderlessMainWindow: public QMainWindow
{
	Q_OBJECT
public:
	explicit BorderlessMainWindow(QWidget *parent = 0);
	~BorderlessMainWindow() {}
protected:
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent* event);
private slots:
	void slot_minimized();
	void slot_restored();
	void slot_maximized();
	void slot_closed();
private:
	CPlatform *mMainWindow;
	QWidget *mTitlebarWidget;
	QFrame *mSeparateLine;
	QLabel *mWindowIcon;
	QLabel *mWindowTitle;
	QPushButton *mMinimizeButton;
	QPushButton *mRestoreButton;
	QPushButton *mMaximizeButton;
	QPushButton *mCloseButton;
	QPoint mLastMousePosition;

	QVBoxLayout *verticalLayout;
	QHBoxLayout *horizontalLayout;
	QWidget *centralWidget;

	bool mMoving;
	bool mMaximized;
};

#endif // platform_H
