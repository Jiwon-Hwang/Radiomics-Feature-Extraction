#include "platform.h"

using namespace std;
using namespace cv;

int FILTER_MODE; 

const static QString configPath = QString("Config/config.ini");
const static string outputFolder = string("Result/");


CPlatform::CPlatform(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);	// ui setting (by QT)
	setMouseTracking(true);
	setAcceptDrops(true);
	setStyles(); // ui setting (custom)

	init();
	createPopup();
	createProgressBar();

	setSignalSlot();
	setThread();
	loadSettings();
	m_ciData.setLogPath(outputFolder + "tempLog.txt"); // error log path

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

// set qt gui styles //
void CPlatform::setStyles() {

	// QLineEdit 입력 제약조건 - "int"만 허용
	QIntValidator *intValidator = new QIntValidator(0, 999999); //최소수, 최대수
	ui.lineEdit_x->setValidator(intValidator);
	ui.lineEdit_y->setValidator(intValidator);

}

// 초기화 //
void CPlatform::init()
{
	m_nActivatedFrameIdx = -1;
	m_ciImage = NULL;
	
	// mouse
	m_bLMouseDown = false;

}
void CPlatform::clear()
{
	if(m_ciImage != NULL) {
		delete m_ciImage;
		m_ciImage = NULL;
	}
}


// Widget 생성 //
void CPlatform::createPopup() {

	// create pop-up instance
	ppopup_Histogram = new popup_Histogram;
	ppopup_Histogram->setModal(true);

	ppopup_GLCM = new popup_GLCM;
	ppopup_GLCM->setModal(true);

	ppopup_Statistics = new popup_Statistics;
	ppopup_Statistics->setModal(true);

	/*
	ppopup_Histogram = new popup_Histogram;
	ppopup_Histogram->setModal(true);
	ppopup_Histogram = new popup_Histogram;
	ppopup_Histogram->setModal(true);
	ppopup_Histogram = new popup_Histogram;
	ppopup_Histogram->setModal(true);
	*/

}
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
	connect(ui.treeWidget_FileDirectory, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(showImage(QTreeWidgetItem*, int)));

	// filter (tab widget)
	QList<QRadioButton *> filterBox = ui.tabWidget_Filters->findChildren<QRadioButton *>();
	for (int i = 0; i < filterBox.size(); i++) {
		connect(filterBox[i], SIGNAL(clicked()), this, SLOT(setFilterMode()));
		connect(filterBox[i], SIGNAL(clicked()), this, SLOT(checkReadyToRun()));
	}

	// resampling (tab widget)
	connect(ui.lineEdit_x, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(checkReadyToRun()));
	connect(ui.lineEdit_y, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(checkReadyToRun()));

	// radiomics feature family (group box)
	QList<QCheckBox *> familyBox = ui.groupBox_Families->findChildren<QCheckBox *>();
	for (int i = 0; i < familyBox.size(); i++) {
		connect(familyBox[i], SIGNAL(clicked()), this, SLOT(setCheckedFamilyState()));
		connect(familyBox[i], SIGNAL(clicked()), this, SLOT(checkReadyToRun()));
	}

	// pop-up
	// Intensity Statistics (popup)
	connect(ppopup_Statistics->ui->checkBox_All, SIGNAL(clicked(bool)), this, SLOT(selectAll(bool))); // 'All' btn 눌렀을 때 나머지 체크박스 선택/해제
	QList<QCheckBox *> featureBox = ppopup_Statistics->ui->groupBox_Features->findChildren<QCheckBox *>(); // 나머지 체크박스들 눌렀을 때 'All' 선택/해제
	for (int i = 0; i < featureBox.size(); i++) {
		if (featureBox[i]->objectName() == "checkBox_All") continue;
		connect(featureBox[i], SIGNAL(clicked()), this, SLOT(checkFeatureBoxState()));
	}
	featureBox.clear();

	// Intensity Histogram (popup)
	connect(ppopup_Histogram->ui->checkBox_All, SIGNAL(clicked(bool)), this, SLOT(selectAll(bool))); 
	featureBox = ppopup_Histogram->ui->groupBox_Features->findChildren<QCheckBox *>(); 
	for (int i = 0; i < featureBox.size(); i++) {
		if (featureBox[i]->objectName() == "checkBox_All") continue;
		connect(featureBox[i], SIGNAL(clicked()), this, SLOT(checkFeatureBoxState()));
	}
	featureBox.clear();

	// GLCM (popup)
	connect(ppopup_GLCM->ui->checkBox_All, SIGNAL(clicked(bool)), this, SLOT(selectAll(bool)));
	featureBox = ppopup_GLCM->ui->groupBox_Features->findChildren<QCheckBox *>();
	for (int i = 0; i < featureBox.size(); i++) {
		if (featureBox[i]->objectName() == "checkBox_All") continue;
		connect(featureBox[i], SIGNAL(clicked()), this, SLOT(checkFeatureBoxState()));
	}
	featureBox.clear();

}
void CPlatform::showPopUp(QObject* sender) {
	// for loop으로 대체 가능 (for auto, break)
	if (sender == ui.checkBox_Statistics) {
		ppopup_Statistics->show();
	}

	if (sender == ui.checkBox_Histogram) {
		ppopup_Histogram->show();
	}

	if (sender == ui.checkBox_Intensity) {
		//ppopup_Histogram->show();
	}

	if (sender == ui.checkBox_Morph) {
		//ppopup_Histogram->show();
	}

	if (sender == ui.checkBox_GLCM) {
		ppopup_GLCM->show();
	}

	if (sender == ui.checkBox_GLRLM) {
		//ppopup_Histogram->show();
	}

}
void CPlatform::setProgressBarValue(int nCurrentIdx, int nMaximumIdx)
{
	float fValue = (float)(nCurrentIdx + 1) / (float)nMaximumIdx * 100;

	if (nCurrentIdx == 1) {
		progressBar->setVisible(true);
	}

	progressBar->setValue(fValue);

	if ((nCurrentIdx + 1) >= nMaximumIdx) {
		progressBar->setValue(0);
		progressBar->setVisible(false);
	}
}

// load and save GUI state //
void CPlatform::loadSettings() {

	if (QFile::exists(configPath)) {

		QSettings settings(configPath, QSettings::IniFormat);

		// filter 
		settings.beginGroup("filter");
		
		QList<QRadioButton *> filterBox = ui.tabWidget_Filters->findChildren<QRadioButton *>();
		for (int i = 0; i < filterBox.size(); i++) {
			filterBox[i]->setChecked(settings.value(filterBox[i]->objectName(), false /* default value */).toBool());
		}
		
		settings.endGroup();


		// resampling
		settings.beginGroup("resampling");

		ui.lineEdit_x->setText(settings.value(ui.lineEdit_x->objectName()).toString());
		ui.lineEdit_y->setText(settings.value(ui.lineEdit_y->objectName()).toString());

		settings.endGroup();
		
				
		// radiomics feature family 
		settings.beginGroup("feature_family");
		
		QList<QCheckBox *> familyBox = ui.groupBox_Families->findChildren<QCheckBox *>();
		for (int i = 0; i < familyBox.size(); i++) {
			familyBox[i]->setChecked(settings.value(familyBox[i]->objectName(), false).toBool());
			initIsActivatedFamily(i); //get<i>(featureFamily).isActivatedFamily = true;
		}
				
		settings.endGroup();

		
		// Intensity Histogram (popup) 
		settings.beginGroup("popup_Statistics");

		int nfeatures_Statistics = IntensityStatistics::FEATURE_COUNT;
		for (int i = 0; i < nfeatures_Statistics + 1; i++) {
			ppopup_Statistics->filterGroup->button(i)->setChecked(settings.value(ppopup_Statistics->filterGroup->button(i)->objectName(), false).toBool());
		}

		settings.endGroup();


		settings.beginGroup("popup_Histogram");

		//int nfeatures_Histogram = ppopup_Histogram->ui->groupBox_Features->findChildren<QCheckBox *>().size();
		int nfeatures_Histogram = IntensityHistogram::FEATURE_COUNT;
		for (int i = 0; i < nfeatures_Histogram + 1; i++) {
			ppopup_Histogram->filterGroup->button(i)->setChecked(settings.value(ppopup_Histogram->filterGroup->button(i)->objectName(), false).toBool());
		}
		
		ppopup_Histogram->ui->comboBox_nBins->setCurrentText(settings.value(ppopup_Histogram->ui->comboBox_nBins->objectName(), "32").toString());

		settings.endGroup();


		// GLCM (popup) 
		settings.beginGroup("popup_GLCM");

		//int nfeatures_GLCM = ppopup_GLCM->ui->groupBox_Features->findChildren<QCheckBox *>().size();
		int nfeatures_GLCM = GLCM::FEATURE_COUNT;
		for (int i = 0; i < nfeatures_GLCM + 1; i++) {
			ppopup_GLCM->filterGroup->button(i)->setChecked(settings.value(ppopup_GLCM->filterGroup->button(i)->objectName(), false).toBool());
		}

		ppopup_GLCM->ui->comboBox_nBins->setCurrentText(settings.value(ppopup_GLCM->ui->comboBox_nBins->objectName(), "32").toString());

		settings.endGroup();

		ui.pushButton_run->setEnabled(true); // enable run button

	}
	
}
void CPlatform::saveSettings() {

	QSettings settings(configPath, QSettings::IniFormat);

	// filter (tab widget)
	settings.beginGroup("filter");

	QList<QRadioButton *> filterBox = ui.tabWidget_Filters->findChildren<QRadioButton *>();
	for (int i = 0; i < filterBox.size(); i++) {
		settings.setValue(filterBox[i]->objectName(), QVariant(filterBox[i]->isChecked()));
	}

	settings.endGroup();


	// resampling (tab widget)
	settings.beginGroup("resampling");

	settings.setValue(ui.lineEdit_x->objectName(), ui.lineEdit_x->text());
	settings.setValue(ui.lineEdit_y->objectName(), ui.lineEdit_y->text());

	settings.endGroup();
	

	// radiomics feature family 
	settings.beginGroup("feature_family");

	QList<QCheckBox *> familyBox = ui.groupBox_Families->findChildren<QCheckBox *>();
	for (int i = 0; i < familyBox.size(); i++) {
		settings.setValue(familyBox[i]->objectName(), QVariant(familyBox[i]->isChecked()));
	}

	settings.endGroup();


	// popup - save settings to config and to object's member variable

	// Intensity Statistics (popup)
	settings.beginGroup("popup_Statistics");

	intenseStat.isCheckedFeature.assign(IntensityStatistics::FEATURE_COUNT, false);
	intenseStat.nCheckedFeatures = 0;

	int nfeatures_Statistics = IntensityStatistics::FEATURE_COUNT;
	for (int i = 0; i < nfeatures_Statistics + 1; i++) {
		settings.setValue(ppopup_Statistics->filterGroup->button(i)->objectName(), QVariant(ppopup_Statistics->filterGroup->button(i)->isChecked()));
		if (i == nfeatures_Statistics) break;
		intenseStat.isCheckedFeature[i] = ppopup_Statistics->filterGroup->button(i)->isChecked();
		if (intenseStat.isCheckedFeature[i] == true) intenseStat.nCheckedFeatures++;
	}

	settings.endGroup();


	// Intensity Histogram (popup)
	settings.beginGroup("popup_Histogram");

	intenseHisto.isCheckedFeature.assign(IntensityHistogram::FEATURE_COUNT, false);
	intenseHisto.nCheckedFeatures = 0;

	//int nfeatures_Histogram = ppopup_Histogram->ui->groupBox_Features->findChildren<QCheckBox *>().size();
	int nfeatures_Histogram = IntensityHistogram::FEATURE_COUNT;
	for (int i = 0; i < nfeatures_Histogram + 1; i++) {
		settings.setValue(ppopup_Histogram->filterGroup->button(i)->objectName(), QVariant(ppopup_Histogram->filterGroup->button(i)->isChecked()));
		if (i == nfeatures_Histogram) break; // exclude 'All' btn
		intenseHisto.isCheckedFeature[i] = ppopup_Histogram->filterGroup->button(i)->isChecked();
		if (intenseHisto.isCheckedFeature[i] == true) intenseHisto.nCheckedFeatures++;
	}

	settings.setValue(ppopup_Histogram->ui->comboBox_nBins->objectName(), ppopup_Histogram->ui->comboBox_nBins->currentText());
	intenseHisto.nBins = ppopup_Histogram->ui->comboBox_nBins->currentText().toInt();

	settings.endGroup();


	// Local Intensity (popup)

	// Morphological (popup)

	// GLCM (popup)
	settings.beginGroup("popup_GLCM");

	glcm.isCheckedFeature.assign(GLCM::FEATURE_COUNT, false);
	glcm.nCheckedFeatures = 0;

	//int nfeatures_GLCM = ppopup_GLCM->ui->groupBox_Features->findChildren<QCheckBox *>().size();
	int nfeatures_GLCM = GLCM::FEATURE_COUNT;
	for (int i = 0; i < nfeatures_GLCM; i++) {
		settings.setValue(ppopup_GLCM->filterGroup->button(i)->objectName(), QVariant(ppopup_GLCM->filterGroup->button(i)->isChecked()));
		if (i == nfeatures_Histogram) break; // exclude 'All' btn
		glcm.isCheckedFeature[i] = ppopup_GLCM->filterGroup->button(i)->isChecked();
		if (glcm.isCheckedFeature[i] == true) glcm.nCheckedFeatures++;
	}

	settings.setValue(ppopup_GLCM->ui->comboBox_nBins->objectName(), ppopup_GLCM->ui->comboBox_nBins->currentText());
	glcm.nBins = ppopup_GLCM->ui->comboBox_nBins->currentText().toInt(); // nBins == sizeMatrix

	settings.endGroup();

	// GLRLM (popup)



}
void CPlatform::initIsActivatedFamily(int FAMILY_IDX)
{

	QSettings settings(configPath, QSettings::IniFormat);
	settings.beginGroup("feature_family"); // 꼭 써줘야 함!

	switch (FAMILY_IDX) 
	{
		case E_INTENSESTAT:
			if (settings.value(ui.checkBox_Statistics->objectName()).toBool() == true) {
				intenseStat.isActivatedFamily = true;
			}
			break;

		case E_INTENSEHISTO:
			if (settings.value(ui.checkBox_Histogram->objectName()).toBool() == true) {
				intenseHisto.isActivatedFamily = true;
			}
			break;

		case E_LOCALINTENSE:
			if (settings.value(ui.checkBox_Intensity->objectName()).toBool() == true) {
				localIntense.isActivatedFamily = true;
			}
			break;

		case E_MORPHOLOGY:
			if (settings.value(ui.checkBox_Morph->objectName()).toBool() == true) {
				morphology.isActivatedFamily = true;
			}
			break;

		case E_GLCM:
			if (settings.value(ui.checkBox_GLCM->objectName()).toBool() == true) {
				glcm.isActivatedFamily = true;
			}
			break;

		case E_GLRLM:
			if (settings.value(ui.checkBox_GLRLM->objectName()).toBool() == true) {
				glrlm.isActivatedFamily = true;
			}
			break;

		default:
			break;
	}

	settings.endGroup();

}
bool CPlatform::checkReadyToRun() {
	
	if (ui.lineEdit_x->text().toInt() != 0 && ui.lineEdit_y->text().toInt() != 0) {

		QList<QRadioButton *> filterBox = ui.tabWidget_Filters->findChildren<QRadioButton *>();
		QList<QCheckBox *> familyBox = ui.groupBox_Families->findChildren<QCheckBox *>();

		for (int i = 0; i < filterBox.size(); i++)
		{
			if (filterBox[i]->isChecked())
			{
				for (int j = 0; j < familyBox.size(); j++)
				{
					if (familyBox[j]->isChecked())
					{
						ui.pushButton_run->setEnabled(true); // enable run button
						return true;
					}
				}
				ui.pushButton_run->setEnabled(false); // disable run button
				break;
			}
		}
	}

	ui.pushButton_run->setEnabled(false); // disable run button
	return false;
}
void CPlatform::selectAll(bool checked) {

	QObject* obj = sender(); // 'All' checkBox
	QList<QCheckBox *> featureBox = obj->parent()->findChildren<QCheckBox *>();

	for (int i = 0; i < featureBox.size(); i++) {
		featureBox[i]->setChecked(checked); // checked, unchecked에 따라 전체 선택, 전체 해제
	}
}
void CPlatform::checkFeatureBoxState() {
	
	QObject* obj = sender(); // 'the other' checkBoxes
	QList<QCheckBox *> featureBox = obj->parent()->findChildren<QCheckBox *>();
	
	bool isAllChecked = true;
	for (int i = 0; i < featureBox.size(); i++) {
		if (!featureBox[i]->isChecked() && featureBox[i]->objectName() != "checkBox_All") {
			isAllChecked = false;
			break;
		}
	}
	
	obj->parent()->findChild<QCheckBox *>("checkBox_All")->setChecked(isAllChecked);

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
void CPlatform::dropEvent(QDropEvent * event)
{
	QStringList list;

	foreach(const QUrl &url, event->mimeData()->urls()) {
		list.push_back(url.toLocalFile());
	}

	readImage(list); // *** image, mask 따로 load 시 죽는 error***
}

// keyboard event //
void CPlatform::keyPressEvent(QKeyEvent* event)
{
	switch(event->key()) {

		case Qt::Key_Up:
		{
			int index = m_nActivatedFrameIdx - 1;
			QTreeWidgetItemIterator it2(ui.treeWidget_FileDirectory);
			while (*it2) {
				int nSeriesIdx = (*it2)->text(1).toInt();
				int nImageIdx = (*it2)->text(2).toInt();
				int nSliceIdx = m_ciData.convertToSliceIdx(nSeriesIdx, nImageIdx);
				if (((*it2)->text(1) != NULL) && (index == nSliceIdx)) {
					showImage(*it2, 0);

					QTreeWidgetItemIterator it(ui.treeWidget_FileDirectory);
					while (*it) {
						(*it)->setSelected(false); // 화면에 보이는 아이템 값을 트리 위젯에서 색칠
						++it;
					}
					(*it2)->setSelected(true);
					ui.treeWidget_FileDirectory->scrollToItem(*it2); // 해당 아이템이 트리 위젯 화면에 들어오도록
					break;
				}
				++it2;
			}
		}
		break;

		case Qt::Key_Down:
		{
			int index = m_nActivatedFrameIdx + 1;
			QTreeWidgetItemIterator it2(ui.treeWidget_FileDirectory);
			while (*it2) {
				int nSeriesIdx = (*it2)->text(1).toInt();
				int nImageIdx = (*it2)->text(2).toInt();
				int nSliceIdx = m_ciData.convertToSliceIdx(nSeriesIdx, nImageIdx);
				if (((*it2)->text(1) != NULL) && (index == nSliceIdx)) {
					showImage(*it2, 0);

					QTreeWidgetItemIterator it(ui.treeWidget_FileDirectory);
					while (*it) {
						(*it)->setSelected(false);
						++it;
					}
					(*it2)->setSelected(true);
					ui.treeWidget_FileDirectory->scrollToItem(*it2);
					break;
				}
				++it2;
			}
		}
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

// mouse wheel event //
void CPlatform::wheelEvent(QWheelEvent* event)
{
	if (m_bLMouseDown) {
		return;
	}

	Q_UNUSED(event);
	int index = m_nActivatedFrameIdx;

	if (event->delta() > 0) {
		index = index - 1;
	}
	else {
		index = index + 1;
	}

	//selectedFrame(index);
	
	QTreeWidgetItemIterator it2(ui.treeWidget_FileDirectory);
	while(*it2) {
		int nSeriesIdx = (*it2)->text(1).toInt();
		int nImageIdx = (*it2)->text(2).toInt();
		int nSliceIdx = m_ciData.convertToSliceIdx(nSeriesIdx, nImageIdx);
		if (((*it2)->text(1) != NULL) && (index == nSliceIdx)) {
			showImage(*it2, 0);

			QTreeWidgetItemIterator it(ui.treeWidget_FileDirectory);
			while(*it) {
				(*it)->setSelected(false);
				++it;
			}
			(*it2)->setSelected(true);
			ui.treeWidget_FileDirectory->scrollToItem(*it2);
			break;
		}
		++it2;
	}
}


// scan data list and emit signal - set signal slot //
void CPlatform::setThread() {
	m_ciData.moveToThread(&m_thread);
	m_ciData.setQThread(&m_thread);

	connect(&m_thread, SIGNAL(started()), &m_ciData, SLOT(slotReadImage()));
	connect(&m_ciData, SIGNAL(signalDataScanFinish()), this, SLOT(slotDataScanFinish()));
	connect(&m_ciData, SIGNAL(signalDataProgress(int, int)), this, SLOT(slotDataProgress(int, int)));
}
void CPlatform::slotDataScanFinish() {

	// 첫번째 Series load, 첫번째 Image show
	m_ciData.loadImages(0); 
	showImage(0);

	// 하단 스크롤바 설정
	int nStartFrameIdx = 0;
	int nEndFrameIdx = m_ciData.getSeries(0)->getImageCount() - 1;
	ui.horizontalScrollBar->setMaximum(nEndFrameIdx - nStartFrameIdx);
	ui.horizontalScrollBar->setMinimum(0);

	// add items in tree widget (receiveTreeState())
	ui.treeWidget_FileDirectory->clear(); // clear tree widget
	int nSeriesCnt = m_ciData.getSeriesCount();
	for (int i = 0; i < nSeriesCnt; i++) {
		addFileDirectoryItem(i);
	}

}
void CPlatform::slotDataProgress(int nCurrentIdx, int nMaximumIdx) {
	setProgressBarValue(nCurrentIdx, nMaximumIdx);
}

// open, load, Image (tree widget) //
void CPlatform::readImage(QStringList list)
{
	//m_ciData.clear(); // image, mask 따로 load 시 죽는 error

	// QThread로 병렬 처리 (2가지 thread) //
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

	std::vector<std::string> convertedFileList;
	convertedFileList.reserve(fileList.size());

	for (int i = 0, ni = fileList.size(); i<ni; i++) {
		char cInputPath[1024] = { 0 };
		QString path = fileList.at(i).toUtf8().constData();
		QTextCodec* c = QTextCodec::codecForLocale();
		QByteArray b = c->fromUnicode(path);
		std::memcpy(cInputPath, b.data(), b.size() + 1);

		convertedFileList.push_back(cInputPath);
	}

	m_ciData.readFileList = convertedFileList;
	m_thread.start();
}
void CPlatform::addFileDirectoryItem(int seriesIdx) {

	CSeries* pCiSeries = m_ciData.getSeries(seriesIdx);

	QString patientName = pCiSeries->m_sPatientName.c_str();
	QString studyName = pCiSeries->m_sStudyName.c_str();
	QString seriesName = pCiSeries->m_sSeriesName.c_str();

	QList<QTreeWidgetItem*> list = ui.treeWidget_FileDirectory->findItems(patientName, Qt::MatchExactly | Qt::MatchRecursive, 0);

	if (list.size() == 0) {
		// 이전에 중복되는 환자가 없었을 경우 ("100") => patient, study, series, image 새로 추가
		QTreeWidgetItem* row_patientName = new QTreeWidgetItem(ui.treeWidget_FileDirectory); // "100"
		row_patientName->setText(0, patientName);
		row_patientName->setIcon(0, QIcon(QPixmap("Resources/folder.png")));

		QTreeWidgetItem* row_studyName = new QTreeWidgetItem(row_patientName); // "CT"
		row_studyName->setText(0, studyName);
		row_studyName->setIcon(0, QIcon(QPixmap("Resources/folder.png")));

		QTreeWidgetItem* row_seriesName = new QTreeWidgetItem(row_studyName); // "ap"
		row_seriesName->setText(0, seriesName);
		row_seriesName->setIcon(0, QIcon(QPixmap("Resources/folder.png")));


		int nImages = pCiSeries->getImageCount();
		for (int i = 0; i < nImages; i++) {
			QTreeWidgetItem* row_image = new QTreeWidgetItem(row_seriesName);
			row_image->setIcon(0, QIcon(QPixmap("Resources/dcm.png")));
			QString imageName = pCiSeries->getImage(i)->getImageName().c_str();
			row_image->setText(0, imageName);
			row_image->setText(1, QString::number(seriesIdx));
			row_image->setText(2, QString::number(i));
			ui.treeWidget_FileDirectory->setCurrentItem(row_image); // 실제 아이템 추가 부분
		}
	}

	else {
		// 중복되는 환자명이 있을 경우 ("100") 
		QTreeWidgetItem* row_patientName = list.at(0);
		bool isDup = false;
		
		for (int i = 0; i < row_patientName->childCount(); i++) {
			QTreeWidgetItem* row_studyName = row_patientName->child(i);
			if (row_studyName->text(0).compare(studyName) == 0) {
				// 중복되는 study명이 있을 경우 ("CT"), series명 검사
				isDup = true;
				for (int j = 0; j < row_studyName->childCount(); j++) {
					QTreeWidgetItem* row_seriesName = row_studyName->child(j);
					if (row_seriesName->text(0).compare(seriesName) == 0) {
						// 중복되는 series명이 있을 경우 ("ap"), do nothing => series마다 함수 들어오므로 그럴 경우 x
						return;
					}
				}

				// 중복되는 study명은 있고 중복되는 series명은 없는 경우 => series, image 새로 추가
				QTreeWidgetItem* row_seriesName = new QTreeWidgetItem(row_studyName); // "ap"
				row_seriesName->setText(0, seriesName);
				row_seriesName->setIcon(0, QIcon(QPixmap("Resources/folder.png")));

				int nImages = pCiSeries->getImageCount();
				for (int i = 0; i < nImages; i++) {
					QTreeWidgetItem* row_image = new QTreeWidgetItem(row_seriesName);
					row_image->setIcon(0, QIcon(QPixmap("Resources/dcm.png")));
					QString imageName = pCiSeries->getImage(i)->getImageName().c_str();
					row_image->setText(0, imageName);
					row_image->setText(1, QString::number(seriesIdx));
					row_image->setText(2, QString::number(i));
					ui.treeWidget_FileDirectory->setCurrentItem(row_image); // 실제 아이템 추가 부분
				}

			}
		}
		
		if (isDup == false) {
			// 중복되는 환자명은 있고 중복되는 study명은 없는 경우 => study, series, image 새로 추가
			QTreeWidgetItem* row_studyName = new QTreeWidgetItem(row_patientName); // "CT"
			row_studyName->setText(0, studyName);
			row_studyName->setIcon(0, QIcon(QPixmap("Resources/folder.png")));

			QTreeWidgetItem* row_seriesName = new QTreeWidgetItem(row_studyName); // "ap"
			row_seriesName->setText(0, seriesName);
			row_seriesName->setIcon(0, QIcon(QPixmap("Resources/folder.png")));

			int nImages = pCiSeries->getImageCount();
			for (int i = 0; i < nImages; i++) {
				QTreeWidgetItem* row_image = new QTreeWidgetItem(row_seriesName);
				row_image->setIcon(0, QIcon(QPixmap("Resources/dcm.png")));
				QString imageName = pCiSeries->getImage(i)->getImageName().c_str();
				row_image->setText(0, imageName);
				row_image->setText(1, QString::number(seriesIdx));
				row_image->setText(2, QString::number(i));
				ui.treeWidget_FileDirectory->setCurrentItem(row_image); // 실제 아이템 추가 부분
			}
		}
	}

}
void CPlatform::showImage(int nSliceIdx)
{
	m_nActivatedFrameIdx = nSliceIdx;
	int nWidth = m_ciData.getWidth(nSliceIdx);
	int nHeight = m_ciData.getHeight(nSliceIdx);
	

	// 처음 Data를 로드하는 경우 (***slotDataScanFinish()에서 showImage(0) 일때만 if문 들어감***)
	// CImage* m_ciImage => image pointer
	if(m_ciImage == NULL) {
		m_ciImage = new CImageView;

		m_ciImage->init(this); // m_nImageWidth, m_nImageHeight 셋팅 (512, 512)
		m_ciImage->setMinimumWidth(200);
		m_ciImage->setMinimumHeight(200);
		m_ciImage->setImageScreenSize(562, 562); // ***여기서 redraw 함수 호출!*** => overlay!

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
	// tree view 더블 클릭 시 show image (slot 함수)
	/*
	if(item->text(1) != 0 && item->text(2) != 0) {
		int nSeriesIdx = item->text(1).toInt();
		int nImageIdx = item->text(2).toInt();
		int nSliceIdx = m_ciData.convertToSliceIdx(nSeriesIdx, nImageIdx); 
		showImage(nSliceIdx);
	}
	*/
	int nSeriesIdx = item->text(1).toInt();
	int nImageIdx = item->text(2).toInt();
	int nSliceIdx = m_ciData.convertToSliceIdx(nSeriesIdx, nImageIdx);

	// 하단 스크롤바 설정 (해당 series에 포함된 slice들에 맞게 변경)
	int nStartFrameIdx = nSliceIdx - nImageIdx; // 시리즈 시작 idx (전체 기준)
	int nEndFrameIdx = nStartFrameIdx + m_ciData.getSeries(nSeriesIdx)->getImageCount() - 1; // 시리즈 끝 idx (전체 기준)
	ui.horizontalScrollBar->setMaximum(nEndFrameIdx);
	ui.horizontalScrollBar->setMinimum(nStartFrameIdx);

	m_ciData.loadImages(nSliceIdx);
	showImage(nSliceIdx);

}

// scroll //
void CPlatform::scrollChangeImage(int nValue)
{
	showImage(nValue);
}


// Algorithms ------------------------------------------------------------------------------------------------------------------------------

// ROI slice check //
bool isEmptyMask(unsigned char* pucMask, int nWidth, int nHeight, float &nPixelsInMask) {

	Mat mask(nWidth, nHeight, CV_8UC1, pucMask);

	nPixelsInMask = countNonZero(mask);

	return nPixelsInMask < 1 ? true : false;
}


// filter //
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


// pre-processing //
vector<int> getImageSizeInterpolated(vector<int> oriImageSize, float pixelSpacingX, float pixelSpacingY, vector<int> outputSpacing) {
	
#ifdef _WIN32
	vector<int> newImageSize = { 0, 0 }; // nWidth, nHeight
#else
	vector<int> newImageSize(2, 0);
#endif // _WIN32

	//calculate now the new image size
	newImageSize[0] = ceil((float)oriImageSize[0] * pixelSpacingX / float(outputSpacing[0]));
	newImageSize[1] = ceil((float)oriImageSize[1] * pixelSpacingY / float(outputSpacing[1]));

	return newImageSize;

}
void CPlatform::resampling(short* psImage, unsigned char* pucMask, int &nWidth, int &nHeight, int nPixelsInMask, float pixelSpacingX, float pixelSpacingY, Mat &image_resampled, Mat &mask_resampled) {

	Mat image(nWidth, nHeight, CV_16SC1, psImage);
	Mat mask(nWidth, nHeight, CV_8UC1, pucMask);

	if (nPixelsInMask < 5) {
		cout << "nPixelsInMask < 5 !!" << endl;
		//fillCSVwithNANs();
	}
	else {
		// down or upsample the image
		vector<int> oriImageSize = { nWidth, nHeight };
		vector<int> outputSpacing = { ui.lineEdit_x->text().toInt(), ui.lineEdit_y->text().toInt() }; // width, height
		vector<int> newImageSize = getImageSizeInterpolated(oriImageSize, pixelSpacingX, pixelSpacingY, outputSpacing); // width, height
		nWidth = newImageSize[0];
		nHeight = newImageSize[1];

		if (float(outputSpacing[0]) * float(outputSpacing[1]) < pixelSpacingX * pixelSpacingY) {
			// up-sampling
			cv::resize(image, image_resampled, Size(newImageSize[0], newImageSize[1]), INTER_NEAREST); 
			cv::resize(mask, mask_resampled, Size(newImageSize[0], newImageSize[1]), INTER_LINEAR);
		}
		else {
			// down-sampling
			cv::resize(image, image_resampled, Size(newImageSize[0], newImageSize[1]), INTER_AREA); 
			cv::resize(mask, mask_resampled, Size(newImageSize[0], newImageSize[1]), INTER_LINEAR);
		}
		
		// convert mask(Mat) values to 0, 1 (necessary after interpolation)
		// 1. 정규화(normalize) : 0~255 or etc -> 0~1로
		cv::normalize(mask_resampled, mask_resampled, 0, 1, NORM_MINMAX, CV_8UC1);
		// 2. 이진화(threshold) : 0~1 -> 0, 1로
		cv::threshold(mask_resampled, mask_resampled, 0.5, 1, THRESH_BINARY);

		/*
		//get the region of the mask
		RegionType boundingBoxRegion = getBoundingBoxMask(maskFiltered);
		itk::Size<3> regionSize = boundingBoxRegion.GetSize();
		//shrink image and mask to the mask region
		imageFiltered = getImageMasked(imageFiltered, boundingBoxRegion);
		maskFiltered = getImageMasked(maskFiltered, boundingBoxRegion);

		//for ontology table
		const typename ImageType::SpacingType& spacingVoxelDim = imageFiltered->GetSpacing();
		float voxelSize[3];
		for (int i = 0; i < 3; i++) {
			voxelSize[i] = (float)spacingVoxelDim[i];
		}
		Image<float, 3> imageVoxelDim(10, 10, 10);
		imageVoxelDim.createOntologyVoxelDimensionTable(config, voxelSize);
		mask = nullptr;
		image = nullptr;
		//calculateFeatures(imageFiltered, maskFiltered, config);
		*/
	}
	
}
void resegmentation(short* psImage, unsigned char* pucMask, int nWidth, int nHeight, Mat &mask_resegmented) {
	
	// outlier filtering (+-3sd) //
	short min_resegmented;
	short max_resegmented;

	// get vector of ori pixels in ROI mask
	vector<short> vectorOfPixels; 
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			short imageValue = psImage[index];

			if (maskValue >(unsigned char)0) {
				vectorOfPixels.push_back(imageValue);
			}
		}
	}

	// calculate mean and std
	short mean = accumulate(vectorOfPixels.begin(), vectorOfPixels.end(), 0.0) / vectorOfPixels.size();
	transform(vectorOfPixels.begin(), vectorOfPixels.end(), vectorOfPixels.begin(), bind2nd(minus<float>(), mean));
	short var = inner_product(vectorOfPixels.begin(), vectorOfPixels.end(), vectorOfPixels.begin(), 0.0) / vectorOfPixels.size();
	short std = sqrt(var);

	// calcalate outlier filtered min, max
	min_resegmented = mean - 3 * std;
	max_resegmented = mean + 3 * std;

	// get resegmented mask
	Mat mask(nWidth, nHeight, CV_8UC1, pucMask);    // 얕은 복사 (같은 메모리 참조)
	mask_resegmented = mask.clone();				// 깊은 복사 (다른 메모리 참조) => return
	for (int row = 0; row < nHeight; row++) {
		for (int col = 0; col < nWidth; col++) {
			int index = row * nWidth + col;
			unsigned char maskValue = pucMask[index];
			short imageValue = psImage[index];

			if (maskValue > (unsigned char)0) {
				mask_resegmented.data[index] = min_resegmented <= imageValue && imageValue <= max_resegmented ? maskValue : (unsigned char)0;
			}
		}
	}

}


// feature extraction //
void CPlatform::setCheckedFamilyState() { // check box 클릭될 때마다(시그널) 호출되는 SLOT 함수
	// for loop으로 대체 가능 (for auto, break)
	QObject* obj = sender();

	if (obj == ui.checkBox_Statistics) {
		intenseStat.isActivatedFamily = !intenseStat.isActivatedFamily;

		if (intenseStat.isActivatedFamily == true) {
			showPopUp(obj); // pop-up
		}
	}

	if (obj == ui.checkBox_Histogram) {
		intenseHisto.isActivatedFamily = !intenseHisto.isActivatedFamily;

		if (intenseHisto.isActivatedFamily == true) {
			showPopUp(obj); // pop-up
		}
	}

	if (obj == ui.checkBox_Intensity) {

	}

	if (obj == ui.checkBox_Morph) {

	}

	if (obj == ui.checkBox_GLCM) {
		glcm.isActivatedFamily = !glcm.isActivatedFamily;

		if (glcm.isActivatedFamily == true) {
			showPopUp(obj); // pop-up
		}
	}

	if (obj == ui.checkBox_GLRLM) {

	}

}
void CPlatform::featureExtraction(short* psImage, unsigned char* pucMask, int nHeight, int nWidth) {
	// for loop으로 대체 가능 (for auto, break)
	if (intenseStat.isActivatedFamily) {
		intenseStat.featureExtraction(psImage, pucMask, nHeight, nWidth);
	}

	if (intenseHisto.isActivatedFamily) {
		intenseHisto.featureExtraction(psImage, pucMask, nHeight, nWidth);
	}

	if (ui.checkBox_Intensity->isChecked()) {
		//short localIntensityPeak = calcLocalIntensityPeak(psImage, pucMask, nHeight, nWidth);
		//localIntense.push_back(localIntensityPeak);
	}

	if (ui.checkBox_Morph->isChecked()) {

	}

	if (glcm.isActivatedFamily) {
		glcm.featureExtraction(psImage, pucMask, nHeight, nWidth);
	}

	if (glrlm.isActivatedFamily) {
		//glrlm.featureExtraction(psImage, pucMask, nHeight, nWidth);
	}
}

void CPlatform::averageAllSlices() {
	// for loop으로 대체 가능 (for auto, break)
	if (intenseStat.isActivatedFamily) {
		intenseStat.averageAllValues();
	}

	if (intenseHisto.isActivatedFamily) {
		intenseHisto.averageAllValues();
	}

	if (ui.checkBox_Intensity->isChecked()) {
		
	}

	if (ui.checkBox_Morph->isChecked()) {

	}

	if (glcm.isActivatedFamily) {
		glcm.averageAllValues();
	}

	if (ui.checkBox_GLRLM->isChecked()) {

	}

}

// [ 3.2 Local Intensity Features ]
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


// preset csv file // 
string getCurrentTime()
{
	time_t timer = time(NULL);
	struct tm* t = localtime(&timer);

	ostringstream os;
	os << put_time(t, "%Y%m%d %H%M%S");

	return os.str();
}
void CPlatform::presetCSVFile(string csvName) {

	ofstream resultCSV(csvName); // 파일이 없으면 새로 생성, 있으면 기존 내용 지우고 새로 작성 (remove 포함)

	// write family name 
	resultCSV << " " << "," << " " << "," << " " << ",";

	if (intenseStat.isActivatedFamily) {
		for (int i = 0; i < intenseStat.nCheckedFeatures; i++) {
			resultCSV << "Statistics" << ",";
		}
	}
	if (intenseHisto.isActivatedFamily) {
		for (int i = 0; i < intenseHisto.nCheckedFeatures; i++) {
			resultCSV << "Histogram" << ",";
		}
	}
	if (glcm.isActivatedFamily) {
		for (int i = 0; i < glcm.nCheckedFeatures; i++) {
			resultCSV << "GLCM" << ",";
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
	if (glrlm.isActivatedFamily) {
	for (int i = 0; i < glrlm.nCheckedFeatures; i++) {
	resultCSV << "GLRLM" << ",";
	}
	}
	*/

	resultCSV << "\n";

	// write feature name 
	resultCSV << " " << "," << " " << "," << " " << ",";

	if (intenseStat.isActivatedFamily) {
		vector<string> featureNames(IntensityStatistics::FEATURE_COUNT, "");
		intenseStat.defineFeatureNames(featureNames); 
		for (int i = 0; i < featureNames.size(); i++) {
			if (intenseStat.isCheckedFeature[i]) {
				resultCSV << featureNames[i] << ",";
			}
		}
	}

	if (intenseHisto.isActivatedFamily) {
		vector<string> featureNames(IntensityHistogram::FEATURE_COUNT, "");
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

	if (glcm.isActivatedFamily) {
		vector<string> featureNames(GLCM::FEATURE_COUNT, "");
		glcm.defineFeatureNames(featureNames); // 일단 전체 feature name들 get
		for (int i = 0; i < featureNames.size(); i++) {
			if (glcm.isCheckedFeature[i]) {
				resultCSV << featureNames[i] << ",";
			}
		}
	}
	if (ui.checkBox_GLRLM->isChecked()) {

	}

	resultCSV.close();

}


// save result //
void writeCSVCheckedValue(vector<float> extractedValues, string csvName) // 모든 class 공통
{
	ofstream resultCSV(csvName, std::ios_base::app);

	for (int i = 0; i< extractedValues.size(); i++) {
		/*
		// 속도 문제로 사용 x
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

	resultCSV << patientName << "," << studyName << "," << seriesName << ",";

	resultCSV.close();
}
void CPlatform::writeCSVFeatureValue(string csvName) {

	if (intenseStat.isActivatedFamily) {
		writeCSVCheckedValue(intenseStat.final1DVec, csvName);
	}

	if (intenseHisto.isActivatedFamily) {
		writeCSVCheckedValue(intenseHisto.final1DVec, csvName);
	}

	if (ui.checkBox_Intensity->isChecked()) {
		
	}

	if (ui.checkBox_Morph->isChecked()) {

	}

	if (glcm.isActivatedFamily) {
		writeCSVCheckedValue(glcm.final1DVec, csvName);
	}

	if (ui.checkBox_GLRLM->isChecked()) {

	}

}
void CPlatform::writeCSVFile(int seriesIdx, string csvName) {

	writeCSVCaseName(seriesIdx, csvName); // case명 & case명_modality 저장
	writeCSVFeatureValue(csvName); // feature values 저장 (각 family별로 호출)

}


// clear all vector //
void CPlatform::clearAll(int seriesIdx) {
	// for loop으로 대체 가능 (for auto, break)
	intenseStat.clearVector();
	intenseHisto.clearVector();
	glcm.clearVector();
	/*
	localIntense.clear();
	morphology.clear();
	glrlm.clear();
	*/

	cout << "clear series[" << seriesIdx << "]'s all values!" << endl;

}


// normalization and 16 bit image show //
void normalizeMask(unsigned char* mask, int nWidth, int nHeight, unsigned char nSetMin, unsigned char nSetMax)
{
	unsigned char nMin = UCHAR_MAX;
	unsigned char nMax = 0;

	for (int i = 0; i<nWidth; i++) {
		for (int j = 0; j<nHeight; j++) {
			if (mask[j*nWidth + i] > nMax) {
				nMax = mask[j*nWidth + i];
			}

			if (mask[j*nWidth + i] < nMin) {
				nMin = mask[j*nWidth + i];
			}
		}
	}

	//normalization
	if (nMax - nMin != 0) {
		for (int i = 0; i<nWidth; i++) {
			for (int j = 0; j<nHeight; j++) {
				mask[j*nWidth + i] = (mask[j*nWidth + i] - nMin) * (nSetMax - nSetMin) / (nMax - nMin) + nSetMin;
			}
		}
	}
}
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


// get min, max of Mat //
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
	// save GUI settings - config, member //
	saveSettings();


	// create and preset csv file //
	string currentTime = getCurrentTime();
	string csvName = outputFolder + currentTime + ".csv";
	presetCSVFile(csvName);
	
	
	// filtering and feature extraction by series //
	int nSeriesCnt = m_ciData.getSeriesCount();
	cout << "*** total number of Series : " << nSeriesCnt << " ***" << endl;

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
		m_ciData.clearImages(i); // clear ori m_ciData (images, masks)

		// slice by slice //
		for (int j = 0; j < nImageCnt; j++) {

			// ROI slice check //
			float nPixelsInMask = 0;
			if (!isEmptyMask(ppucMasks[j], nWidth, nHeight, nPixelsInMask)) {

				// filtering //
				Mat img_filtered;
				filtering(ppsImages[j], img_filtered, nWidth, nHeight, FILTER_MODE);
				SAFE_DELETE_ARRAY(ppsImages[j]); // psImage 포인터가 가리키고 있던 메모리 데이터들 해제 => ***이후엔 Mat 데이터를 가리키므로, 또 해제해주지 않아야 함!***
				ppsImages[j] = (short*)img_filtered.data; // 주소 변경
				
				// Resampling (Interpolation) //
				Mat img_resampled, mask_resampled;
				int nWidth_new = nWidth; // 원본값(nWidth, nHeight)는 한 시리즈 내에서 유지해야 함! 갱신 X! (for. 첫 ROI check)
				int nHeight_new = nHeight;
				float pixelSpacingX, pixelSpacingY;
				m_ciData.getPixelSpacing(i, j, pixelSpacingX, pixelSpacingY);
				resampling(ppsImages[j], ppucMasks[j], nWidth_new, nHeight_new, nPixelsInMask, pixelSpacingX, pixelSpacingY, img_resampled, mask_resampled); // new nWidth, nHeight 
				SAFE_DELETE_ARRAY(ppucMasks[j]);
				ppsImages[j] = (short*)img_resampled.data;
				ppucMasks[j] = (unsigned char*)mask_resampled.data;
				
				// Resegmentation (Outlier Filteration) //
				Mat mask_resegmented;
				resegmentation(ppsImages[j], ppucMasks[j], nWidth_new, nHeight_new, mask_resegmented);
				ppucMasks[j] = (unsigned char*)mask_resegmented.data;
				
				// feature extraction //
				if (!isEmptyMask(ppucMasks[j], nWidth_new, nHeight_new, nPixelsInMask)) {   // resegmentation 후의 mask 재확인
					featureExtraction(ppsImages[j], ppucMasks[j], nHeight_new, nWidth_new); // final2DVec에 각 슬라이스들 값 누적
				}
			}
		}
		
		// mean all ROI slices //
		averageAllSlices(); // final1DVec에 각 슬라이스 평균값 넣기

		// write csv file //
		writeCSVFile(i, csvName);
		
		// clear all vector //
		clearAll(i);

		// 메모리 소멸 //
		//SAFE_DELETE_VOLUME(ppucMasks, nMaskCnt);

		// progress bar //
		setProgressBarValue(i, nSeriesCnt);
	}


	// remove if isEmpty or change log file name
	string txtName = outputFolder + currentTime + ".txt";
	m_ciData.checkIsEmptyLog(txtName);
	

	// finish pop-up and exit		
	if (QMessageBox(QMessageBox::Information, " ", "extraction finished!", QMessageBox::Close).exec() == QMessageBox::Close) {
		QApplication::quit();
	}
	
}

