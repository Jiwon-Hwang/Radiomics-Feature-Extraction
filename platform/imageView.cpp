#include <image.h>
#include "platform.h"

// 생성자, 소멸자, 초기화 //
CImageView::CImageView(QWidget* parent)
	: QWidget(parent)
{
	setMouseTracking(true);

	m_qImageScreenDrawingPosition = new QLabel(this);
    m_qImageScreenDrawingPosition->setObjectName(QStringLiteral("label"));
    m_qImageScreenDrawingPosition->setGeometry(QRect(0, 0, 512, 512));
    m_qImageScreenDrawingPosition->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	m_qImageScreenDrawingPosition->setMouseTracking(true);

	init();
}
CImageView::~CImageView()
{
	clear();
}
void CImageView::init()
{
	// 멤버변수 //
	m_parent = NULL;
	m_pucWLWWImage = NULL;

	m_nActivatedFrameIdx = -1;
	m_nImageWidth = 0;
	m_nImageHeight = 0;
	m_nImageScreenWidth = 628;
	m_nImageScreenHeight = 628;
	m_nWindowWidth = 0;
	m_nWindowLevel = 0;

	// mouse
	m_bLMouseDown = false;
	m_bRMouseDown = false;
}
void CImageView::init(CPlatform* parent)
{
	// 멤버변수 //
	setParent(parent);
	m_nActivatedFrameIdx = m_parent->m_nActivatedFrameIdx;

	int nWidth = m_parent->m_ciData.getWidth(m_nActivatedFrameIdx);
	int nHeight = m_parent->m_ciData.getHeight(m_nActivatedFrameIdx);
	if (nWidth != m_nImageWidth || nHeight != m_nImageHeight) {
		m_qImage = QImage(nWidth, nHeight, QImage::Format_RGB32);

		m_nImageWidth = nWidth;
		m_nImageHeight = nHeight;
	}

	short* psImage = m_parent->m_ciData.getImage(m_nActivatedFrameIdx);
	SAFE_DELETE_ARRAY(m_pucWLWWImage);
	m_pucWLWWImage = new unsigned char[m_nImageWidth * m_nImageHeight];
	memset(m_pucWLWWImage, 0, sizeof(unsigned char)*m_nImageWidth * m_nImageHeight);

	int nWL, nWW;
	m_parent->m_ciData.getCImage(m_nActivatedFrameIdx)->getWLWW(nWL, nWW);

	int nMin = nWL - nWW;
	int nMax = nWL + nWW;

	adjustIntensityHistogram(nMin, nMax); // (window min, window max)
	//adjustIntensityHistogram(psImage, m_nImageWidth, m_nImageHeight);

	// mouse
	m_bLMouseDown = false;
	m_bRMouseDown = false;
}
void CImageView::clear()
{
	if(m_qImageScreenDrawingPosition != NULL) {
		delete m_qImageScreenDrawingPosition;
	}
	SAFE_DELETE_ARRAY(m_pucWLWWImage);

	m_qpMouseTrackingPoints.clear();
}

// Setting //
void CImageView::setParent(CPlatform* parent) {
	m_parent = parent;
}
void CImageView::setSignalSlot()
{
	
}
void CImageView::setImageScreenSize(int nScreenWidth, int nScreenHeight)
{
	this->setMaximumWidth(nScreenWidth);
	this->setMaximumHeight(nScreenHeight);

	// XA Copy
	// 기본 XA 영상
	for(int row=0; row< m_nImageHeight; row++) {
		for(int col=0; col<m_nImageWidth; col++) {
			int index = row*m_nImageWidth + col;
			int value = 0;

			m_qImage.setPixel(col, row, qRgb(value, value, value));
		}
	}
	m_nImageScreenWidth = nScreenWidth;
	m_nImageScreenHeight = nScreenHeight;

	m_qImageScreenDrawingPosition->setGeometry(QRect(0, 0, m_nImageScreenWidth, m_nImageScreenHeight));
	redraw(false);
}
bool CImageView::checkImageRange(int nX, int nY, int nMargin)
{
	if( (nX>=nMargin) && (nX<m_nImageWidth-nMargin) && (nY>=nMargin) && (nY<m_nImageHeight-nMargin) ) {
		return true;
	}
	return false;
}
void CImageView::adjustIntensityHistogram(short* psImage, int nWidth, int nHeight)
{
	// histogram (CurrentMaxValue, CurrentMinValue);
	//int nCurrentMinValue = m_parent->m_ciHistogramView->getCurrentMinValue();
	//int nCurrentMaxValue = m_parent->m_ciHistogramView->getCurrentMaxValue();

	// normalize
	
	int nMaxValue = INT_MIN;
	int nMinValue = INT_MAX;

	for (int row = 0; row< nHeight; row++) {
		for (int col = 0; col<nWidth; col++) {
			int index = row*nWidth + col;
			short value = psImage[index];

			if (value > nMaxValue) {
				nMaxValue = value;
			}
			else if (value < nMinValue) {
				nMinValue = value;
			}
		}
	}

	int nWindowWidth = nMaxValue - nMinValue;
	int nWindowLevel = (nWindowWidth / 2) + (nMinValue);
	int nCurrentMaxValue = nWindowLevel + (int)nWindowWidth / 2;
	int nCurrentMinValue = nWindowLevel - (int)nWindowWidth / 2;

	// 
	adjustIntensityHistogram(nCurrentMinValue, nCurrentMaxValue);
}
void CImageView::adjustIntensityHistogram(int nWindowMinValue, int nWindowMaxValue)
{
	// input parameter //
	short* psInputImage = m_parent->m_ciData.getImage(m_nActivatedFrameIdx);
	unsigned char* usOutputImage = m_pucWLWWImage;
	int nImageWidth = m_nImageWidth;
	int nImageHeight = m_nImageHeight;

	if (psInputImage == NULL) {
		return;
	}

	// window
	for (int row = 0; row< nImageHeight; row++) {
		for (int col = 0; col< nImageWidth; col++) {
			int index = row*nImageWidth + col;
			int value = (short)psInputImage[index];

			// 
			if (nWindowMinValue > value) {
				usOutputImage[index] = (unsigned char)0;
			}
			else if (nWindowMaxValue < value) {
				usOutputImage[index] = (unsigned char)255;
			}
			else {
				int normValue = (float)(value - nWindowMinValue)* (255) / (float)(nWindowMaxValue - nWindowMinValue + 1);
				usOutputImage[index] = (unsigned char)normValue;
			}
		}
	}

	// setting //
	int nWindowLevel = (nWindowMinValue + nWindowMaxValue) / 2;
	int nWindowWidth = (nWindowMaxValue - nWindowMinValue);
	m_nWindowWidth = nWindowWidth;
	m_nWindowLevel = nWindowLevel;
}
bool CImageView::setImage(unsigned char* pucImage, int nWidth, int nHeight)
{
	if(pucImage == NULL) {return false;}
	if(m_pucWLWWImage == NULL) {return false;}

	for(int row=0; row< nHeight; row++) {
		for(int col=0; col< nWidth; col++) {
			int index = row*nWidth + col;
			m_pucWLWWImage[index] = pucImage[index];
		}
	}

	redraw(false);
	return true;
}

// point transform //
QPoint CImageView::pointToWindowPoint(QPoint point)
{
	QPoint windowPoint;

	windowPoint.setX( ((point.x() / (float)m_nImageWidth * (float)m_nImageScreenWidth) + 0.5)   );
	windowPoint.setY( ((point.y() / (float)m_nImageHeight * (float)m_nImageScreenHeight) + 0.5) );

	return windowPoint;
}
QPoint CImageView::windowPointToPoint(QPoint windowPoint)
{
	QPoint point;

	point.setX( ((windowPoint.x() / (float)m_nImageScreenWidth * (float)m_nImageWidth) + 0.5)   );
	point.setY( ((windowPoint.y() / (float)m_nImageScreenHeight * (float)m_nImageHeight) + 0.5) );

	return point;
}

QPoint CImageView::getMousePoint(void)
{
	return m_qpMousePoint;
}
QVector<QPoint> CImageView::getMousePoints(void)
{
	QVector<QPoint> result;
	
	for(int i=0; i< m_qpMouseTrackingPoints.size(); i++) {
		result.push_back(m_qpMouseTrackingPoints[i]);
	}

	return result;
}

// redraw //
void CImageView::redraw(bool isMouseMove)
{
	if(m_pucWLWWImage == NULL) {return;}

	// 기본 이미지, Label 이미지, Selected Region
	if(isMouseMove == false) {
		// 기본 XA 영상 (0~255로 정규화)
		for(int row=0; row< m_nImageHeight; row++) {
			for(int col=0; col<m_nImageWidth; col++) {
				int index = row*m_nImageWidth + col;
				unsigned char value = (unsigned char)m_pucWLWWImage[index];

				m_qImage.setPixel(col, row, qRgb(value, value, value)); // setPixel : 특정 위치(col, row)에 qRgb 값으로 픽셀값 지정. 색칠
			}
		}

		// 마스크 overlay
		unsigned char* pucMask = m_parent->m_ciData.getMask(m_nActivatedFrameIdx);
		if(pucMask) {
			for(int row = 0; row< m_nImageHeight; row++) {
				for(int col = 0; col<m_nImageWidth; col++) {
					int index = row*m_nImageWidth + col;

					unsigned char value = (unsigned char)m_pucWLWWImage[index];
					unsigned char maskValue = (unsigned char)pucMask[index];

					if (maskValue > (unsigned char)0) {
						int r = 255, g = 0, b = 0;
						float fOpacity = 0.7;

						float fValueR = value * fOpacity + r*(1 - fOpacity);
						float fValueG = value * fOpacity + g*(1 - fOpacity);
						float fValueB = value * fOpacity + b*(1 - fOpacity);

						m_qImage.setPixel(col, row, qRgb(fValueR, fValueG, fValueB));
					}
				}
			}
		}
		
		// scaling
		m_qImageScreen = m_qImage.scaled(QSize(m_nImageScreenWidth, m_nImageScreenHeight), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}
}

// Event //
void CImageView::mousePressEvent(QMouseEvent* event)
{
	Q_UNUSED(event);

	int nMouseX = ( (event->x() / (float)m_nImageScreenWidth * (float)m_nImageWidth) + 0.5);
	int nMouseY = ( (event->y() / (float)m_nImageScreenHeight * (float)m_nImageHeight) + 0.5);
	
	if(event->button() == Qt::LeftButton) {
		m_bLMouseDown = true;
		m_qpLMouseDownPoint = QPoint(nMouseX, nMouseY);

		if(checkImageRange(nMouseX, nMouseY, 2)) {
			m_qpMousePoint = QPoint(nMouseX, nMouseY);
			m_qpMouseTrackingPoints.clear();
			m_qpMouseTrackingPoints.push_back(m_qpMousePoint);
		}
	}
	else if(event->button() == Qt::RightButton) {

	}
	
	redraw(false);
}
void CImageView::mouseMoveEvent(QMouseEvent* event)
{
	Q_UNUSED(event);

	int nMouseX = ( (event->x() / (float)m_nImageScreenWidth * (float)m_nImageWidth) + 0.5);
	int nMouseY = ( (event->y() / (float)m_nImageScreenHeight * (float)m_nImageHeight) + 0.5);

	if(checkImageRange(nMouseX, nMouseY, 2)) {
		m_qpMousePoint = QPoint(nMouseX, nMouseY);

		if((m_bLMouseDown||m_bRMouseDown)) {
			m_qpMouseTrackingPoints.push_back(m_qpMousePoint);
			redraw(true);
		}
	}
	else {
		
	}
}
void CImageView::mouseReleaseEvent(QMouseEvent* event)
{
	Q_UNUSED(event);

	int nMouseX = ( (event->x() / (float)m_nImageScreenWidth * (float)m_nImageWidth) + 0.5);
	int nMouseY = ( (event->y() / (float)m_nImageScreenHeight * (float)m_nImageHeight) + 0.5);
	//qDebug("mouse up event(%d, %d)", nMouseX, nMouseY);

	if(event->button() == Qt::LeftButton) {
		m_bLMouseDown = false;

		//
		if(checkImageRange(nMouseX, nMouseY, 2)) {
			m_qpMousePoint = QPoint(nMouseX, nMouseY);
			//m_qpMouseTrackingPoints.clear();
		}
	}
	else if(event->button() == Qt::RightButton) {
		m_bRMouseDown = false;
	}

	redraw(false);
}
void CImageView::mouseDoubleClickEvent(QMouseEvent* event)
{

}
void CImageView::wheelEvent(QWheelEvent* event)
{
	
}
void CImageView::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	if(m_nImageWidth > 0) {
		QPixmap p = QPixmap::fromImage(m_qImageScreen);
		m_qImageScreenDrawingPosition->setPixmap(p);
	}
}