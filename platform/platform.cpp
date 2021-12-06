#include "platform.h"

using namespace std;
using namespace cv;

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

	// �ʺ�� ������ ����...
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

// �ʱ�ȭ //
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

// Widget ���� //
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

// ȭ�� resize event //
void CPlatform::resizeEvent(QResizeEvent* event)
{
	int nImageLayoutWidth = ui.gridLayout_4->geometry().width();
	int nImageLayoutHeight = ui.gridLayout_4->geometry().height();

	// �ʺ�� ������ ����...
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

// ���� �̸� ������ ���� //
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

	// ù��° Series, ù��° image
	showImage(0);

	int nStartFrameIdx = 0;
	int nEndFrameIdx = m_ciData.getSeries(0)->getImageCount()-1; // 0��° �ø�� ������
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
	

	// ó�� Data�� �ε��ϴ� ���
	// CImage* m_ciImage => image pointer
	if(m_ciImage == NULL) {
		m_ciImage = new CImageView;

		m_ciImage->init(this); // m_nImageWidth, m_nImageHeight ���� (512, 512)
		m_ciImage->setMinimumWidth(200);
		m_ciImage->setMinimumHeight(200);
		m_ciImage->setImageScreenSize(562, 562); // ***���⼭ redraw �Լ� ȣ��!*** => overlay!

		ui.gridLayout_4->addWidget(m_ciImage, 0, 0); // gridLayout_4 ��ü�� ���� ���� �߰� (0, 0 ��ġ��) => ���⼭ image show(display) �ϴ� �� X
	
	}
	else {
		m_ciImage->init(this);
		m_ciImage->setMinimumWidth(200);
		m_ciImage->setMinimumHeight(200);
		m_ciImage->setImageScreenSize(562, 562);

		// �ְ� �ٽ�
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

		m_ciImage->setImageScreenSize(nWidth, nHeight); // â ũ�� �����Կ� ���� image ũ�⵵ ���ϵ��� �ٽ� ����
	}
}

void CPlatform::showImage(QTreeWidgetItem* item, int column)
{
	if(item->text(1) != 0) {
		int nFrameIdx = item->text(1).toInt();
		showImage(nFrameIdx);
	}
}


// �˰��� //
void CPlatform::scrollChangeImage(int nValue)
{
	//cout << "nValue : " << nValue << endl;
	showImage(nValue);
}


// ***���� 9�� �����̽� 1�忡 ���ؼ��� ������***
short CPlatform::calcLocalIntensityPeak(short* pusImage, unsigned char* pucMask, int nHeight, int nWidth) {

	// 1. get center pos of max (intensity peak in ROI)
	pair<int, int> posOfCenter; // ���� �ӽ� ��ǥ �� center(max) ��ǥ => (row, col)
	short maxValue = 0; // ���� �ӽ� ��ǥ �� ���� peak(max) ��

	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col; // 1�������� ����
			unsigned char maskValue = pucMask[index];
			short imageValue = pusImage[index];

			// ROI ���� && �ӽ� �ִ밪���� ū �� ���� (ROI�� ������ ��� pass)
			if (maskValue >(unsigned char)0 && imageValue > maxValue) {
				maxValue = imageValue;
				//posOfCenter = { row, col };	// c++ 11�̻� (vs2012���� �ȵſ� �̤�)
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
	int nPixels = 2 * floor((radius - (float)pixelSpacing / 2.0) / pixelSpacing) + 1; // floor : ����. nPixels (19) : ����� ������� ����ũ�� ������ ���ԵǴ� �ִ� �ȼ� ���� (����, ���� ����)

	unsigned char** convMatrix = new unsigned char*[nPixels]; // 2���� convolution mask ���� �Ҵ�. nPixels�� ��ŭ�� �����͸� ������ ���� �ִ� �迭 (nPixels ��)

	for (int row = 0; row < nPixels; row++) {
	convMatrix[row] = new unsigned char[nPixels]; // �� ���� ������ ä���(�ʱ�ȭ)
	memset(convMatrix[row], 0, sizeof(unsigned char)*nPixels); // �޸� ������ 0���� �ʱ�ȭ
	}

	// 2-2. fill the convolutional matrix (19 x 19)
	pair<float, float> tempPos = { 0, 0 };
	float distFromCenter;

	for (int row = 0; row < nPixels; row++) {
	for (int col = 0; col < nPixels; col++) {
	tempPos.first = float(row) * pixelSpacing + pixelSpacing / 2 - (posOfCenter.first + 0.5) * pixelSpacing; // X��(row) ���� �Ÿ�(mm)
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


	// convMatrix ����? size -> nPixel or 512?
	Mat img(Size(nPixels, nPixels), CV_8UC1, convMatrix);
	imshow("img", img);
	waitKey(0);


	// 2-3. calculateConvolutionMatrix => getConvMatix & fillConvMatrix ȣ�� �� convMatrix return (nPixels x nPixels ũ��)
	*/

	// 2. check distFromCenter and calculate sum (mean) 
	float radius = 6.2;
	double pixelSpacing = m_ciData.getPixelSpacing(9); // 0.650391mm
	int nrPixels = floor((radius - (float)pixelSpacing / 2.0) / pixelSpacing); // 9. floor : ����
	int nPixels = 2 * nrPixels + 1; // 19

	pair<float, float> tempDist;
	float distFromCenter;
	short sumIntensity = 0;
	short meanIntensity;
	int cnt = 0; // 279

	// [230-9][246-9] center ��ǥ�� �ȼ��κ��� �Ÿ� ��� => 9 : nrPixels
	for (int row = posOfCenter.first - nrPixels; row < posOfCenter.first + nrPixels; row++) {
		for (int col = posOfCenter.second - nrPixels; col < posOfCenter.second + nrPixels; col++) {
			tempDist.first = float(row) * pixelSpacing + pixelSpacing / 2 - (posOfCenter.first + 0.5) * pixelSpacing; // X��(row) ���� �������� �Ÿ�(mm) => X_temp - X_center
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


vector<short> getVectorOfPixelsInROI(short* pusImage, unsigned char* pucMask, int nHeight, int nWidth) {

	vector<short> vectorOfOriPixels; // size : 3032
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			short imageValue = pusImage[index];

			// ROI ���ΰ� push (ROI�� ������ ��� pass)
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
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(minus<float>(), minimumValue)); // minus<T> : ���� ���� �Լ� ��ü (-) / bind2nd : 2��° �μ��� ������ �Լ� ��ü�� ��ȯ / transform : �ϰ� ����

	//get the range
	float range = (maximumValue - minimumValue) / nBins; // range : �� (width of a bin) => ***float�� ���� / �� ���� �ƴ϶� ��¥ "������" ����!!!***

	//divide every element of the matrix by the range
	transform(tempFloatVec.begin(), tempFloatVec.end(), tempFloatVec.begin(), bind2nd(divides<float>(), range));

	//replace 0 to 1 => ibsi ��ũ�� ��ŷ �κ�! (min - min = 0�� �ǹǷ� �ּ��� 1�� �ٲ���� �ڿ� ceil���� min value of each bin : 1 ����)
	replace(tempFloatVec.begin(), tempFloatVec.end(), 0, 1);

	//do rounding
	for (int i = 0; i < tempFloatVec.size(); i++) {
		tempFloatVec[i] = ceil(tempFloatVec[i]);
	}

	//type casting
	vector<unsigned short> vectorOfDiscretizedPixels(tempFloatVec.begin(), tempFloatVec.end()); // ����ȭ�� ���� ��� �ȼ����� ���� ����

	return vectorOfDiscretizedPixels;
}

vector<unsigned short> getVectorOfDiffGreyLevels(vector<unsigned short> vectorOfDiscretizedPixels) {

	vector<unsigned short> diffGreyLevels(vectorOfDiscretizedPixels.begin(), vectorOfDiscretizedPixels.end()); // 1~nbins ���� ������ ����ȭ�� �ȼ�����
	diffGreyLevels.erase(unique(diffGreyLevels.begin(), diffGreyLevels.end()), diffGreyLevels.end()); // size : 168 => 32 bins�� ������ �� ���� �� �� 5���� �ȼ������� �󵵼��� ����

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


// ***���� GUI üũ�ڽ� ����(true, false)�� ���� for�� �ȿ� if�� �־ �����������(<< or continue) ����
void defineIntenseFeatures(vector<string> &features) {
	features.push_back("mean");
	features.push_back("variance");
	/*
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
	*/
	
}

void extractIntenseData(vector<float> &intenseData, vector<float> intenseFeatures) {
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

void writeCSVFileIntensity(vector<float> intense, string outputFolder)
{
	string csvName = outputFolder + "_intensityHistogram.csv"; // Result/ICC_01_001_intensityHistogram.csv

	// remove existing file
	remove(csvName.c_str());

	char * name = new char[csvName.size() + 1];
	copy(csvName.begin(), csvName.end(), name);
	name[csvName.size()] = '\0';

	

	ofstream intenseCSV;
	intenseCSV.open(name, std::ios_base::app);
	vector<string> features;
	defineIntenseFeatures(features);

	//vector<float> intenseData;
	//extractIntenseData(intenseData, intense); // ***������ run()���� Ư¡���� intese vector�� push__back
	for (int i = 0; i< intense.size(); i++) {
		intenseCSV << "Intensity histogram" << "," << features[i] << ",";
		intenseCSV << intense[i];
		intenseCSV << "\n";
	}
	intenseCSV.close();
}

//template <typename T>
void CPlatform::run()
{
	// paramter //
	//double fParam1 = ui.doubleSpinBox->value();
	//double fParam2 = ui.doubleSpinBox_2->value();
	//double fParam3 = ui.doubleSpinBox_3->value();

	// mousePoint // (mouse ��ǥ�� �̹��� ũ�⿡ �µ��� �ڵ� ��ȯ�Ǿ ��ȯ) (ȭ��ũ�� x)
	// QPoint qpoint = m_ciImage->getMousePoint();	// Mouse ��ǥ �� �Ѱ��� �����ö�
	// QVector<QPoint> qpoints = m_ciImage->getMousePoints();	// �巡��

	// �迭 ���� //
	int nWidth = 0; // 512
	int nHeight = 0; // 512
	short* psImage = NULL;
	unsigned char* pucMask = NULL;
	
	// ROI �ִ� 10��° �̹��� copy
	m_ciData.copyImage(9, psImage, nWidth, nHeight); // DCM
	m_ciData.copyMask(9, pucMask, nWidth, nHeight); // label image

	
	// ***���� file�� copy***
	int nWidth2 = 0;
	int nHeight2 = 0;
	int nImageCnt = 0; // copyImages()���� nImageCnt ��� �� ��ȯ
	short** ppsImages = NULL;
	m_ciData.copyImages(0, ppsImages, nImageCnt, nWidth2, nHeight2); // �� �׷��� ���� dcm�� ���� ���� => copyImage ȣ��
	
	/*
	cout << "nWidth2 : " << nWidth2 << endl;
	cout << "nHeight2 : " << nHeight2 << endl;
	cout << "nImageCnt : " << nImageCnt << endl;
	*/


	/* [3.2] Local Intensity Features */
	// calcLocalIntensityPeak(pusImage, pucMask, nHeight, nWidth); // 3.2.1 Local Intensity Peak

	

	/* [3.4] Intensity Histogram Features */
	// 3.4.0 Ready for Feature Extraction => ���� class�� ��� ����, public ��� ����, ��� �Լ��� �����! (ex. class IntensityHistogram)
	int nBins = 32; // user input parameter
	vector<float> intense; // ��� ���� float
	vector<short> vectorOfOriPixels = getVectorOfPixelsInROI(psImage, pucMask, nHeight, nWidth);
	vector<unsigned short> vectorOfDiscretizedPixels = getVectorOfDiscretizedPixels_nBins(vectorOfOriPixels, nBins);
	vector<unsigned int> hist = getHistogram(vectorOfDiscretizedPixels);
	
	// 3.4.1 Mean discretised intensity
	float meanValue = calcMeanDiscretisedIntensity(vectorOfDiscretizedPixels); // 15.5894
	intense.push_back(meanValue);

	// 3.4.2 Discretised intensity variance
	float varValue = calcDiscretisedIntensityVariance(vectorOfDiscretizedPixels); // 29.5587
	intense.push_back(varValue);
	
	// 3.3.3 Discretised intensity skewness




	/* save and write extracted feature values */ 
	const static string outputFolder = string("Result/"); 
	string patientName = string("ICC_01_001"); // parameter
	string outputPath = outputFolder;
	outputPath.append(patientName); // "Result/ICC_01_001_intensityHistogram.csv"

	writeCSVFileIntensity(intense, outputPath);
	
	cout << "writeCSVFile done!" << endl;



	// ����� �޸𸮿� ���� //
	//m_ciImage->setImage(pucImage, nWidth, nHeight); // redraw call

	// �޸� �Ҹ� //
	SAFE_DELETE_ARRAY(psImage);
	SAFE_DELETE_ARRAY(pucMask);
	// 2���� �迭 �� ��, SAFE_DELETE_VOLUME(�迭��, 1���� �迭 ��)
	SAFE_DELETE_VOLUME(ppsImages, nImageCnt);
	
}

