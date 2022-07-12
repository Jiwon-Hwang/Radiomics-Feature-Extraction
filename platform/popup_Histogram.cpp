#include "popup_Histogram.h"


popup_Histogram::popup_Histogram(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::popup_Histogram)
{
	ui->setupUi(this);

	filterGroup = new QButtonGroup;
	filterGroup->setExclusive(false); // default : true
	filterGroup->addButton(ui->checkBox_Mean, IntensityHistogram::MEAN);
	filterGroup->addButton(ui->checkBox_Variance, IntensityHistogram::VARIANCE);
	filterGroup->addButton(ui->checkBox_Skewness, IntensityHistogram::SKEWNESS);
	filterGroup->addButton(ui->checkBox_Kurtosis, IntensityHistogram::KURTOSIS);
	filterGroup->addButton(ui->checkBox_Median, IntensityHistogram::MEDIAN);
	filterGroup->addButton(ui->checkBox_Minimum, IntensityHistogram::MINIMUM);
	filterGroup->addButton(ui->checkBox_10thPercentile, IntensityHistogram::PERCENTILE10);
	filterGroup->addButton(ui->checkBox_90thPercentile, IntensityHistogram::PERCENTILE90);
	filterGroup->addButton(ui->checkBox_Maximum, IntensityHistogram::MAXIMUM);
	filterGroup->addButton(ui->checkBox_InterquartileRange, IntensityHistogram::INTERQUARTILERANGE);
	filterGroup->addButton(ui->checkBox_Mode, IntensityHistogram::MODE);
	filterGroup->addButton(ui->checkBox_Range, IntensityHistogram::RANGE);
	filterGroup->addButton(ui->checkBox_MeanAbsDev, IntensityHistogram::MEANABSDEV);
	filterGroup->addButton(ui->checkBox_RobustMeanAbsDev, IntensityHistogram::ROBUSTMEANABSDEV);
	filterGroup->addButton(ui->checkBox_MedianAbsDev, IntensityHistogram::MEDIANABSDEV);
	filterGroup->addButton(ui->checkBox_CoeffOfVar, IntensityHistogram::COEFFOFVAR);
	filterGroup->addButton(ui->checkBox_QuartileCoeff, IntensityHistogram::QUARTILECOEFF);
	filterGroup->addButton(ui->checkBox_Entropy, IntensityHistogram::ENTROPY);
	filterGroup->addButton(ui->checkBox_Uniformity, IntensityHistogram::UNIFORMITY);
	filterGroup->addButton(ui->checkBox_MaxHisGradient, IntensityHistogram::MAXHISTGRADIENT);
	filterGroup->addButton(ui->checkBox_MaxHistGradGrey, IntensityHistogram::MAXHISTGRADGREY);
	filterGroup->addButton(ui->checkBox_MinHistGradient, IntensityHistogram::MINHISTGRADIENT);
	filterGroup->addButton(ui->checkBox_MinHistGradGrey, IntensityHistogram::MINHISTGRADGREY);

	filterGroup->addButton(ui->checkBox_All, IntensityHistogram::FEATURE_COUNT); 


	// QLineEdit 입력 제약조건 - "int"만 허용
	QIntValidator *intValidator = new QIntValidator(0, 999999); //최소수, 최대수
	ui->lineEdit_sBin->setValidator(intValidator);

}

popup_Histogram::~popup_Histogram()
{
	delete ui;
}