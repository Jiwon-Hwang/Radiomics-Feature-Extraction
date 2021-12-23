#include "platform.h"

using namespace std;
using namespace cv;

int FILTER_MODE; 
vector<string> featureFamilyName_list = {"Histogram", "Local Intensity", "Morphology", "GLCM"};

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
	connect(ui.pushButton_run, SIGNAL(clicked()), this, SLOT(run()));
	connect(ui.horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollChangeImage(int)));
	connect(ui.radioButton_None, SIGNAL(clicked()), this, SLOT(setFilterMode()));
	connect(ui.radioButton_Gaussian, SIGNAL(clicked()), this, SLOT(setFilterMode()));
	connect(ui.radioButton_Laplacian, SIGNAL(clicked()), this, SLOT(setFilterMode()));
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
	for(int i=0, ni=list.size(); i<ni; i++) {
		QDirIterator it(list[i], QDir::Dirs|QDir::Files, QDirIterator::Subdirectories);

		do {
			it.next();
			if(it.fileInfo().isDir() == false) {
				fileList.push_back(it.filePath());
			}
		} while (it.hasNext());
	}

	for(int i=0, ni=fileList.size(); i<ni; i++) {
		char cInputPath[1024] = {0};
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
	int nEndFrameIdx = m_ciData.getSeries(0)->getImageCount()-1; // 0번째 시리즈만 보여줌
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

// Filter //
void CPlatform::setFilterMode() { // radio btn 체크될 때마다(시그널) 호출되는 슬롯 함수
	
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
// [ 3.2 Loacl Intensity Features ] // 
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

// [ 3.4 Intensity Histogram Features ] //
vector<short> getVectorOfPixelsInROI(short* pusImage, unsigned char* pucMask, int nHeight, int nWidth) {

	vector<short> vectorOfOriPixels; // size : 3032
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			short imageValue = pusImage[index];

			// ROI 내부값 push (ROI가 없으면 계속 pass)
			if (maskValue >(unsigned char)0) {
				vectorOfOriPixels.push_back(imageValue);
			}
		}
	}

	sort(vectorOfOriPixels.begin(), vectorOfOriPixels.end()); // min(front) : -1, max(back) : 180
	
	return vectorOfOriPixels;
}
vector<unsigned short> getVectorOfDiscretizedPixels_nBins(vector<short> vectorOfOriPixels, int nBins = 32) {
	
	float minimumValue = (float)*min_element(vectorOfOriPixels.begin(), vectorOfOriPixels.end()); // min_element() : pointer return
	float maximumValue = (float)*max_element(vectorOfOriPixels.begin(), vectorOfOriPixels.end());
	vector<float> tempFloatVec(vectorOfOriPixels.begin(), vectorOfOriPixels.end());

	if (minimumValue == 0 && minimumValue == maximumValue) {
		cout << "error in calculating discretization, VOI contains only 0" << endl;
		exit(0);
	}
	else if (minimumValue > 0 && minimumValue == maximumValue) {
		cout << "error in calculating discretization, VOI contains only one intensity value, minimum value is set to 0" << endl;
		minimumValue = 0;
	}

	//subtract minimum value from every matrix element
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(minus<float>(), minimumValue)); // minus<T> : 이항 연산 함수 객체 (-) / bind2nd : 2번째 인수를 고정해 함수 객체로 변환 / transform : 일괄 연산

	//get the range
	float range = (maximumValue - minimumValue) / nBins; // range : 몫 (width of a bin) => ***float이 들어가면 / 는 몫이 아니라 진짜 "나누기" 연산!!!***

	//divide every element of the matrix by the range
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(divides<float>(), range));

	//replace 0 to 1 => ibsi 핑크색 마킹 부분! (min - min = 0이 되므로 최소인 1로 바꿔줘야 뒤에 ceil에서 min value of each bin : 1 유지)
	replace(tempFloatVec.begin(), tempFloatVec.end(), 0, 1);

	//do rounding
	for (int i = 0; i < tempFloatVec.size(); i++) {
		tempFloatVec[i] = ceil(tempFloatVec[i]);
	}

	//type casting
	vector<unsigned short> vectorOfDiscretizedPixels(tempFloatVec.begin(), tempFloatVec.end()); // 양자화한 최종 결과 픽셀값들 담을 벡터

	return vectorOfDiscretizedPixels;
}
vector<unsigned short> getVectorOfDiffGreyLevels(vector<unsigned short> vectorOfDiscretizedPixels) {

	vector<unsigned short> diffGreyLevels(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end()); // 1~nbins 사이 값으로 양자화된 픽셀값들
	diffGreyLevels.erase(unique(diffGreyLevels.begin(), diffGreyLevels.end()), diffGreyLevels.end()); // size : 168 => 32 bins로 나누면 한 구간 당 약 5개의 픽셀값들의 빈도수들 누적

	return diffGreyLevels;
}
vector<unsigned int> getHistogram(vector<unsigned short> vectorOfDiscretizedPixels) {

	vector<unsigned short> diffGreyLevels = getVectorOfDiffGreyLevels(vectorOfDiscretizedPixels); 

	vector<unsigned int> hist;
	unsigned int nCnt;
	unsigned short greyLevel;
	for (int i = 0; i < diffGreyLevels.size(); i++) {
		greyLevel = diffGreyLevels[i];
		nCnt = 0;
		for (int j = 0; j < vectorOfDiscretizedPixels.size(); j++) {
			if (vectorOfDiscretizedPixels[j] == greyLevel) {
				nCnt += 1;
			}
		}
		hist.push_back(nCnt);
	}

	return hist;
}

float calcMeanDiscretisedIntensity(vector<unsigned short> vectorOfDiscretizedPixels) {
	float mean = accumulate(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end(), 0.0) / vectorOfDiscretizedPixels.size(); // 0.0 : initial value of the sum

	return mean;
} 
float calcDiscretisedIntensityVariance(vector<unsigned short> vectorOfDiscretizedPixels) {
	unsigned int size = vectorOfDiscretizedPixels.size(); // size_t : unsigned int
	float mean = accumulate(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end(), 0.0) / size;
	vector<float> diff(size);
	transform(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end(), diff.begin(), bind2nd(minus<float>(), mean));
	float variance = inner_product(diff.begin(), diff.end(), diff.begin(), 0.0) / size;

	/*
	// with anonymous function : func = [capture](parameters){body}
	auto variance_func = [&mean, &size](float accumulator, const float& val) {
		return accumulator + ((val - mean) * (val - mean) / size);
	};
	variance = accumulate(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end(), 0.0, variance_func);
	*/

	/*
	// with for loop
	float variance = 0;
	for (int n = 0; n < vectorOfDiscretizedPixels.size(); n++)
	{
		variance += (vectorOfDiscretizedPixels[n] - meanValue) * (vectorOfDiscretizedPixels[n] - meanValue);
	}
	variance /= vectorOfDiscretizedPixels.size();
	*/

	return variance;
}


// Save Result //
// [ 3.2 Local Intensity Features ] // 
void defineLocalIntenseFeatures(vector<string> &features)
{

}
void extractLocalIntensityData(vector<float> &localIntensityData, vector<float> localIntensityFeatures)
{

}

// [ 3.4 Intensity Histogram Features ] //
void defineIntensityHistogramFeatures(vector<string> &features) {
	features.push_back("mean");
	features.push_back("variance");
	features.push_back("skewness");
	features.push_back("kurtosis");
	features.push_back("median");
	features.push_back("minimum");
	features.push_back("10th percentile");
	features.push_back("90th percentile");
	features.push_back("maximum");
	features.push_back("Interquartile range");
	features.push_back("mode");
	features.push_back("range");
	features.push_back("Mean absolut deviation");
	features.push_back("Robust mean absolute deviation");
	features.push_back("Median absolut deviation");
	features.push_back("Coefficient of variation");
	features.push_back("Quartile coefficient");
	features.push_back("Entropy");
	features.push_back("Uniformity");
	//features.push_back("Energy");
	features.push_back("Maximum histogram gradient");
	features.push_back("Maximum histogram gradient grey level");
	features.push_back("Minimum histogram gradient");
	features.push_back("Minimum histogram gradient grey level");	
}
void extractIntensityHistogramData(vector<float> intensityHistogramFeatures, vector<float> &intensityHistogramValues) {
	// 일단 여기서 intensityHistogramFeatures라는 객체의 멤버변수 값들(NAN을 포함한 특징값들) 모두 한 벡터(intensityHistogramValues)에 push 
	// 추후 밖에서 이 벡터를 세부 특징들 체크 T/F 여부 담는 bool 배열 반복문으로 체크하면서 True에 해당하는 idx의 특징값들만 뽑아서 writeCSV

	//intenseData.push_back(intenseFeatures.meanValue);
	//intenseData.push_back(intenseFeatures.varianceValue);
	/*
	intenseData.push_back(intenseFeatures.skewnessInt);
	intenseData.push_back(intenseFeatures.kurtosisInt);
	intenseData.push_back(intenseFeatures.medianValue);
	intenseData.push_back(intenseFeatures.minimumValue);
	intenseData.push_back(intenseFeatures.percentile10);
	intenseData.push_back(intenseFeatures.percentile90);
	intenseData.push_back(intenseFeatures.maximumValue);
	intenseData.push_back(intenseFeatures.interquartileRange);
	intenseData.push_back(intenseFeatures.mode);
	intenseData.push_back(intenseFeatures.rangeValue);
	intenseData.push_back(intenseFeatures.meanAbsDev);
	intenseData.push_back(intenseFeatures.robustMeanAbsDev);
	intenseData.push_back(intenseFeatures.medianAbsDev);
	intenseData.push_back(intenseFeatures.coeffOfVar);
	intenseData.push_back(intenseFeatures.quartileCoeff);
	intenseData.push_back(intenseFeatures.entropy);
	intenseData.push_back(intenseFeatures.histUniformity);
	//intenseData.push_back(intenseFeatures.energyValue);
	intenseData.push_back(intenseFeatures.maxHistGradient);
	intenseData.push_back(intenseFeatures.maxHistGradGreyValue);
	intenseData.push_back(intenseFeatures.minHistGradient);
	intenseData.push_back(intenseFeatures.minHistGradGreyValue);
	*/
}


void writeCSVCheckedValue(vector<float> extractedValues, string csvName)
{
	ofstream resultCSV(csvName, std::ios_base::app);

	for (int i = 0; i< extractedValues.size(); i++) {
		if (extractedValues[i] != NAN) { // false(not checked)면 멤버변수의 초기값 NAN
			resultCSV << extractedValues[i] << ",";
		}
	}
	resultCSV.close();
}

void CPlatform::writeCSVCaseName(int seriesIdx, string csvName) {
	ofstream resultCSV(csvName, std::ios_base::app); // std::ios_base::app => 이어쓰기 모드로 열기(open)

	// ***추후 series.h에서 멤버변수로 폴더명 접근 가능. series idx로 접근***
	string imagePath = m_ciData.getImagePath(seriesIdx, 9); // (nSeriesIdx, nImageIdx)
	vector<string> paths = m_ciData.splitPath(imagePath);

	resultCSV << paths[paths.size() - 4] << "," << paths[paths.size() - 3] << ",";

	resultCSV.close();
}

void CPlatform::writeCSVFeatureValue(string csvName) {

	if (ui.checkBox_Histogram->isChecked()) {
		//vector<float> hitogramValue; // 추출값들 push 받아올 벡터
		//intenseHisto.extractData(hitogramValue); // ***extractData : 각 클래스별 멤버함수. 각 family class의 멤버변수들에 접근해서 vector에 push해오기(NAN 포함)***
		//writeCSVCheckedValue(hitogramValue, csvName);
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


// Preset CSV File - write checked feature family name and feature name // 
void CPlatform::presetCSVFile(string csvName) {

	ofstream resultCSV(csvName); // 파일이 없으면 새로 생성, 있으면 기존 내용 지우고 새로 작성 (remove 포함)

								 // write feature family name (선택된(true) 큰 특징명들 이름 삽입)
	resultCSV << " " << "," << " " << ","; // 맨 앞 공백 2칸

	QList<QCheckBox *> checkbox_list = ui.groupBox_families->findChildren<QCheckBox *>(); // checkBoxes should be in groupBox

	int nFeatures = 2; // 추후 True로 체크된 특징 개수 parameter input => ***각 family마다 다른 개수 가져오기!! (ex. int family.nCheckedFeatures)***
	for (int i = 0; i < checkbox_list.size(); i++) {
		if (checkbox_list.at(i)->isChecked()) {
			for (int j = 0; j < nFeatures; j++) {
				resultCSV << featureFamilyName_list[i] << ",";
			}
		}
	}
	resultCSV << "\n";

	// write feature name (각 family마다 선택된(true) 세부 특징들 이름 삽입)
	resultCSV << " " << "," << " " << ","; // 맨 앞 공백 2칸

	if (ui.checkBox_Histogram->isChecked()) {
		vector<string> features;
		defineIntensityHistogramFeatures(features); // 전체 feature name들 get
		//for (int i = 0; i < features.size(); i++) {
		// ***여기서 해당 family에서 체크된 feature들 bool 배열 체크해서 if문 통과시키기 (true값 수정)***
		//if (true) { 
		for (int j = 0; j< nFeatures; j++) {
			resultCSV << features[j] << ",";
		}
	//}
	}

	if (ui.checkBox_Intensity->isChecked()) {

	}

	if (ui.checkBox_Morph->isChecked()) {

	}

	if (ui.checkBox_GLCM->isChecked()) {

	}

	resultCSV << "\n";

	resultCSV.close();

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
	

	// case별 바깥 for loop 시작 (ex. ICC_01_001 - ICC_01_001_CT) //

	// 배열 복사 //
	int nWidth = 0; // 512
	int nHeight = 0; // 512
	short* psImage = NULL;
	unsigned char* pucMask = NULL;
	
	m_ciData.copyImage(9, psImage, nWidth, nHeight); // DCM
	m_ciData.copyMask(9, pucMask, nWidth, nHeight); // label image

	/*
	// ***여러 file들 copy***
	int nWidth2 = 0;
	int nHeight2 = 0;
	int nImageCnt = 0; // copyImages()에서 nImageCnt 계산 후 반환
	short** ppsImages = NULL;
	m_ciData.copyImages(0, ppsImages, nImageCnt, nWidth2, nHeight2); // 한 그룹의 여러 dcm들 깊은 복사 => copyImage 호출

	cout << "nWidth2 : " << nWidth2 << endl;
	cout << "nHeight2 : " << nHeight2 << endl;
	cout << "nImageCnt : " << nImageCnt << endl;
	*/

	
	// filtering //
	/*
	for (int i = 0; i < 3; i++) {
	if (ui.radioButton_Gaussian->isChecked()) {
	int FILTER_MODE = FILTER_GAUSSIAN;
	}
	}
	*/
	Mat img_filtered;
	filtering(psImage, img_filtered, nWidth, nHeight, FILTER_MODE); // 0, 1, 2 : radio btn
	SAFE_DELETE_ARRAY(psImage); // psImage 포인터가 가리키고 있던 메모리 데이터들 해제 => 이후엔 Mat 데이터를 가리키므로, 또 해제해주지 않아야 함!
	psImage = (short*)img_filtered.data; // 주소 변경
	


	// feature extraction // => ***추후 feature family 각각 class로 빼기 & featureExtraction() 통합함수로 빼기***
	// ***이부분에서 벡터 대신 객체 생성 후 값 대입***
	vector<float> intensityHistogram;
	vector<short> localIntensity;
	vector<float> morphology;
	vector<float> glcm;

	//for (int i = 0; i < checkbox_list.size(); i++) { // checkbox.at(i) 이런식으로 추후 수정

	if (ui.checkBox_Histogram->isChecked()) {
		// 3.4.0 Ready for Feature Extraction
		int nBins = 32; // user input parameter
		vector<short> vectorOfOriPixels = getVectorOfPixelsInROI(psImage, pucMask, nHeight, nWidth);
		vector<unsigned short> vectorOfDiscretizedPixels = getVectorOfDiscretizedPixels_nBins(vectorOfOriPixels, nBins);
		vector<unsigned int> hist = getHistogram(vectorOfDiscretizedPixels);

		// 3.4.1 Mean discretised intensity
		float meanValue = calcMeanDiscretisedIntensity(vectorOfDiscretizedPixels); // 15.5894
		intensityHistogram.push_back(meanValue);

		// 3.4.2 Discretised intensity variance
		float varValue = calcDiscretisedIntensityVariance(vectorOfDiscretizedPixels); // 29.5587
		intensityHistogram.push_back(varValue);

		// 3.3.3 Discretised intensity skewness


	}


	if (ui.checkBox_Intensity->isChecked()) {
		short localIntensityPeak = calcLocalIntensityPeak(psImage, pucMask, nHeight, nWidth);
		localIntensity.push_back(localIntensityPeak);
			
	}

	if (ui.checkBox_Morph->isChecked()) {

	}

	if (ui.checkBox_GLCM->isChecked()) {

	}



	// write csv file 
	int seriesIdx = 0; // for loop idx
	writeCSVFile(seriesIdx, csvName);
	


	// 메모리 소멸 //
	SAFE_DELETE_ARRAY(pucMask);
	// 2차원 배열 일 땐, SAFE_DELETE_VOLUME(배열명, 1차원 배열 수)
	//SAFE_DELETE_VOLUME(ppsImages, nImageCnt);
	
}

