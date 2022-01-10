#include "platform.h"

using namespace std;
using namespace cv;

int FILTER_MODE; 

CPlatform::CPlatform(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);			// ui setting 
	setMouseTracking(true);
	setAcceptDrops(true);

	init();
	createProgressBar();

	setSignalSlot();
}
CPlatform::~CPlatform()
{
	clear();
}

// Borderless Main Window
BorderlessMainWindow::BorderlessMainWindow(QWidget *parent) : QMainWindow(parent, Qt::CustomizeWindowHint ) {
	setObjectName("borderlessMainWindow");
	setWindowFlags(Qt::FramelessWindowHint| Qt::WindowSystemMenuHint);
	this->resize(1024, 680);

	mMainWindow = new CPlatform(this);
	setWindowTitle(mMainWindow->windowTitle());

	mMainWindow->statusBar()->setSizeGripEnabled(true);
	mMainWindow->setWindowFlags(Qt::FramelessWindowHint);

	verticalLayout = new QVBoxLayout();
	verticalLayout->setSpacing(0);
	verticalLayout->setMargin(0);

	horizontalLayout = new QHBoxLayout();
	horizontalLayout->setSpacing(0);
	horizontalLayout->setMargin(0);

	mTitlebarWidget = new QWidget(this);
	mTitlebarWidget->setObjectName("titlebarWidget");
	mTitlebarWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	mTitlebarWidget->setLayout(horizontalLayout);

	mMinimizeButton = new QPushButton(mTitlebarWidget);
	mMinimizeButton->setObjectName("minimizeButton");
	connect(mMinimizeButton, SIGNAL(clicked()), this, SLOT(slot_minimized()));

	mRestoreButton = new QPushButton(mTitlebarWidget);
	mRestoreButton->setObjectName("restoreButton");
	mRestoreButton->setVisible(false);
	connect(mRestoreButton, SIGNAL(clicked()), this, SLOT(slot_restored()));

	mMaximizeButton = new QPushButton(mTitlebarWidget);
	mMaximizeButton->setObjectName("maximizeButton");
	connect(mMaximizeButton, SIGNAL(clicked()), this, SLOT(slot_maximized()));

	mCloseButton = new QPushButton(mTitlebarWidget);
	mCloseButton->setObjectName("closeButton");
	connect(mCloseButton, SIGNAL(clicked()), this, SLOT(slot_closed()));

	mWindowIcon = new QLabel(mTitlebarWidget);
	mWindowIcon->setObjectName("windowIcon");
	mWindowIcon->setPixmap(QPixmap("Resources/Deep Imaging Laboratory2.bmp"));
	mWindowIcon->setStyleSheet("padding: 5px;");

	mWindowTitle = new QLabel(mTitlebarWidget);
	mWindowTitle->setObjectName("windowTitle");
	//mWindowTitle->setText(windowTitle());
	QString str = QString::fromLocal8Bit("Radiomics Feature Extract");
	mWindowTitle->setText(str);
	mWindowTitle->setStyleSheet("padding-left: 3px; font-size: 14px; font-weight: bold; color: #ffffff");

	mSeparateLine = new QFrame();
	mSeparateLine->setFrameShape(QFrame::HLine);
	mSeparateLine->setStyleSheet("border-top: 1px solid #666666;");

	horizontalLayout->addWidget(mWindowIcon);
	horizontalLayout->addWidget(mWindowTitle);
	horizontalLayout->addStretch(1);
	horizontalLayout->addWidget(mMinimizeButton);
	horizontalLayout->addWidget(mRestoreButton);
	horizontalLayout->addWidget(mMaximizeButton);
	horizontalLayout->addWidget(mCloseButton);

	verticalLayout->addWidget(mTitlebarWidget);
	verticalLayout->addWidget(mSeparateLine);
	verticalLayout->addWidget(mMainWindow);

	centralWidget = new QWidget(this);
	centralWidget->setObjectName("centralWidget");
	centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	centralWidget->setLayout(verticalLayout);

	setCentralWidget(centralWidget);
}
void BorderlessMainWindow::mousePressEvent(QMouseEvent* event) {
	if (!mTitlebarWidget->underMouse() && !mWindowTitle->underMouse())
		return;

	if(event->button() == Qt::LeftButton) {
		mMoving = true;
		mLastMousePosition = event->pos();
	}
}
void BorderlessMainWindow::mouseMoveEvent(QMouseEvent* event) {
	if (!mTitlebarWidget->underMouse() && !mWindowTitle->underMouse())
		return;

	if( event->buttons().testFlag(Qt::LeftButton) && mMoving) {
		this->move(this->pos() + (event->pos() - mLastMousePosition));
	}
}
void BorderlessMainWindow::mouseReleaseEvent(QMouseEvent* event) {
	if (!mTitlebarWidget->underMouse() && !mWindowTitle->underMouse())
	return;

	if(event->button() == Qt::LeftButton) {
		mMoving = false;
	}
}
void BorderlessMainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
	Q_UNUSED(event);
	if (!mTitlebarWidget->underMouse() && !mWindowTitle->underMouse())
		return;

	mMaximized = !mMaximized;
	if (mMaximized) {
		slot_maximized();
	} else {
		slot_restored();
	}
}
void BorderlessMainWindow::slot_minimized() {
	setWindowState(Qt::WindowMinimized);
}
void BorderlessMainWindow::slot_restored() {
	mRestoreButton->setVisible(false);
	mMaximizeButton->setVisible(true);
	setWindowState(Qt::WindowNoState);
	setStyleSheet("#borderlessMainWindow{border:1px solid palette(highlight);}");
}
void BorderlessMainWindow::slot_maximized() {
	mRestoreButton->setVisible(true);
	mMaximizeButton->setVisible(false);
	showFullScreen();
	setStyleSheet("#borderlessMainWindow{border:1px solid palette(base);}");
}
void BorderlessMainWindow::slot_closed() {
	close();
}
void BorderlessMainWindow::resizeEvent(QResizeEvent* event)
{
	int nImageLayoutWidth = mMainWindow->ui.gridLayout_4->geometry().width();
	int nImageLayoutHeight = mMainWindow->ui.gridLayout_4->geometry().height();

	// 너비는 변하질 않음...
	if(mMainWindow->m_ciImage != NULL) {
		int nWidth = 0;
		int nHeight = 0;
		if(nImageLayoutWidth > nImageLayoutHeight) {
			nWidth = nImageLayoutHeight;
			nHeight = nImageLayoutHeight * ((float)mMainWindow->m_ciImage->m_nImageHeight / (float)mMainWindow->m_ciImage->m_nImageWidth);
		}
		else {
			nWidth = nImageLayoutWidth;
			nHeight = nImageLayoutWidth * ((float)mMainWindow->m_ciImage->m_nImageHeight / (float)mMainWindow->m_ciImage->m_nImageWidth);
		}

		mMainWindow->m_ciImage->setImageScreenSize(nWidth,nHeight);
	}
}

// 초기화 //
void CPlatform::init()
{
	m_nActivatedFrameIdx = -1;
	m_ciImage = NULL;
}
void CPlatform::clear()
{
	if(m_ciImage != NULL) {
		delete m_ciImage;
		m_ciImage = NULL;
	}
}

// Widget 생성 //
void CPlatform::createProgressBar()
{
	progressBar = new QProgressBar(ui.statusBar);
	progressBar->setMaximumSize(250, 15);
	progressBar->setAlignment(Qt::AlignRight);
	progressBar->setValue(0);
	progressBar->setVisible(false);

	ui.statusBar->addPermanentWidget(progressBar);
}
void CPlatform::setSignalSlot()
{
	// platform
	connect(ui.pushButton_run, SIGNAL(clicked()), this, SLOT(run()));
	connect(ui.horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollChangeImage(int)));
	//connect(mCloseButton, SIGNAL(clicked()), this, SLOT(slot_closed()));
	/*
	mCloseButton = new QPushButton(mTitlebarWidget);
	mCloseButton->setObjectName("closeButton");
	connect(mCloseButton, SIGNAL(clicked()), this, SLOT(slot_closed()));
	*/

	// filter
	connect(ui.radioButton_None, SIGNAL(clicked()), this, SLOT(setFilterMode()));
	connect(ui.radioButton_Gaussian, SIGNAL(clicked()), this, SLOT(setFilterMode()));
	connect(ui.radioButton_Laplacian, SIGNAL(clicked()), this, SLOT(setFilterMode()));

	// radiomics feature family
	connect(ui.checkBox_Histogram, SIGNAL(clicked()), this, SLOT(setCheckedState()));
	connect(ui.checkBox_Intensity, SIGNAL(clicked()), this, SLOT(setCheckedState()));
	connect(ui.checkBox_Morph, SIGNAL(clicked()), this, SLOT(setCheckedState()));
	connect(ui.checkBox_GLCM, SIGNAL(clicked()), this, SLOT(setCheckedState()));

	// feature (pop-up)

}
void CPlatform::setProgressBarValue(int nCurrentIdx, int nMaximumIdx)
{
	float fValue = (float)(nCurrentIdx+1) / (float)nMaximumIdx * 100;

	if(nCurrentIdx == 0) {
		progressBar->setVisible(true);
	}

	progressBar->setValue(fValue);

	if((nCurrentIdx+1) == nMaximumIdx) {
		progressBar->setValue(0);
		progressBar->setVisible(false);
	}
}

// 화면 resize event //
void CPlatform::resizeEvent(QResizeEvent* event)
{
	int nImageLayoutWidth = ui.gridLayout_4->geometry().width();
	int nImageLayoutHeight = ui.gridLayout_4->geometry().height();

	// 너비는 변하질 않음...
	if(m_ciImage != NULL) {
		int nWidth = 0;
		int nHeight = 0;
		if(nImageLayoutWidth > nImageLayoutHeight) {
			nWidth = nImageLayoutHeight;
			nHeight = nImageLayoutHeight * ((float)m_ciImage->m_nImageHeight / (float)m_ciImage->m_nImageWidth);
		}
		else {
			nWidth = nImageLayoutWidth;
			nHeight = nImageLayoutWidth * ((float)m_ciImage->m_nImageHeight / (float)m_ciImage->m_nImageWidth);
		}

		m_ciImage->setImageScreenSize(nWidth,nHeight);
	}
}

// 파일 이름 순으로 정렬 //
bool compareNames(const QString& s1, const QString& s2)
{
	// ignore common prefix..
	int i = 0;
	while ((i < s1.length()) && (i < s2.length()) && (s1.at(i).toLower() == s2.at(i).toLower()))
		++i;
	++i;
	// something left to compare?
	if ((i < s1.length()) && (i < s2.length()))
	{
		// get number prefix from position i - doesnt matter from which string
		int k = i - 1;
		//If not number return native comparator
		if (!s1.at(k).isNumber() || !s2.at(k).isNumber())
		{
			//Two next lines
			//E.g. 1_... < 12_...
			if (s1.at(k).isNumber())
				return false;
			if (s2.at(k).isNumber())
				return true;
			return QString::compare(s1, s2, Qt::CaseSensitive) < 0;
		}
		QString n = "";
		k--;
		while ((k >= 0) && (s1.at(k).isNumber()))
		{
			n = s1.at(k) + n;
			--k;
		}
		// get relevant/signficant number string for s1
		k = i - 1;
		QString n1 = "";
		while ((k < s1.length()) && (s1.at(k).isNumber()))
		{
			n1 += s1.at(k);
			++k;
		}

		// get relevant/signficant number string for s2
		//Decrease by
		k = i - 1;
		QString n2 = "";
		while ((k < s2.length()) && (s2.at(k).isNumber()))
		{
			n2 += s2.at(k);
			++k;
		}

		// got two numbers to compare?
		if (!n1.isEmpty() && !n2.isEmpty())
			return (n + n1).toInt() < (n + n2).toInt();
		else
		{
			// not a number has to win over a number.. number could have ended earlier... same prefix..
			if (!n1.isEmpty())
				return false;
			if (!n2.isEmpty())
				return true;
			return s1.at(i) < s2.at(i);
		}
	}
	else
	{
		// shortest string wins
		return s1.length() < s2.length();
	}
}

// drag and drop event //
void CPlatform::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}
void CPlatform::dropEvent(QDropEvent* event)
{
	QStringList list;

	foreach (const QUrl &url, event->mimeData()->urls()) {
		list.push_back(url.toLocalFile());
	}

	readImage(list);	
}

// keyboard event //
void CPlatform::keyPressEvent(QKeyEvent* event)
{
	switch(event->key()) {
		case Qt::Key_Control:
			
		break;
	}
}
void CPlatform::keyReleaseEvent(QKeyEvent* event)
{
	switch(event->key()) {
		case Qt::Key_Control:
			
		break;
	}
}

// open, load, Image //
void CPlatform::readImage(QStringList list) {

	QStringList fileList;
	for (int i = 0, ni = list.size(); i<ni; i++) {
		QFileInfo f(list[i]);

		if (f.isDir()) {
			QDirIterator it(list[i], QDir::Dirs | QDir::Files, QDirIterator::Subdirectories);

			do {
				it.next();
				if (it.fileInfo().isDir() == false) {
					fileList.push_back(it.filePath());
				}
			} while (it.hasNext());
		}
		else {
			fileList.push_back(list[i]);
		}
	}

	sort(fileList.begin(), fileList.end(), compareNames);

	for (int i = 0, ni = fileList.size(); i<ni; i++) {
		char cInputPath[1024] = { 0 };
		QString path = fileList.at(i).toUtf8().constData();
		QTextCodec* c = QTextCodec::codecForLocale();
		QByteArray b = c->fromUnicode(path);
		std::memcpy(cInputPath, b.data(), b.size() + 1);

		m_ciData.readImage(cInputPath);
		setProgressBarValue(i, ni);
	}

	m_ciData.matchingImageAndMask();
	m_ciData.sortingImageAndMask();

	// 첫번째 Series, 첫번째 image
	showImage(0);

	int nStartFrameIdx = 0;
	int nEndFrameIdx = m_ciData.getSeries(0)->getImageCount() - 1;
	ui.horizontalScrollBar->setMaximum(nEndFrameIdx - nStartFrameIdx);
	ui.horizontalScrollBar->setMinimum(0);
}
void CPlatform::showImage(int nFrameIdx)
{
	m_nActivatedFrameIdx = nFrameIdx;
	short* psImage = m_ciData.getImage(nFrameIdx);
	unsigned char* pucMask = m_ciData.getMask(nFrameIdx);
	int nWidth = m_ciData.getWidth(nFrameIdx);
	int nHeight = m_ciData.getHeight(nFrameIdx);
	

	// 처음 Data를 로드하는 경우
	// CImage* m_ciImage => image pointer
	if(m_ciImage == NULL) {
		m_ciImage = new CImageView;

		m_ciImage->init(this); // m_nImageWidth, m_nImageHeight 셋팅 (512, 512)
		m_ciImage->setMinimumWidth(200);
		m_ciImage->setMinimumHeight(200);
		m_ciImage->setImageScreenSize(562, 562); // ***여기서 redraw 함수 호출!*** => overlay!

		ui.gridLayout_4->addWidget(m_ciImage, 0, 0); // gridLayout_4 객체에 위젯 공간 추가 (0, 0 위치에) => 여기서 image show(display) 하는 건 X
	
	}
	else {
		m_ciImage->init(this);
		m_ciImage->setMinimumWidth(200);
		m_ciImage->setMinimumHeight(200);
		m_ciImage->setImageScreenSize(562, 562);

		// 넣고 다시
		int nImageLayoutWidth = ui.gridLayout_4->geometry().width(); // 587
		int nImageLayoutHeight = ui.gridLayout_4->geometry().height(); // 562


		if (nImageLayoutWidth > nImageLayoutHeight) {
			nWidth = nImageLayoutHeight;
			nHeight = nImageLayoutHeight * ((float)m_ciImage->m_nImageHeight / (float)m_ciImage->m_nImageWidth);
		}
		else {
			nWidth = nImageLayoutWidth;
			nHeight = nImageLayoutWidth * ((float)m_ciImage->m_nImageHeight / (float)m_ciImage->m_nImageWidth);
		}

		m_ciImage->setImageScreenSize(nWidth, nHeight); // 창 크기 변경함에 따라 image 크기도 변하도록 다시 세팅
	}
}
void CPlatform::showImage(QTreeWidgetItem* item, int column)
{
	if(item->text(1) != 0) {
		int nFrameIdx = item->text(1).toInt();
		showImage(nFrameIdx);
	}
}

// scroll //
void CPlatform::scrollChangeImage(int nValue)
{
	//cout << "nValue : " << nValue << endl;
	showImage(nValue);
}


// Algorithms ------------------------------------------------------------------------------------------------------------------------------

// ROI slice check //
bool isEmptyMask(unsigned char* pucMask, int nWidth, int nHeight) {

	Mat mask(nWidth, nHeight, CV_8UC1, pucMask);

	return countNonZero(mask) < 1 ? true : false;
}


// Filter //
void CPlatform::setFilterMode() { // radio btn 체크될 때마다(시그널) 호출되는 SLOT 함수
	
	if (ui.radioButton_None->isChecked())
		FILTER_MODE = FILTER_NONE;
	if (ui.radioButton_Gaussian->isChecked())
		FILTER_MODE = FILTER_GAUSSIAN;
	if (ui.radioButton_Laplacian->isChecked())
		FILTER_MODE = FILTER_LAPLACIAN;

}
void filtering(short* psImage, Mat &img_filtered, int nWidth, int nHeight, int FILTER_MODE) {
	
	Mat img(nWidth, nHeight, CV_16SC1, psImage);

	switch (FILTER_MODE) 
	{
		case FILTER_NONE:
			img_filtered = img.clone();
			break;
		
		case FILTER_GAUSSIAN: 
		{
			GaussianBlur(img, img_filtered, Size(7, 7), 0); 
			break;
		}
			
		case FILTER_LAPLACIAN: 
		{
			Laplacian(img, img_filtered, CV_16S, 7);
			break;
		}

		default:
			cout << "error : Unknown Filter Mode!" << endl;
	}
}


// Feature Extraction //
void CPlatform::setCheckedState() { // check box 클릭될 때마다(시그널) 호출되는 SLOT 함수
	
	QObject* obj = sender();

	if (obj == ui.checkBox_Histogram) {
		intenseHisto.isActivatedFamily = !intenseHisto.isActivatedFamily;
		if (intenseHisto.isActivatedFamily == true) {
			// pop-up (set isCheckedFeature, nCheckedFeatures) => if문으로 체크 후 T/F push 하는걸로 수정
			intenseHisto.isCheckedFeature.push_back(true);
			intenseHisto.isCheckedFeature.push_back(true);
			intenseHisto.isCheckedFeature.push_back(true);
			intenseHisto.isCheckedFeature.push_back(true);
			for (int i = 0; i < 19; i++) {
				intenseHisto.isCheckedFeature.push_back(false);
			}

			intenseHisto.nCheckedFeatures = 4;
		}
	}

	if (obj == ui.checkBox_Intensity) {

	}

	if (obj == ui.checkBox_Morph) {

	}

	if (obj == ui.checkBox_GLCM) {

	}
}
void CPlatform::featureExtraction(short* psImage, unsigned char* pucMask, int nHeight, int nWidth) {
	if (intenseHisto.isActivatedFamily) {
		intenseHisto.featureExtraction(psImage, pucMask, nHeight, nWidth);
	}

	if (ui.checkBox_Intensity->isChecked()) {
		//short localIntensityPeak = calcLocalIntensityPeak(psImage, pucMask, nHeight, nWidth);
		//localIntense.push_back(localIntensityPeak);
	}

	if (ui.checkBox_Morph->isChecked()) {

	}

	if (ui.checkBox_GLCM->isChecked()) {

	}

}

void CPlatform::averageAllSlices() {
	if (intenseHisto.isActivatedFamily) {
		intenseHisto.averageAllValues();
	}

	if (ui.checkBox_Intensity->isChecked()) {
		
	}

	if (ui.checkBox_Morph->isChecked()) {

	}

	if (ui.checkBox_GLCM->isChecked()) {

	}

}

// [ 3.2 Loacal Intensity Features ]
short CPlatform::calcLocalIntensityPeak(short* pusImage, unsigned char* pucMask, int nHeight, int nWidth) {

	// 1. get center pos of max (intensity peak in ROI)
	pair<int, int> posOfCenter; // 현재 임시 좌표 및 center(max) 좌표 => (row, col)
	short maxValue = 0; // 현재 임시 좌표 및 최종 peak(max) 값

	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col; // 1차원으로 접근
			unsigned char maskValue = pucMask[index];
			short imageValue = pusImage[index];

			// ROI 내부 && 임시 최대값보다 큰 값 선택 (ROI가 없으면 계속 pass)
			if (maskValue >(unsigned char)0 && imageValue > maxValue) {
				maxValue = imageValue;
				//posOfCenter = { row, col };	// c++ 11이상 (vs2012에서 안돼용 ㅜㅜ)
				posOfCenter = make_pair(row, col);
			}
		}
	}

	cout << "maxValue : " << maxValue << endl; // 180
	cout << "posOfCenter : " << posOfCenter.first << ", " << posOfCenter.second << endl; // (230, 246)

	/*
	// 2. calculateConvolutionMatrix(image)
	// 2-1. get the size of the convolutional matrix (getConvMatrixSize) and allocate convMatrix
	float radius = 6.2;
	double pixelSpacing = m_ciData.getPixelSpacing(9); // 0.650391mm
	int nPixels = 2 * floor((radius - (float)pixelSpacing / 2.0) / pixelSpacing) + 1; // floor : 버림. nPixels (19) : 원모양 컨볼루션 마스크의 지름에 포함되는 최대 픽셀 개수 (가로, 세로 길이)

	unsigned char** convMatrix = new unsigned char*[nPixels]; // 2차원 convolution mask 동적 할당. nPixels개 만큼의 포인터를 행으로 갖고 있는 배열 (nPixels 행)

	for (int row = 0; row < nPixels; row++) {
	convMatrix[row] = new unsigned char[nPixels]; // 각 행의 열값들 채우기(초기화)
	memset(convMatrix[row], 0, sizeof(unsigned char)*nPixels); // 메모리 공간을 0으로 초기화
	}

	// 2-2. fill the convolutional matrix (19 x 19)
	pair<float, float> tempPos = { 0, 0 };
	float distFromCenter;

	for (int row = 0; row < nPixels; row++) {
	for (int col = 0; col < nPixels; col++) {
	tempPos.first = float(row) * pixelSpacing + pixelSpacing / 2 - (posOfCenter.first + 0.5) * pixelSpacing; // X축(row) 방향 거리(mm)
	tempPos.second = float(col) * pixelSpacing + pixelSpacing / 2 - (posOfCenter.second + 0.5) * pixelSpacing; // Y
	cout << "tempPos.first : " << tempPos.first << endl;
	cout << "tempPos.second : " << tempPos.second << endl;

	distFromCenter = sqrt(pow((tempPos.first - (posOfCenter.first + 0.5)*pixelSpacing), 2) + pow((tempPos.second - (posOfCenter.second + 0.5) * pixelSpacing), 2));
	if (abs(tempPos.first) <= 6.2 && abs(tempPos.second) <= 6.2) {
	//cout << "hi?" << endl;
	convMatrix[row][col] = 1;
	}
	//cout << convMatrix[row][col] << " ";
	}
	//cout << endl;
	}


	// convMatrix 찍어보기? size -> nPixel or 512?
	Mat img(Size(nPixels, nPixels), CV_8UC1, convMatrix);
	imshow("img", img);
	waitKey(0);


	// 2-3. calculateConvolutionMatrix => getConvMatix & fillConvMatrix 호출 후 convMatrix return (nPixels x nPixels 크기)
	*/

	// 2. check distFromCenter and calculate sum (mean) 
	float radius = 6.2;
	double pixelSpacing = m_ciData.getPixelSpacing(9); // 0.650391mm
	int nrPixels = floor((radius - (float)pixelSpacing / 2.0) / pixelSpacing); // 9. floor : 버림
	int nPixels = 2 * nrPixels + 1; // 19

	pair<float, float> tempDist;
	float distFromCenter;
	short sumIntensity = 0;
	short meanIntensity;
	int cnt = 0; // 279

	// [230-9][246-9] center 좌표의 픽셀로부터 거리 계산 => 9 : nrPixels
	for (int row = posOfCenter.first - nrPixels; row < posOfCenter.first + nrPixels; row++) {
		for (int col = posOfCenter.second - nrPixels; col < posOfCenter.second + nrPixels; col++) {
			tempDist.first = float(row) * pixelSpacing + pixelSpacing / 2 - (posOfCenter.first + 0.5) * pixelSpacing; // X축(row) 방향 중점과의 거리(mm) => X_temp - X_center
			tempDist.second = float(col) * pixelSpacing + pixelSpacing / 2 - (posOfCenter.second + 0.5) * pixelSpacing; // Y
			distFromCenter = sqrt(pow(tempDist.first, 2) + pow(tempDist.second, 2));
			if (distFromCenter <= 6.2) {
				// sum pixel
				int index = row * nWidth + col;
				sumIntensity += pusImage[index];
				cnt++;
			}
		}
	}

	meanIntensity = sumIntensity / cnt;
	cout << "meanIntensity : " << meanIntensity << endl;

	return meanIntensity;
}


// Preset CSV File // 
void CPlatform::presetCSVFile(string csvName) {

	ofstream resultCSV(csvName); // 파일이 없으면 새로 생성, 있으면 기존 내용 지우고 새로 작성 (remove 포함)


								 // write family name 
	resultCSV << " " << "," << " " << ",";

	if (intenseHisto.isActivatedFamily) {
		for (int i = 0; i < intenseHisto.nCheckedFeatures; i++) {
			resultCSV << "Histogram" << ",";
		}
	}
	/*
	if (localIntense.isActivatedFamily) {
	for (int i = 0; i < localIntense.nCheckedFeatures; i++) {
	resultCSV << "Local Intensity" << ",";
	}
	}
	if (morphology.isActivatedFamily) {
	for (int i = 0; i < morphology.nCheckedFeatures; i++) {
	resultCSV << "Morphology" << ",";
	}
	}
	if (glcm.isActivatedFamily) {
	for (int i = 0; i < glcm.nCheckedFeatures; i++) {
	resultCSV << "GLCM" << ",";
	}
	}
	*/

	resultCSV << "\n";


	// write feature name 
	resultCSV << " " << "," << " " << ",";

	if (intenseHisto.isActivatedFamily) {
		vector<string> featureNames;
		intenseHisto.defineFeatureNames(featureNames); // 일단 전체 feature name들 get
		for (int i = 0; i < featureNames.size(); i++) {
			if (intenseHisto.isCheckedFeature[i]) {
				resultCSV << featureNames[i] << ",";
			}
		}
	}

	if (ui.checkBox_Intensity->isChecked()) {

	}

	if (ui.checkBox_Morph->isChecked()) {

	}

	if (ui.checkBox_GLCM->isChecked()) {

	}

	resultCSV.close();

}


// Save Result //
void writeCSVCheckedValue(vector<float> extractedValues, string csvName) // 모든 class 공통
{
	ofstream resultCSV(csvName, std::ios_base::app);

	for (int i = 0; i< extractedValues.size(); i++) {
		// if(extractedValues[i] != NAN) => error! (-nan(ind) !- NAN)
		/*
		if (!isnan(extractedValues[i])) { 
			resultCSV << extractedValues[i] << ",";
		}
		*/
		resultCSV << extractedValues[i] << ",";
	}
	resultCSV.close();
}
void CPlatform::writeCSVCaseName(int seriesIdx, string csvName) {
	ofstream resultCSV(csvName, std::ios_base::app); // std::ios_base::app => 이어쓰기 모드로 열기(open)

	resultCSV << "\n";

	string patientName = m_ciData.getSeries(seriesIdx)->m_sPatientName;
	string studyName = m_ciData.getSeries(seriesIdx)->m_sStudyName;
	string seriesName = m_ciData.getSeries(seriesIdx)->m_sSeriesName;

	resultCSV << patientName << "," << studyName + '_' + seriesName << ",";

	resultCSV.close();
}
void CPlatform::writeCSVFeatureValue(string csvName) {

	if (intenseHisto.isActivatedFamily) {
		/*
		vector<float> hitogramValue; // 추출값들 push 받아올 벡터
		intenseHisto.extractFeatureValues(hitogramValue); // ***extractData : 모든 멤버변수 값들 vector에 push해오기(NAN 포함)***
		*/
		writeCSVCheckedValue(intenseHisto.final1DVec, csvName);
	}

	if (ui.checkBox_Intensity->isChecked()) {
		
	}

	if (ui.checkBox_Morph->isChecked()) {

	}

	if (ui.checkBox_GLCM->isChecked()) {

	}
}
void CPlatform::writeCSVFile(int seriesIdx, string csvName) {

	writeCSVCaseName(seriesIdx, csvName); // case명 & case명_modality 저장
	writeCSVFeatureValue(csvName); // feature values 저장 (각 family별로 호출)

}


// Clear All Vector //
void CPlatform::clearAll(int seriesIdx) {
	intenseHisto.clear();
	/*
	localIntense.clear();
	morphology.clear();
	glcm.clear();
	*/
	cout << "clear series[" << seriesIdx << "]'s all vectors!" << endl;
}

// Normalize and 16 bit Image Show //
void RescaleIntensityFilter(short* sIN_OUT_img, int nWidth, int nHeight, int nSetMax, int nSetMin)
{
	int nMin = INT_MAX;
	int nMax = INT_MIN;

	for (int i = 0; i<nWidth; i++) {
		for (int j = 0; j<nHeight; j++) {
			if (sIN_OUT_img[j*nWidth + i] > nMax) {
				nMax = sIN_OUT_img[j*nWidth + i];
			}

			if (sIN_OUT_img[j*nWidth + i] < nMin) {
				nMin = sIN_OUT_img[j*nWidth + i];
			}
		}
	}

	//normalization
	if (nMax - nMin != 0) {
		for (int i = 0; i<nWidth; i++) {
			for (int j = 0; j<nHeight; j++) {
				sIN_OUT_img[j*nWidth + i] = (sIN_OUT_img[j*nWidth + i] - nMin) * (nSetMax - nSetMin) / (nMax - nMin) + nSetMin;
			}
		}
	}
}
void ShowShortImage(short* sImg, int nWidth, int nHeight, char* cWindowName)
{
	int nImgSize = nWidth*nHeight;
	short* sCopyImg = new short[nImgSize];
	memcpy(sCopyImg, sImg, sizeof(short)*nImgSize);

	RescaleIntensityFilter(sCopyImg, nWidth, nHeight, 255, 0);

	IplImage* img = cvCreateImage(cvSize(nWidth, nHeight), 8, 1);

	for (int row = 0; row< nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			img->imageData[img->widthStep*row + col] = (char)sCopyImg[row*nWidth + col];
		}
	}

	if (cWindowName == NULL) {
		cWindowName = "test";
	}

	cvShowImage(cWindowName, img);
	cvWaitKey(0);

	cvReleaseImage(&img);


	delete[] sCopyImg;
}


// Get min, max of Mat //
double getMinOfMat(Mat m) {
	double minVal;
	double maxVal;
	Point minLoc;
	Point maxLoc;
	minMaxLoc(m, &minVal, &maxVal, &minLoc, &maxLoc);

	return minVal;
}
double getMaxOfMat(Mat m) {
	double minVal;
	double maxVal;
	Point minLoc;
	Point maxLoc;
	minMaxLoc(m, &minVal, &maxVal, &minLoc, &maxLoc);

	return maxVal;
}



// Run //
void CPlatform::run()
{
	// create and preset csv file //
	const static string outputFolder = string("Result/");
	string csvName = outputFolder + "result.csv";

	presetCSVFile(csvName);
	
	
	// filtering and feature extraction by series //
	int nSeriesCnt = m_ciData.getSeriesCount();

	cout << "*** nSeriesCnt : " << nSeriesCnt << " ***" << endl;

	for (int i = 0; i < nSeriesCnt; i++) {

		int nWidth = 0;		
		int nHeight = 0;	
		int nImageCnt = 0;	
		int nMaskCnt = 0;	
		short** ppsImages = NULL;
		unsigned char** ppucMasks = NULL;

		// copy series //
		m_ciData.copyImages(i, ppsImages, nImageCnt, nWidth, nHeight);
		m_ciData.copyMasks(i, ppucMasks, nMaskCnt, nWidth, nHeight);

		// slice by slice //
		for (int j = 0; j < nImageCnt; j++) {
			// ROI slice check //
			if (!isEmptyMask(ppucMasks[j], nWidth, nHeight)) {
				
				// filtering //
				Mat img_filtered;
				filtering(ppsImages[j], img_filtered, nWidth, nHeight, FILTER_MODE); 
				SAFE_DELETE_ARRAY(ppsImages[j]); // psImage 포인터가 가리키고 있던 메모리 데이터들 해제 => 이후엔 Mat 데이터를 가리키므로, 또 해제해주지 않아야 함!
				ppsImages[j] = (short*)img_filtered.data; // 주소 변경

				// feature extraction //
				featureExtraction(ppsImages[j], ppucMasks[j], nHeight, nWidth);
			}
		}

		// mean all ROI slices //
		averageAllSlices();

		// write csv file //
		writeCSVFile(i, csvName);
		
		// clear all vector //
		clearAll(i);

		// 메모리 소멸 //
		SAFE_DELETE_VOLUME(ppucMasks, nMaskCnt);

	}

	// finish pop-up and exit		
	if (QMessageBox(QMessageBox::Information, " ", "extraction finished!", QMessageBox::Close).exec() == QMessageBox::Close) {
		QApplication::quit();
	}

	/*
	QMessageBox msgBox;
	msgBox.setWindowTitle("Radiomics Feature Extraction");
	//msgBox.setText("<p align='center'>finish!&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<br>");
	msgBox.setText("<br><br>extraction finished!");
	msgBox.setStandardButtons(QMessageBox::Close);

	QGridLayout* layout = (QGridLayout*)msgBox.layout();
	layout->setColumnMinimumWidth(1, 190);
	layout->setRowMinimumHeight(1, 45);

	if (msgBox.exec() == QMessageBox::Close) {
	QApplication::quit();
	}
	*/
	
}

