#include "popup_Morph.h"


popup_Morph::popup_Morph(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::popup_Morph)
{
	ui->setupUi(this);

	filterGroup = new QButtonGroup;
	filterGroup->setExclusive(false); // default : true
	filterGroup->addButton(ui->checkBox_Volume_M, Morphology::VOLUME);
	filterGroup->addButton(ui->checkBox_Volume_VC, Morphology::APPROXVOLUME);
	filterGroup->addButton(ui->checkBox_SurfaceArea, Morphology::SURFACE);
	filterGroup->addButton(ui->checkBox_Surf2VolRatio, Morphology::SURF2VOLRATIO);
	filterGroup->addButton(ui->checkBox_Compactness1, Morphology::COMPACTNESS1);
	filterGroup->addButton(ui->checkBox_Compactness2, Morphology::COMPACTNESS2);
	filterGroup->addButton(ui->checkBox_SphericalDisp, Morphology::SPHERICALDISP);
	filterGroup->addButton(ui->checkBox_Sphericity, Morphology::SPHERICITY);
	filterGroup->addButton(ui->checkBox_Asphericity, Morphology::ASPHERICITY);
	filterGroup->addButton(ui->checkBox_Center, Morphology::CENTER);
	filterGroup->addButton(ui->checkBox_MaxDiameter, Morphology::MAXDIAMETER);
	filterGroup->addButton(ui->checkBox_MajorAxisLength, Morphology::MAJORAXISLENGTH);
	filterGroup->addButton(ui->checkBox_MinorAxisLength, Morphology::MINORAXISLENGTH);
	filterGroup->addButton(ui->checkBox_LeastAxisLength, Morphology::LEASTAXISLENGTH);
	filterGroup->addButton(ui->checkBox_Elongation, Morphology::ELONGATION);
	filterGroup->addButton(ui->checkBox_Flatness, Morphology::FLATNESS);
	filterGroup->addButton(ui->checkBox_VolDenAABB, Morphology::VOLDENAABB);
	filterGroup->addButton(ui->checkBox_AreaDenAABB, Morphology::AREADENAABB);
	filterGroup->addButton(ui->checkBox_VolDenOMBB, Morphology::VOLDENOMBB);
	filterGroup->addButton(ui->checkBox_AreaDenOMBB, Morphology::AREADENOMBB);
	filterGroup->addButton(ui->checkBox_VolDenAEE, Morphology::VOLDENAEE);
	filterGroup->addButton(ui->checkBox_AreaDenAEE, Morphology::AREADENAEE);
	filterGroup->addButton(ui->checkBox_VolDenCH, Morphology::VOLDENCH);
	filterGroup->addButton(ui->checkBox_AreaDenCH, Morphology::AREADENCH);
	filterGroup->addButton(ui->checkBox_IntegratedIntensity, Morphology::INTEGRATEDINTENSE);
	filterGroup->addButton(ui->checkBox_MoransI, Morphology::MORANSI);
	filterGroup->addButton(ui->checkBox_GearysC, Morphology::GEARYSC);
	
	filterGroup->addButton(ui->checkBox_All, Morphology::FEATURE_COUNT);

}

popup_Morph::~popup_Morph()
{
	if (filterGroup) {
		delete filterGroup;
		filterGroup = NULL;
	}

	delete ui;
}