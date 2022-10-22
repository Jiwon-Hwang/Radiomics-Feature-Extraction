#include "popup_GLSZM.h"


popup_GLSZM::popup_GLSZM(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::popup_GLSZM)
{
	ui->setupUi(this);

	filterGroup = new QButtonGroup;
	filterGroup->setExclusive(false); // default : true
	filterGroup->addButton(ui->checkBox_GNUN, GLSZM::GNUN);
	filterGroup->addButton(ui->checkBox_HGZE, GLSZM::HGZE);
	filterGroup->addButton(ui->checkBox_SZE, GLSZM::SZE);
	filterGroup->addButton(ui->checkBox_ZSV, GLSZM::ZSV);
	filterGroup->addButton(ui->checkBox_GLV, GLSZM::GLV);
	filterGroup->addButton(ui->checkBox_GNU, GLSZM::GNU);
	filterGroup->addButton(ui->checkBox_ZSNU, GLSZM::ZSNU);
	filterGroup->addButton(ui->checkBox_LZHGE, GLSZM::LZHGE);
	filterGroup->addButton(ui->checkBox_ZSNUN, GLSZM::ZSNUN);
	filterGroup->addButton(ui->checkBox_ZSE, GLSZM::ZSE);
	filterGroup->addButton(ui->checkBox_SZHGE, GLSZM::SZHGE);
	filterGroup->addButton(ui->checkBox_LZLGE, GLSZM::LZLGE);
	filterGroup->addButton(ui->checkBox_LZE, GLSZM::LZE);
	filterGroup->addButton(ui->checkBox_LGZE, GLSZM::LGZE);
	filterGroup->addButton(ui->checkBox_SZLGE, GLSZM::SZLGE);
	filterGroup->addButton(ui->checkBox_ZP, GLSZM::ZP);
	
	filterGroup->addButton(ui->checkBox_All, GLSZM::FEATURE_COUNT);


	// QLineEdit 입력 제약조건 - "int"만 허용
	QIntValidator intValidator(0, 999999); //최소수, 최대수
	ui->lineEdit_sBin->setValidator(&intValidator);
}

popup_GLSZM::~popup_GLSZM()
{
	if (filterGroup) {
		delete filterGroup;
		filterGroup = NULL;
	}
	delete ui;
}