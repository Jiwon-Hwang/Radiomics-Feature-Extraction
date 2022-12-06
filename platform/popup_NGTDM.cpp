#include "popup_NGTDM.h"


popup_NGTDM::popup_NGTDM(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::popup_NGTDM)
{
	ui->setupUi(this);

	filterGroup = new QButtonGroup;
	filterGroup->setExclusive(false); // default : true
	filterGroup->addButton(ui->checkBox_Coarseness, NGTDM::COARSENESS);
	filterGroup->addButton(ui->checkBox_Contrast, NGTDM::CONTRAST);
	filterGroup->addButton(ui->checkBox_Busyness, NGTDM::BUSYNESS);
	filterGroup->addButton(ui->checkBox_Complexity, NGTDM::COMPLEXITY);
	filterGroup->addButton(ui->checkBox_Strength, NGTDM::STRENGTH);
	
	filterGroup->addButton(ui->checkBox_All, NGTDM::FEATURE_COUNT);


	// QLineEdit 입력 제약조건 - "int"만 허용
	QIntValidator intValidator(0, 999999); //최소수, 최대수
	ui->lineEdit_sBin->setValidator(&intValidator);
}

popup_NGTDM::~popup_NGTDM()
{
	if (filterGroup) {
		delete filterGroup;
		filterGroup = NULL;
	}
	delete ui;
}