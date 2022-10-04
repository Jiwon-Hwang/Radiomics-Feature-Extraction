#include "popup_Statistics.h"


popup_Statistics::popup_Statistics(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::popup_Statistics)
{
	ui->setupUi(this);

	filterGroup = new QButtonGroup;
	filterGroup->setExclusive(false); // default : true
	filterGroup->addButton(ui->checkBox_Mean, IntensityStatistics::MEAN);
	filterGroup->addButton(ui->checkBox_Variance, IntensityStatistics::VARIANCE);
	filterGroup->addButton(ui->checkBox_Skewness, IntensityStatistics::SKEWNESS);
	filterGroup->addButton(ui->checkBox_Kurtosis, IntensityStatistics::KURTOSIS);
	filterGroup->addButton(ui->checkBox_Median, IntensityStatistics::MEDIAN);
	filterGroup->addButton(ui->checkBox_Minimum, IntensityStatistics::MINIMUM);
	filterGroup->addButton(ui->checkBox_10thPercentile, IntensityStatistics::PERCENTILE10);
	filterGroup->addButton(ui->checkBox_90thPercentile, IntensityStatistics::PERCENTILE90);
	filterGroup->addButton(ui->checkBox_Maximum, IntensityStatistics::MAXIMUM);
	filterGroup->addButton(ui->checkBox_InterquartileRange, IntensityStatistics::INTERQUARTILERANGE);
	filterGroup->addButton(ui->checkBox_Range, IntensityStatistics::RANGE);
	filterGroup->addButton(ui->checkBox_MeanAbsDev, IntensityStatistics::MEANABSDEV);
	filterGroup->addButton(ui->checkBox_RobustMeanAbsDev, IntensityStatistics::ROBUSTMEANABSDEV);
	filterGroup->addButton(ui->checkBox_MedianAbsDev, IntensityStatistics::MEDIANABSDEV);
	filterGroup->addButton(ui->checkBox_CoeffOfVar, IntensityStatistics::COEFFOFVAR);
	filterGroup->addButton(ui->checkBox_QuartileCoeff, IntensityStatistics::QUARTILECOEFF);
	filterGroup->addButton(ui->checkBox_Energy, IntensityStatistics::ENERGY);
	filterGroup->addButton(ui->checkBox_RootMeanSquare, IntensityStatistics::ROOTMEANSQUARE);

	filterGroup->addButton(ui->checkBox_All, IntensityStatistics::FEATURE_COUNT); 

}

popup_Statistics::~popup_Statistics()
{
	if (filterGroup) {
		delete filterGroup;
		filterGroup = NULL;
	}
	delete ui;
}