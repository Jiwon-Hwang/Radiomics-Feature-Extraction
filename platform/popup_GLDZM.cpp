#include "popup_GLDZM.h"


popup_GLDZM::popup_GLDZM(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::popup_GLDZM)
{
	ui->setupUi(this);

	filterGroup = new QButtonGroup;
	filterGroup->setExclusive(false); // default : true
	filterGroup->addButton(ui->checkBox_SRE, GLDZM::SRE);
	filterGroup->addButton(ui->checkBox_LRE, GLDZM::LRE);
	filterGroup->addButton(ui->checkBox_LGRE, GLDZM::LGRE);
	filterGroup->addButton(ui->checkBox_HGRE, GLDZM::HGRE);
	filterGroup->addButton(ui->checkBox_SRLE, GLDZM::SRLE);
	filterGroup->addButton(ui->checkBox_SRHE, GLDZM::SRHE);
	filterGroup->addButton(ui->checkBox_LRLE, GLDZM::LRLE);
	filterGroup->addButton(ui->checkBox_LRHE, GLDZM::LRHE);
	filterGroup->addButton(ui->checkBox_GNU, GLDZM::GNU);
	filterGroup->addButton(ui->checkBox_GNUN, GLDZM::GNUN);
	filterGroup->addButton(ui->checkBox_RLNU, GLDZM::RLNU);
	filterGroup->addButton(ui->checkBox_RLNUN, GLDZM::RLNUN);
	filterGroup->addButton(ui->checkBox_RP, GLDZM::RP);
	filterGroup->addButton(ui->checkBox_GLV, GLDZM::GLV);
	filterGroup->addButton(ui->checkBox_RLV, GLDZM::RLV);
	filterGroup->addButton(ui->checkBox_RE, GLDZM::RE);
	
	filterGroup->addButton(ui->checkBox_All, GLDZM::FEATURE_COUNT);


	// QLineEdit 입력 제약조건 - "int"만 허용
	QIntValidator intValidator(0, 999999); //최소수, 최대수
	ui->lineEdit_sBin->setValidator(&intValidator);
}

popup_GLDZM::~popup_GLDZM()
{
	if (filterGroup) {
		delete filterGroup;
		filterGroup = NULL;
	}
	delete ui;
}