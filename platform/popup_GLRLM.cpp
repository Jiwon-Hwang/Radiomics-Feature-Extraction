#include "popup_GLRLM.h"


popup_GLRLM::popup_GLRLM(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::popup_GLRLM)
{
	ui->setupUi(this);

	filterGroup = new QButtonGroup;
	filterGroup->setExclusive(false); // default : true
	filterGroup->addButton(ui->checkBox_SRE, GLRLM::SRE);
	filterGroup->addButton(ui->checkBox_LRE, GLRLM::LRE);
	filterGroup->addButton(ui->checkBox_LGE, GLRLM::LGE);
	filterGroup->addButton(ui->checkBox_HGE, GLRLM::HGE);
	filterGroup->addButton(ui->checkBox_SRLE, GLRLM::SRLE);
	filterGroup->addButton(ui->checkBox_SRHE, GLRLM::SRHE);
	filterGroup->addButton(ui->checkBox_LRLE, GLRLM::LRLE);
	filterGroup->addButton(ui->checkBox_LRHE, GLRLM::LRHE);
	filterGroup->addButton(ui->checkBox_GNU, GLRLM::GNU);
	filterGroup->addButton(ui->checkBox_GNUN, GLRLM::GNUN);
	filterGroup->addButton(ui->checkBox_RLNU, GLRLM::RLNU);
	filterGroup->addButton(ui->checkBox_RLNUN, GLRLM::RLNUN);
	filterGroup->addButton(ui->checkBox_RP, GLRLM::RP);
	filterGroup->addButton(ui->checkBox_GLV, GLRLM::GLV);
	filterGroup->addButton(ui->checkBox_RLV, GLRLM::RLV);
	filterGroup->addButton(ui->checkBox_RE, GLRLM::RE);
	
	filterGroup->addButton(ui->checkBox_All, GLRLM::FEATURE_COUNT);

}

popup_GLRLM::~popup_GLRLM()
{
	delete ui;
}