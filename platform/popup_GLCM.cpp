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

}

popup_GLCM::~popup_GLCM()
{
	delete ui;
}