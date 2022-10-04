#include "popup_GLCM.h"


popup_GLCM::popup_GLCM(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::popup_GLCM)
{
	ui->setupUi(this);

	filterGroup = new QButtonGroup;
	filterGroup->setExclusive(false); // default : true
	filterGroup->addButton(ui->checkBox_JointMaximum, GLCM::JOINTMAXIMUM);
	filterGroup->addButton(ui->checkBox_JointAverage, GLCM::JOINTAVERAGE);
	filterGroup->addButton(ui->checkBox_JointVariance, GLCM::JOINTVARIANCE);
	filterGroup->addButton(ui->checkBox_JointEntropy, GLCM::JOINTENTROPY);
	filterGroup->addButton(ui->checkBox_DifferenceAverage, GLCM::DIFFAVERAGE);
	filterGroup->addButton(ui->checkBox_DifferenceVariance, GLCM::DIFFVARIANCE);
	filterGroup->addButton(ui->checkBox_DifferenceEntropy, GLCM::DIFFENTROPY);
	filterGroup->addButton(ui->checkBox_SumAverage, GLCM::SUMAVERAGE);
	filterGroup->addButton(ui->checkBox_SumVariance, GLCM::SUMVARIANCE);
	filterGroup->addButton(ui->checkBox_SumEntropy, GLCM::SUMENTROPY);
	filterGroup->addButton(ui->checkBox_AngSecMoment, GLCM::ANGSECMOMENT);
	filterGroup->addButton(ui->checkBox_Contrast, GLCM::CONTRAST);
	filterGroup->addButton(ui->checkBox_Dissimilarity, GLCM::DISSIMILARITY);
	filterGroup->addButton(ui->checkBox_InverseDiff, GLCM::INVERSEDIFF);
	filterGroup->addButton(ui->checkBox_InverseDiffNorm, GLCM::INVERSEDIFFNORM);
	filterGroup->addButton(ui->checkBox_InverseDiffMom, GLCM::INVERSEDIFFMOM);
	filterGroup->addButton(ui->checkBox_InverseDiffMomNorm, GLCM::INVERSEDIFFMOMNORM);
	filterGroup->addButton(ui->checkBox_InverseVariance, GLCM::INVERSEVAR);
	filterGroup->addButton(ui->checkBox_Correlation, GLCM::CORRELATION);
	filterGroup->addButton(ui->checkBox_Autocorrelation, GLCM::AUTOCORRELATION);
	filterGroup->addButton(ui->checkBox_ClusterTendency, GLCM::CLUSTERTENDENCY);
	filterGroup->addButton(ui->checkBox_ClusterShade, GLCM::CLUSTERSHADE);
	filterGroup->addButton(ui->checkBox_ClusterProminence, GLCM::CLUSTERPROMINENCE);
	filterGroup->addButton(ui->checkBox_FirstMCorrelation, GLCM::FIRSTMCORRELATION);
	filterGroup->addButton(ui->checkBox_SecondMCorrelation, GLCM::SECONDMCORRELATION);
	
	filterGroup->addButton(ui->checkBox_All, GLCM::FEATURE_COUNT);

	// QLineEdit 입력 제약조건 - "int"만 허용
	QIntValidator intValidator(0, 999999); //최소수, 최대수 설정 이후엔 QT 내부적으로 필요 없음 (new 필요 X) => 메모리 누수
	ui->lineEdit_sBin->setValidator(&intValidator);
}

popup_GLCM::~popup_GLCM()
{
	// new로 선언한 것은 항상 사용 후 delete (동적 할당 후 해제 필수 => 메모리 누수)
	if (filterGroup) {
		delete filterGroup;
		filterGroup = NULL;
	}
	delete ui;
}