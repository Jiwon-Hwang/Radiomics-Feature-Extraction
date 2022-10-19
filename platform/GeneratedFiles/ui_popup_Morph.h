/********************************************************************************
** Form generated from reading UI file 'popup_Morph.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POPUP_MORPH_H
#define UI_POPUP_MORPH_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_popup_Morph
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox_Features;
    QGridLayout *gridLayout;
    QCheckBox *checkBox_Elongation;
    QCheckBox *checkBox_LeastAxisLength;
    QCheckBox *checkBox_VolDenAABB;
    QCheckBox *checkBox_MinorAxisLength;
    QCheckBox *checkBox_MajorAxisLength;
    QCheckBox *checkBox_SurfaceArea;
    QCheckBox *checkBox_MaxDiameter;
    QCheckBox *checkBox_Sphericity;
    QCheckBox *checkBox_Flatness;
    QCheckBox *checkBox_Volume_VC;
    QCheckBox *checkBox_Volume_M;
    QCheckBox *checkBox_All;
    QCheckBox *checkBox_Asphericity;
    QCheckBox *checkBox_SphericalDisp;
    QCheckBox *checkBox_Surf2VolRatio;
    QCheckBox *checkBox_Center;
    QCheckBox *checkBox_Compactness2;
    QCheckBox *checkBox_Compactness1;
    QCheckBox *checkBox_AreaDenAABB;
    QCheckBox *checkBox_VolDenOMBB;
    QCheckBox *checkBox_AreaDenAEE;
    QCheckBox *checkBox_AreaDenOMBB;
    QCheckBox *checkBox_VolDenAEE;
    QCheckBox *checkBox_VolDenCH;
    QCheckBox *checkBox_AreaDenCH;
    QCheckBox *checkBox_MoransI;
    QCheckBox *checkBox_GearysC;
    QCheckBox *checkBox_IntegratedIntensity;

    void setupUi(QDialog *popup_Morph)
    {
        if (popup_Morph->objectName().isEmpty())
            popup_Morph->setObjectName(QStringLiteral("popup_Morph"));
        popup_Morph->resize(400, 770);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(popup_Morph->sizePolicy().hasHeightForWidth());
        popup_Morph->setSizePolicy(sizePolicy);
        buttonBox = new QDialogButtonBox(popup_Morph);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(230, 730, 151, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);
        groupBox_Features = new QGroupBox(popup_Morph);
        groupBox_Features->setObjectName(QStringLiteral("groupBox_Features"));
        groupBox_Features->setGeometry(QRect(20, 30, 361, 691));
        groupBox_Features->setCheckable(false);
        gridLayout = new QGridLayout(groupBox_Features);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(12, 10, 12, 10);
        checkBox_Elongation = new QCheckBox(groupBox_Features);
        checkBox_Elongation->setObjectName(QStringLiteral("checkBox_Elongation"));
        checkBox_Elongation->setEnabled(true);
        checkBox_Elongation->setCheckable(true);
        checkBox_Elongation->setChecked(false);
        checkBox_Elongation->setTristate(false);

        gridLayout->addWidget(checkBox_Elongation, 8, 0, 1, 1);

        checkBox_LeastAxisLength = new QCheckBox(groupBox_Features);
        checkBox_LeastAxisLength->setObjectName(QStringLiteral("checkBox_LeastAxisLength"));
        checkBox_LeastAxisLength->setEnabled(true);
        checkBox_LeastAxisLength->setCheckable(true);
        checkBox_LeastAxisLength->setChecked(false);
        checkBox_LeastAxisLength->setTristate(false);

        gridLayout->addWidget(checkBox_LeastAxisLength, 7, 1, 1, 1);

        checkBox_VolDenAABB = new QCheckBox(groupBox_Features);
        checkBox_VolDenAABB->setObjectName(QStringLiteral("checkBox_VolDenAABB"));
        checkBox_VolDenAABB->setEnabled(true);
        checkBox_VolDenAABB->setCheckable(true);
        checkBox_VolDenAABB->setChecked(false);
        checkBox_VolDenAABB->setTristate(false);

        gridLayout->addWidget(checkBox_VolDenAABB, 9, 0, 1, 1);

        checkBox_MinorAxisLength = new QCheckBox(groupBox_Features);
        checkBox_MinorAxisLength->setObjectName(QStringLiteral("checkBox_MinorAxisLength"));
        checkBox_MinorAxisLength->setEnabled(true);
        checkBox_MinorAxisLength->setCheckable(true);
        checkBox_MinorAxisLength->setChecked(false);
        checkBox_MinorAxisLength->setTristate(false);

        gridLayout->addWidget(checkBox_MinorAxisLength, 7, 0, 1, 1);

        checkBox_MajorAxisLength = new QCheckBox(groupBox_Features);
        checkBox_MajorAxisLength->setObjectName(QStringLiteral("checkBox_MajorAxisLength"));
        checkBox_MajorAxisLength->setEnabled(true);
        checkBox_MajorAxisLength->setCheckable(true);
        checkBox_MajorAxisLength->setChecked(false);
        checkBox_MajorAxisLength->setTristate(false);

        gridLayout->addWidget(checkBox_MajorAxisLength, 6, 1, 1, 1);

        checkBox_SurfaceArea = new QCheckBox(groupBox_Features);
        checkBox_SurfaceArea->setObjectName(QStringLiteral("checkBox_SurfaceArea"));
        checkBox_SurfaceArea->setEnabled(true);
        checkBox_SurfaceArea->setCheckable(true);
        checkBox_SurfaceArea->setChecked(false);
        checkBox_SurfaceArea->setTristate(false);

        gridLayout->addWidget(checkBox_SurfaceArea, 2, 0, 1, 1);

        checkBox_MaxDiameter = new QCheckBox(groupBox_Features);
        checkBox_MaxDiameter->setObjectName(QStringLiteral("checkBox_MaxDiameter"));
        checkBox_MaxDiameter->setEnabled(true);
        checkBox_MaxDiameter->setCheckable(true);
        checkBox_MaxDiameter->setChecked(false);
        checkBox_MaxDiameter->setTristate(false);

        gridLayout->addWidget(checkBox_MaxDiameter, 6, 0, 1, 1);

        checkBox_Sphericity = new QCheckBox(groupBox_Features);
        checkBox_Sphericity->setObjectName(QStringLiteral("checkBox_Sphericity"));
        checkBox_Sphericity->setEnabled(true);
        checkBox_Sphericity->setCheckable(true);
        checkBox_Sphericity->setChecked(false);
        checkBox_Sphericity->setTristate(false);

        gridLayout->addWidget(checkBox_Sphericity, 4, 1, 1, 1);

        checkBox_Flatness = new QCheckBox(groupBox_Features);
        checkBox_Flatness->setObjectName(QStringLiteral("checkBox_Flatness"));
        checkBox_Flatness->setEnabled(true);
        checkBox_Flatness->setCheckable(true);
        checkBox_Flatness->setChecked(false);
        checkBox_Flatness->setTristate(false);

        gridLayout->addWidget(checkBox_Flatness, 8, 1, 1, 1);

        checkBox_Volume_VC = new QCheckBox(groupBox_Features);
        checkBox_Volume_VC->setObjectName(QStringLiteral("checkBox_Volume_VC"));
        checkBox_Volume_VC->setEnabled(true);
        checkBox_Volume_VC->setCheckable(true);
        checkBox_Volume_VC->setChecked(false);
        checkBox_Volume_VC->setTristate(false);

        gridLayout->addWidget(checkBox_Volume_VC, 1, 1, 1, 1);

        checkBox_Volume_M = new QCheckBox(groupBox_Features);
        checkBox_Volume_M->setObjectName(QStringLiteral("checkBox_Volume_M"));
        checkBox_Volume_M->setAutoFillBackground(false);
        checkBox_Volume_M->setChecked(false);
        checkBox_Volume_M->setTristate(false);

        gridLayout->addWidget(checkBox_Volume_M, 1, 0, 1, 1);

        checkBox_All = new QCheckBox(groupBox_Features);
        checkBox_All->setObjectName(QStringLiteral("checkBox_All"));
        checkBox_All->setEnabled(true);
        QFont font;
        font.setBold(true);
        font.setUnderline(false);
        font.setWeight(75);
        font.setKerning(true);
        checkBox_All->setFont(font);
        checkBox_All->setAutoFillBackground(false);
        checkBox_All->setChecked(false);
        checkBox_All->setTristate(false);

        gridLayout->addWidget(checkBox_All, 0, 0, 1, 1);

        checkBox_Asphericity = new QCheckBox(groupBox_Features);
        checkBox_Asphericity->setObjectName(QStringLiteral("checkBox_Asphericity"));
        checkBox_Asphericity->setAutoFillBackground(false);
        checkBox_Asphericity->setChecked(false);
        checkBox_Asphericity->setTristate(false);

        gridLayout->addWidget(checkBox_Asphericity, 5, 0, 1, 1);

        checkBox_SphericalDisp = new QCheckBox(groupBox_Features);
        checkBox_SphericalDisp->setObjectName(QStringLiteral("checkBox_SphericalDisp"));
        checkBox_SphericalDisp->setAutoFillBackground(false);
        checkBox_SphericalDisp->setChecked(false);
        checkBox_SphericalDisp->setTristate(false);

        gridLayout->addWidget(checkBox_SphericalDisp, 4, 0, 1, 1);

        checkBox_Surf2VolRatio = new QCheckBox(groupBox_Features);
        checkBox_Surf2VolRatio->setObjectName(QStringLiteral("checkBox_Surf2VolRatio"));
        checkBox_Surf2VolRatio->setEnabled(true);
        checkBox_Surf2VolRatio->setCheckable(true);
        checkBox_Surf2VolRatio->setChecked(false);
        checkBox_Surf2VolRatio->setTristate(false);

        gridLayout->addWidget(checkBox_Surf2VolRatio, 2, 1, 1, 1);

        checkBox_Center = new QCheckBox(groupBox_Features);
        checkBox_Center->setObjectName(QStringLiteral("checkBox_Center"));
        checkBox_Center->setEnabled(true);
        checkBox_Center->setCheckable(true);
        checkBox_Center->setChecked(false);
        checkBox_Center->setTristate(false);

        gridLayout->addWidget(checkBox_Center, 5, 1, 1, 1);

        checkBox_Compactness2 = new QCheckBox(groupBox_Features);
        checkBox_Compactness2->setObjectName(QStringLiteral("checkBox_Compactness2"));
        checkBox_Compactness2->setEnabled(true);
        checkBox_Compactness2->setCheckable(true);
        checkBox_Compactness2->setChecked(false);
        checkBox_Compactness2->setTristate(false);

        gridLayout->addWidget(checkBox_Compactness2, 3, 1, 1, 1);

        checkBox_Compactness1 = new QCheckBox(groupBox_Features);
        checkBox_Compactness1->setObjectName(QStringLiteral("checkBox_Compactness1"));
        checkBox_Compactness1->setEnabled(true);
        checkBox_Compactness1->setCheckable(true);
        checkBox_Compactness1->setChecked(false);
        checkBox_Compactness1->setTristate(false);

        gridLayout->addWidget(checkBox_Compactness1, 3, 0, 1, 1);

        checkBox_AreaDenAABB = new QCheckBox(groupBox_Features);
        checkBox_AreaDenAABB->setObjectName(QStringLiteral("checkBox_AreaDenAABB"));
        checkBox_AreaDenAABB->setEnabled(true);
        checkBox_AreaDenAABB->setCheckable(true);
        checkBox_AreaDenAABB->setChecked(false);
        checkBox_AreaDenAABB->setTristate(false);

        gridLayout->addWidget(checkBox_AreaDenAABB, 9, 1, 1, 1);

        checkBox_VolDenOMBB = new QCheckBox(groupBox_Features);
        checkBox_VolDenOMBB->setObjectName(QStringLiteral("checkBox_VolDenOMBB"));
        checkBox_VolDenOMBB->setEnabled(true);
        checkBox_VolDenOMBB->setCheckable(true);
        checkBox_VolDenOMBB->setChecked(false);
        checkBox_VolDenOMBB->setTristate(false);

        gridLayout->addWidget(checkBox_VolDenOMBB, 10, 0, 1, 1);

        checkBox_AreaDenAEE = new QCheckBox(groupBox_Features);
        checkBox_AreaDenAEE->setObjectName(QStringLiteral("checkBox_AreaDenAEE"));
        checkBox_AreaDenAEE->setEnabled(true);
        checkBox_AreaDenAEE->setCheckable(true);
        checkBox_AreaDenAEE->setChecked(false);
        checkBox_AreaDenAEE->setTristate(false);

        gridLayout->addWidget(checkBox_AreaDenAEE, 11, 1, 1, 1);

        checkBox_AreaDenOMBB = new QCheckBox(groupBox_Features);
        checkBox_AreaDenOMBB->setObjectName(QStringLiteral("checkBox_AreaDenOMBB"));
        checkBox_AreaDenOMBB->setEnabled(true);
        checkBox_AreaDenOMBB->setCheckable(true);
        checkBox_AreaDenOMBB->setChecked(false);
        checkBox_AreaDenOMBB->setTristate(false);

        gridLayout->addWidget(checkBox_AreaDenOMBB, 10, 1, 1, 1);

        checkBox_VolDenAEE = new QCheckBox(groupBox_Features);
        checkBox_VolDenAEE->setObjectName(QStringLiteral("checkBox_VolDenAEE"));
        checkBox_VolDenAEE->setEnabled(true);
        checkBox_VolDenAEE->setCheckable(true);
        checkBox_VolDenAEE->setChecked(false);
        checkBox_VolDenAEE->setTristate(false);

        gridLayout->addWidget(checkBox_VolDenAEE, 11, 0, 1, 1);

        checkBox_VolDenCH = new QCheckBox(groupBox_Features);
        checkBox_VolDenCH->setObjectName(QStringLiteral("checkBox_VolDenCH"));
        checkBox_VolDenCH->setEnabled(true);
        checkBox_VolDenCH->setCheckable(true);
        checkBox_VolDenCH->setChecked(false);
        checkBox_VolDenCH->setTristate(false);

        gridLayout->addWidget(checkBox_VolDenCH, 12, 0, 1, 1);

        checkBox_AreaDenCH = new QCheckBox(groupBox_Features);
        checkBox_AreaDenCH->setObjectName(QStringLiteral("checkBox_AreaDenCH"));
        checkBox_AreaDenCH->setEnabled(true);
        checkBox_AreaDenCH->setCheckable(true);
        checkBox_AreaDenCH->setChecked(false);
        checkBox_AreaDenCH->setTristate(false);

        gridLayout->addWidget(checkBox_AreaDenCH, 12, 1, 1, 1);

        checkBox_MoransI = new QCheckBox(groupBox_Features);
        checkBox_MoransI->setObjectName(QStringLiteral("checkBox_MoransI"));
        checkBox_MoransI->setEnabled(true);
        checkBox_MoransI->setCheckable(true);
        checkBox_MoransI->setChecked(false);
        checkBox_MoransI->setTristate(false);

        gridLayout->addWidget(checkBox_MoransI, 13, 1, 1, 1);

        checkBox_GearysC = new QCheckBox(groupBox_Features);
        checkBox_GearysC->setObjectName(QStringLiteral("checkBox_GearysC"));
        checkBox_GearysC->setEnabled(true);
        checkBox_GearysC->setCheckable(true);
        checkBox_GearysC->setChecked(false);
        checkBox_GearysC->setTristate(false);

        gridLayout->addWidget(checkBox_GearysC, 14, 0, 1, 1);

        checkBox_IntegratedIntensity = new QCheckBox(groupBox_Features);
        checkBox_IntegratedIntensity->setObjectName(QStringLiteral("checkBox_IntegratedIntensity"));
        checkBox_IntegratedIntensity->setEnabled(true);
        checkBox_IntegratedIntensity->setCheckable(true);
        checkBox_IntegratedIntensity->setChecked(false);
        checkBox_IntegratedIntensity->setTristate(false);

        gridLayout->addWidget(checkBox_IntegratedIntensity, 13, 0, 1, 1);


        retranslateUi(popup_Morph);
        QObject::connect(buttonBox, SIGNAL(accepted()), popup_Morph, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), popup_Morph, SLOT(reject()));

        QMetaObject::connectSlotsByName(popup_Morph);
    } // setupUi

    void retranslateUi(QDialog *popup_Morph)
    {
        popup_Morph->setWindowTitle(QApplication::translate("popup_Morph", "Set Measurements", Q_NULLPTR));
        groupBox_Features->setTitle(QApplication::translate("popup_Morph", "Features", Q_NULLPTR));
        checkBox_Elongation->setText(QApplication::translate("popup_Morph", "Elongation", Q_NULLPTR));
        checkBox_LeastAxisLength->setText(QApplication::translate("popup_Morph", "Least Axis Length", Q_NULLPTR));
        checkBox_VolDenAABB->setText(QApplication::translate("popup_Morph", "Volume Density (AABB)", Q_NULLPTR));
        checkBox_MinorAxisLength->setText(QApplication::translate("popup_Morph", "Minor Axis Length", Q_NULLPTR));
        checkBox_MajorAxisLength->setText(QApplication::translate("popup_Morph", "Major Axis Length", Q_NULLPTR));
        checkBox_SurfaceArea->setText(QApplication::translate("popup_Morph", "Surface Area", Q_NULLPTR));
        checkBox_MaxDiameter->setText(QApplication::translate("popup_Morph", "Max 3D Diameter", Q_NULLPTR));
        checkBox_Sphericity->setText(QApplication::translate("popup_Morph", "Sphericity", Q_NULLPTR));
        checkBox_Flatness->setText(QApplication::translate("popup_Morph", "Flatness", Q_NULLPTR));
        checkBox_Volume_VC->setText(QApplication::translate("popup_Morph", "Volume (voxel counting)", Q_NULLPTR));
        checkBox_Volume_M->setText(QApplication::translate("popup_Morph", "Volume (mesh)", Q_NULLPTR));
        checkBox_All->setText(QApplication::translate("popup_Morph", "All", Q_NULLPTR));
        checkBox_Asphericity->setText(QApplication::translate("popup_Morph", "Asphericity", Q_NULLPTR));
        checkBox_SphericalDisp->setText(QApplication::translate("popup_Morph", "Spherical Disproportion", Q_NULLPTR));
        checkBox_Surf2VolRatio->setText(QApplication::translate("popup_Morph", "Surface to Volume Ratio", Q_NULLPTR));
        checkBox_Center->setText(QApplication::translate("popup_Morph", "Center of mass shift", Q_NULLPTR));
        checkBox_Compactness2->setText(QApplication::translate("popup_Morph", "Compactness 2", Q_NULLPTR));
        checkBox_Compactness1->setText(QApplication::translate("popup_Morph", "Compactness 1", Q_NULLPTR));
        checkBox_AreaDenAABB->setText(QApplication::translate("popup_Morph", "Area Density (AABB)", Q_NULLPTR));
        checkBox_VolDenOMBB->setText(QApplication::translate("popup_Morph", "Volume Density (OMBB)", Q_NULLPTR));
        checkBox_AreaDenAEE->setText(QApplication::translate("popup_Morph", "Area Density (AEE)", Q_NULLPTR));
        checkBox_AreaDenOMBB->setText(QApplication::translate("popup_Morph", "Area Density (OMBB)", Q_NULLPTR));
        checkBox_VolDenAEE->setText(QApplication::translate("popup_Morph", "Volume Density (AEE)", Q_NULLPTR));
        checkBox_VolDenCH->setText(QApplication::translate("popup_Morph", "Volume Density (CH)", Q_NULLPTR));
        checkBox_AreaDenCH->setText(QApplication::translate("popup_Morph", "Area Density (CH)", Q_NULLPTR));
        checkBox_MoransI->setText(QApplication::translate("popup_Morph", "Moran's I Index", Q_NULLPTR));
        checkBox_GearysC->setText(QApplication::translate("popup_Morph", "Geary's C Measure", Q_NULLPTR));
        checkBox_IntegratedIntensity->setText(QApplication::translate("popup_Morph", "Integrated Intensity", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class popup_Morph: public Ui_popup_Morph {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POPUP_MORPH_H
