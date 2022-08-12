#include "popup_Intensity.h"


popup_Intensity::popup_Intensity(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::popup_Intensity)
{
	ui->setupUi(this);

	filterGroup = new QButtonGroup;
	filterGroup->setExclusive(false); // default : true
	filterGroup->addButton(ui->checkBox_LocalPeak, LocalIntensity::LOCAL);
	filterGroup->addButton(ui->checkBox_GlobalPeak, LocalIntensity::GLOBAL);
	
	filterGroup->addButton(ui->checkBox_All, LocalIntensity::FEATURE_COUNT); 

}

popup_Intensity::~popup_Intensity()
{
	delete ui;
}