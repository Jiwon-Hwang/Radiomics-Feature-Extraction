#include "popup_NGLDM.h"


popup_NGLDM::popup_NGLDM(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::popup_NGLDM)
{
	ui->setupUi(this);

	filterGroup = new QButtonGroup;
	filterGroup->setExclusive(false); // default : true
	filterGroup->addButton(ui->checkBox_SRE, NGLDM::SRE);
	filterGroup->addButton(ui->checkBox_LRE, NGLDM::LRE);
	filterGroup->addButton(ui->checkBox_LGRE, NGLDM::LGRE);
	filterGroup->addButton(ui->checkBox_HGRE, NGLDM::HGRE);
	filterGroup->addButton(ui->checkBox_SRLE, NGLDM::SRLE);
	filterGroup->addButton(ui->checkBox_SRHE, NGLDM::SRHE);
	filterGroup->addButton(ui->checkBox_LRLE, NGLDM::LRLE);
	filterGroup->addButton(ui->checkBox_LRHE, NGLDM::LRHE);
	filterGroup->addButton(ui->checkBox_GNU, NGLDM::GNU);
	filterGroup->addButton(ui->checkBox_GNUN, NGLDM::GNUN);
	filterGroup->addButton(ui->checkBox_RLNU, NGLDM::RLNU);
	filterGroup->addButton(ui->checkBox_RLNUN, NGLDM::RLNUN);
	filterGroup->addButton(ui->checkBox_RP, NGLDM::RP);
	filterGroup->addButton(ui->checkBox_GLV, NGLDM::GLV);
	filterGroup->addButton(ui->checkBox_RLV, NGLDM::RLV);
	filterGroup->addButton(ui->checkBox_RE, NGLDM::RE);
	filterGroup->addButton(ui->checkBox_Energy, NGLDM::ENERGY);
	
	filterGroup->addButton(ui->checkBox_All, NGLDM::FEATURE_COUNT);


	// QLineEdit �Է� �������� - "int"�� ���
	QIntValidator intValidator(0, 999999); //�ּҼ�, �ִ��
	ui->lineEdit_sBin->setValidator(&intValidator);
}

popup_NGLDM::~popup_NGLDM()
{
	if (filterGroup) {
		delete filterGroup;
		filterGroup = NULL;
	}
	delete ui;
}