/********************************************************************************
** Form generated from reading UI file 'popup_NGTDM.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POPUP_NGTDM_H
#define UI_POPUP_NGTDM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_popup_NGTDM
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox_Features;
    QGridLayout *gridLayout;
    QCheckBox *checkBox_Complexity;
    QCheckBox *checkBox_Coarseness;
    QCheckBox *checkBox_All;
    QCheckBox *checkBox_Contrast;
    QCheckBox *checkBox_Strength;
    QCheckBox *checkBox_Busyness;
    QGroupBox *groupBox_Bins;
    QGridLayout *gridLayout_3;
    QLineEdit *lineEdit_sBin;
    QComboBox *comboBox_nBins;
    QRadioButton *radioButton_FBN;
    QRadioButton *radioButton_FBS;

    void setupUi(QDialog *popup_NGTDM)
    {
        if (popup_NGTDM->objectName().isEmpty())
            popup_NGTDM->setObjectName(QStringLiteral("popup_NGTDM"));
        popup_NGTDM->resize(400, 520);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(popup_NGTDM->sizePolicy().hasHeightForWidth());
        popup_NGTDM->setSizePolicy(sizePolicy);
        buttonBox = new QDialogButtonBox(popup_NGTDM);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(230, 470, 151, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);
        groupBox_Features = new QGroupBox(popup_NGTDM);
        groupBox_Features->setObjectName(QStringLiteral("groupBox_Features"));
        groupBox_Features->setGeometry(QRect(20, 30, 361, 321));
        groupBox_Features->setCheckable(false);
        gridLayout = new QGridLayout(groupBox_Features);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(12, 10, 6, 10);
        checkBox_Complexity = new QCheckBox(groupBox_Features);
        checkBox_Complexity->setObjectName(QStringLiteral("checkBox_Complexity"));
        checkBox_Complexity->setEnabled(true);
        checkBox_Complexity->setChecked(false);

        gridLayout->addWidget(checkBox_Complexity, 2, 1, 1, 1);

        checkBox_Coarseness = new QCheckBox(groupBox_Features);
        checkBox_Coarseness->setObjectName(QStringLiteral("checkBox_Coarseness"));
        checkBox_Coarseness->setAutoFillBackground(false);
        checkBox_Coarseness->setChecked(false);
        checkBox_Coarseness->setTristate(false);

        gridLayout->addWidget(checkBox_Coarseness, 1, 0, 1, 1);

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

        checkBox_Contrast = new QCheckBox(groupBox_Features);
        checkBox_Contrast->setObjectName(QStringLiteral("checkBox_Contrast"));
        checkBox_Contrast->setEnabled(true);
        checkBox_Contrast->setCheckable(true);
        checkBox_Contrast->setChecked(false);
        checkBox_Contrast->setTristate(false);

        gridLayout->addWidget(checkBox_Contrast, 1, 1, 1, 1);

        checkBox_Strength = new QCheckBox(groupBox_Features);
        checkBox_Strength->setObjectName(QStringLiteral("checkBox_Strength"));
        checkBox_Strength->setEnabled(true);
        checkBox_Strength->setChecked(false);

        gridLayout->addWidget(checkBox_Strength, 5, 0, 1, 1);

        checkBox_Busyness = new QCheckBox(groupBox_Features);
        checkBox_Busyness->setObjectName(QStringLiteral("checkBox_Busyness"));
        checkBox_Busyness->setEnabled(true);

        gridLayout->addWidget(checkBox_Busyness, 2, 0, 1, 1);

        groupBox_Bins = new QGroupBox(popup_NGTDM);
        groupBox_Bins->setObjectName(QStringLiteral("groupBox_Bins"));
        groupBox_Bins->setGeometry(QRect(20, 360, 361, 100));
        gridLayout_3 = new QGridLayout(groupBox_Bins);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        lineEdit_sBin = new QLineEdit(groupBox_Bins);
        lineEdit_sBin->setObjectName(QStringLiteral("lineEdit_sBin"));
        lineEdit_sBin->setEnabled(false);

        gridLayout_3->addWidget(lineEdit_sBin, 2, 1, 1, 1);

        comboBox_nBins = new QComboBox(groupBox_Bins);
        comboBox_nBins->setObjectName(QStringLiteral("comboBox_nBins"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboBox_nBins->sizePolicy().hasHeightForWidth());
        comboBox_nBins->setSizePolicy(sizePolicy1);
        comboBox_nBins->setMaxVisibleItems(5);
        comboBox_nBins->setMaxCount(64);
        comboBox_nBins->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
        comboBox_nBins->setIconSize(QSize(16, 16));

        gridLayout_3->addWidget(comboBox_nBins, 0, 1, 1, 1);

        radioButton_FBN = new QRadioButton(groupBox_Bins);
        radioButton_FBN->setObjectName(QStringLiteral("radioButton_FBN"));
        radioButton_FBN->setChecked(true);

        gridLayout_3->addWidget(radioButton_FBN, 0, 0, 1, 1);

        radioButton_FBS = new QRadioButton(groupBox_Bins);
        radioButton_FBS->setObjectName(QStringLiteral("radioButton_FBS"));
        radioButton_FBS->setEnabled(true);

        gridLayout_3->addWidget(radioButton_FBS, 2, 0, 1, 1);


        retranslateUi(popup_NGTDM);
        QObject::connect(buttonBox, SIGNAL(accepted()), popup_NGTDM, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), popup_NGTDM, SLOT(reject()));

        QMetaObject::connectSlotsByName(popup_NGTDM);
    } // setupUi

    void retranslateUi(QDialog *popup_NGTDM)
    {
        popup_NGTDM->setWindowTitle(QApplication::translate("popup_NGTDM", "Set Measurements", Q_NULLPTR));
        groupBox_Features->setTitle(QApplication::translate("popup_NGTDM", "Features", Q_NULLPTR));
        checkBox_Complexity->setText(QApplication::translate("popup_NGTDM", "Complexity", Q_NULLPTR));
        checkBox_Coarseness->setText(QApplication::translate("popup_NGTDM", "Coarseness", Q_NULLPTR));
        checkBox_All->setText(QApplication::translate("popup_NGTDM", "All", Q_NULLPTR));
        checkBox_Contrast->setText(QApplication::translate("popup_NGTDM", "Contrast", Q_NULLPTR));
        checkBox_Strength->setText(QApplication::translate("popup_NGTDM", "Strength", Q_NULLPTR));
        checkBox_Busyness->setText(QApplication::translate("popup_NGTDM", "Busyness", Q_NULLPTR));
        groupBox_Bins->setTitle(QApplication::translate("popup_NGTDM", "Discretization", Q_NULLPTR));
        comboBox_nBins->clear();
        comboBox_nBins->insertItems(0, QStringList()
         << QApplication::translate("popup_NGTDM", "8", Q_NULLPTR)
         << QApplication::translate("popup_NGTDM", "16", Q_NULLPTR)
         << QApplication::translate("popup_NGTDM", "32", Q_NULLPTR)
         << QApplication::translate("popup_NGTDM", "64", Q_NULLPTR)
        );
        comboBox_nBins->setCurrentText(QApplication::translate("popup_NGTDM", "8", Q_NULLPTR));
        radioButton_FBN->setText(QApplication::translate("popup_NGTDM", "Fixed Bin Number (FBN)      ", Q_NULLPTR));
        radioButton_FBS->setText(QApplication::translate("popup_NGTDM", "Fixed Bin Size (FBS)", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class popup_NGTDM: public Ui_popup_NGTDM {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POPUP_NGTDM_H
