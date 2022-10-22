/********************************************************************************
** Form generated from reading UI file 'popup_GLSZM.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POPUP_GLSZM_H
#define UI_POPUP_GLSZM_H

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

class Ui_popup_GLSZM
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox_Features;
    QGridLayout *gridLayout;
    QCheckBox *checkBox_GNUN;
    QCheckBox *checkBox_HGZE;
    QCheckBox *checkBox_SZE;
    QCheckBox *checkBox_ZSV;
    QCheckBox *checkBox_GLV;
    QCheckBox *checkBox_GNU;
    QCheckBox *checkBox_ZSNU;
    QCheckBox *checkBox_LZHGE;
    QCheckBox *checkBox_ZSNUN;
    QCheckBox *checkBox_ZSE;
    QCheckBox *checkBox_SZHGE;
    QCheckBox *checkBox_LZLGE;
    QCheckBox *checkBox_LZE;
    QCheckBox *checkBox_LGZE;
    QCheckBox *checkBox_SZLGE;
    QCheckBox *checkBox_ZP;
    QCheckBox *checkBox_All;
    QGroupBox *groupBox_Bins;
    QGridLayout *gridLayout_4;
    QLineEdit *lineEdit_sBin;
    QComboBox *comboBox_nBins;
    QRadioButton *radioButton_FBN;
    QRadioButton *radioButton_FBS;

    void setupUi(QDialog *popup_GLSZM)
    {
        if (popup_GLSZM->objectName().isEmpty())
            popup_GLSZM->setObjectName(QStringLiteral("popup_GLSZM"));
        popup_GLSZM->resize(440, 770);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(popup_GLSZM->sizePolicy().hasHeightForWidth());
        popup_GLSZM->setSizePolicy(sizePolicy);
        buttonBox = new QDialogButtonBox(popup_GLSZM);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(270, 730, 151, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);
        groupBox_Features = new QGroupBox(popup_GLSZM);
        groupBox_Features->setObjectName(QStringLiteral("groupBox_Features"));
        groupBox_Features->setGeometry(QRect(20, 30, 401, 580));
        groupBox_Features->setCheckable(false);
        gridLayout = new QGridLayout(groupBox_Features);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(12, 10, 6, 10);
        checkBox_GNUN = new QCheckBox(groupBox_Features);
        checkBox_GNUN->setObjectName(QStringLiteral("checkBox_GNUN"));
        checkBox_GNUN->setEnabled(true);
        checkBox_GNUN->setCheckable(true);
        checkBox_GNUN->setChecked(false);
        checkBox_GNUN->setTristate(false);

        gridLayout->addWidget(checkBox_GNUN, 7, 1, 1, 1);

        checkBox_HGZE = new QCheckBox(groupBox_Features);
        checkBox_HGZE->setObjectName(QStringLiteral("checkBox_HGZE"));
        checkBox_HGZE->setEnabled(true);
        checkBox_HGZE->setChecked(false);

        gridLayout->addWidget(checkBox_HGZE, 2, 1, 1, 1);

        checkBox_SZE = new QCheckBox(groupBox_Features);
        checkBox_SZE->setObjectName(QStringLiteral("checkBox_SZE"));
        checkBox_SZE->setAutoFillBackground(false);
        checkBox_SZE->setChecked(false);
        checkBox_SZE->setTristate(false);

        gridLayout->addWidget(checkBox_SZE, 1, 0, 1, 1);

        checkBox_ZSV = new QCheckBox(groupBox_Features);
        checkBox_ZSV->setObjectName(QStringLiteral("checkBox_ZSV"));
        checkBox_ZSV->setEnabled(true);
        QFont font;
        font.setFamily(QString::fromUtf8("\352\265\264\353\246\274"));
        font.setPointSize(9);
        checkBox_ZSV->setFont(font);
        checkBox_ZSV->setAutoFillBackground(false);
        checkBox_ZSV->setChecked(false);
        checkBox_ZSV->setTristate(false);

        gridLayout->addWidget(checkBox_ZSV, 10, 0, 1, 1);

        checkBox_GLV = new QCheckBox(groupBox_Features);
        checkBox_GLV->setObjectName(QStringLiteral("checkBox_GLV"));
        checkBox_GLV->setEnabled(true);
        checkBox_GLV->setFont(font);
        checkBox_GLV->setAcceptDrops(false);
        checkBox_GLV->setAutoFillBackground(false);
        checkBox_GLV->setChecked(false);
        checkBox_GLV->setTristate(false);

        gridLayout->addWidget(checkBox_GLV, 9, 1, 1, 1);

        checkBox_GNU = new QCheckBox(groupBox_Features);
        checkBox_GNU->setObjectName(QStringLiteral("checkBox_GNU"));
        checkBox_GNU->setEnabled(true);
        checkBox_GNU->setAutoFillBackground(false);
        checkBox_GNU->setChecked(false);
        checkBox_GNU->setTristate(false);

        gridLayout->addWidget(checkBox_GNU, 7, 0, 1, 1);

        checkBox_ZSNU = new QCheckBox(groupBox_Features);
        checkBox_ZSNU->setObjectName(QStringLiteral("checkBox_ZSNU"));
        checkBox_ZSNU->setEnabled(true);
        checkBox_ZSNU->setAutoFillBackground(false);
        checkBox_ZSNU->setChecked(false);
        checkBox_ZSNU->setTristate(false);

        gridLayout->addWidget(checkBox_ZSNU, 8, 0, 1, 1);

        checkBox_LZHGE = new QCheckBox(groupBox_Features);
        checkBox_LZHGE->setObjectName(QStringLiteral("checkBox_LZHGE"));
        checkBox_LZHGE->setEnabled(true);
        checkBox_LZHGE->setChecked(false);

        gridLayout->addWidget(checkBox_LZHGE, 6, 1, 1, 1);

        checkBox_ZSNUN = new QCheckBox(groupBox_Features);
        checkBox_ZSNUN->setObjectName(QStringLiteral("checkBox_ZSNUN"));
        checkBox_ZSNUN->setEnabled(true);
        checkBox_ZSNUN->setAutoFillBackground(false);
        checkBox_ZSNUN->setChecked(false);
        checkBox_ZSNUN->setTristate(false);

        gridLayout->addWidget(checkBox_ZSNUN, 8, 1, 1, 1);

        checkBox_ZSE = new QCheckBox(groupBox_Features);
        checkBox_ZSE->setObjectName(QStringLiteral("checkBox_ZSE"));
        checkBox_ZSE->setEnabled(true);
        checkBox_ZSE->setFont(font);
        checkBox_ZSE->setAutoFillBackground(false);
        checkBox_ZSE->setChecked(false);
        checkBox_ZSE->setTristate(false);

        gridLayout->addWidget(checkBox_ZSE, 10, 1, 1, 1);

        checkBox_SZHGE = new QCheckBox(groupBox_Features);
        checkBox_SZHGE->setObjectName(QStringLiteral("checkBox_SZHGE"));
        checkBox_SZHGE->setEnabled(true);
        checkBox_SZHGE->setChecked(false);

        gridLayout->addWidget(checkBox_SZHGE, 5, 1, 1, 1);

        checkBox_LZLGE = new QCheckBox(groupBox_Features);
        checkBox_LZLGE->setObjectName(QStringLiteral("checkBox_LZLGE"));
        checkBox_LZLGE->setEnabled(true);
        checkBox_LZLGE->setChecked(false);

        gridLayout->addWidget(checkBox_LZLGE, 6, 0, 1, 1);

        checkBox_LZE = new QCheckBox(groupBox_Features);
        checkBox_LZE->setObjectName(QStringLiteral("checkBox_LZE"));
        checkBox_LZE->setEnabled(true);
        checkBox_LZE->setCheckable(true);
        checkBox_LZE->setChecked(false);
        checkBox_LZE->setTristate(false);

        gridLayout->addWidget(checkBox_LZE, 1, 1, 1, 1);

        checkBox_LGZE = new QCheckBox(groupBox_Features);
        checkBox_LGZE->setObjectName(QStringLiteral("checkBox_LGZE"));
        checkBox_LGZE->setEnabled(true);

        gridLayout->addWidget(checkBox_LGZE, 2, 0, 1, 1);

        checkBox_SZLGE = new QCheckBox(groupBox_Features);
        checkBox_SZLGE->setObjectName(QStringLiteral("checkBox_SZLGE"));
        checkBox_SZLGE->setEnabled(true);
        checkBox_SZLGE->setChecked(false);

        gridLayout->addWidget(checkBox_SZLGE, 5, 0, 1, 1);

        checkBox_ZP = new QCheckBox(groupBox_Features);
        checkBox_ZP->setObjectName(QStringLiteral("checkBox_ZP"));
        checkBox_ZP->setEnabled(true);
        checkBox_ZP->setFont(font);
        checkBox_ZP->setAutoFillBackground(false);
        checkBox_ZP->setChecked(false);
        checkBox_ZP->setTristate(false);

        gridLayout->addWidget(checkBox_ZP, 9, 0, 1, 1);

        checkBox_All = new QCheckBox(groupBox_Features);
        checkBox_All->setObjectName(QStringLiteral("checkBox_All"));
        checkBox_All->setEnabled(true);
        QFont font1;
        font1.setBold(true);
        font1.setUnderline(false);
        font1.setWeight(75);
        font1.setKerning(true);
        checkBox_All->setFont(font1);
        checkBox_All->setAutoFillBackground(false);
        checkBox_All->setChecked(false);
        checkBox_All->setTristate(false);

        gridLayout->addWidget(checkBox_All, 0, 0, 1, 1);

        groupBox_Bins = new QGroupBox(popup_GLSZM);
        groupBox_Bins->setObjectName(QStringLiteral("groupBox_Bins"));
        groupBox_Bins->setGeometry(QRect(20, 620, 401, 100));
        gridLayout_4 = new QGridLayout(groupBox_Bins);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        lineEdit_sBin = new QLineEdit(groupBox_Bins);
        lineEdit_sBin->setObjectName(QStringLiteral("lineEdit_sBin"));
        lineEdit_sBin->setEnabled(false);

        gridLayout_4->addWidget(lineEdit_sBin, 2, 1, 1, 1);

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

        gridLayout_4->addWidget(comboBox_nBins, 0, 1, 1, 1);

        radioButton_FBN = new QRadioButton(groupBox_Bins);
        radioButton_FBN->setObjectName(QStringLiteral("radioButton_FBN"));
        radioButton_FBN->setChecked(true);

        gridLayout_4->addWidget(radioButton_FBN, 0, 0, 1, 1);

        radioButton_FBS = new QRadioButton(groupBox_Bins);
        radioButton_FBS->setObjectName(QStringLiteral("radioButton_FBS"));
        radioButton_FBS->setEnabled(true);

        gridLayout_4->addWidget(radioButton_FBS, 2, 0, 1, 1);


        retranslateUi(popup_GLSZM);
        QObject::connect(buttonBox, SIGNAL(accepted()), popup_GLSZM, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), popup_GLSZM, SLOT(reject()));

        QMetaObject::connectSlotsByName(popup_GLSZM);
    } // setupUi

    void retranslateUi(QDialog *popup_GLSZM)
    {
        popup_GLSZM->setWindowTitle(QApplication::translate("popup_GLSZM", "Set Measurements", Q_NULLPTR));
        groupBox_Features->setTitle(QApplication::translate("popup_GLSZM", "Features", Q_NULLPTR));
        checkBox_GNUN->setText(QApplication::translate("popup_GLSZM", "Grey Non-Uniform Norm", Q_NULLPTR));
        checkBox_HGZE->setText(QApplication::translate("popup_GLSZM", "High Grey Zone Emphasis", Q_NULLPTR));
        checkBox_SZE->setText(QApplication::translate("popup_GLSZM", "Small Zone Emphasis", Q_NULLPTR));
        checkBox_ZSV->setText(QApplication::translate("popup_GLSZM", "Zone Size Variance", Q_NULLPTR));
        checkBox_GLV->setText(QApplication::translate("popup_GLSZM", "Grey Level Variance", Q_NULLPTR));
        checkBox_GNU->setText(QApplication::translate("popup_GLSZM", "Grey Non-Uniform", Q_NULLPTR));
        checkBox_ZSNU->setText(QApplication::translate("popup_GLSZM", "Zone Size Non-Uniform", Q_NULLPTR));
        checkBox_LZHGE->setText(QApplication::translate("popup_GLSZM", "Large Zone High Grey Emph", Q_NULLPTR));
        checkBox_ZSNUN->setText(QApplication::translate("popup_GLSZM", "Zone Size Non-Uniform Norm", Q_NULLPTR));
        checkBox_ZSE->setText(QApplication::translate("popup_GLSZM", "Zone Size Entropy", Q_NULLPTR));
        checkBox_SZHGE->setText(QApplication::translate("popup_GLSZM", "Small Zone High Grey Emph", Q_NULLPTR));
        checkBox_LZLGE->setText(QApplication::translate("popup_GLSZM", "Large Zone Low Grey Emph", Q_NULLPTR));
        checkBox_LZE->setText(QApplication::translate("popup_GLSZM", "Large Zone Emphasis", Q_NULLPTR));
        checkBox_LGZE->setText(QApplication::translate("popup_GLSZM", "Low Grey Zone Emphasis", Q_NULLPTR));
        checkBox_SZLGE->setText(QApplication::translate("popup_GLSZM", "Small Zone Low Grey Emph", Q_NULLPTR));
        checkBox_ZP->setText(QApplication::translate("popup_GLSZM", "Zone Percentage", Q_NULLPTR));
        checkBox_All->setText(QApplication::translate("popup_GLSZM", "All", Q_NULLPTR));
        groupBox_Bins->setTitle(QApplication::translate("popup_GLSZM", "Discretization", Q_NULLPTR));
        comboBox_nBins->clear();
        comboBox_nBins->insertItems(0, QStringList()
         << QApplication::translate("popup_GLSZM", "8", Q_NULLPTR)
         << QApplication::translate("popup_GLSZM", "16", Q_NULLPTR)
         << QApplication::translate("popup_GLSZM", "32", Q_NULLPTR)
         << QApplication::translate("popup_GLSZM", "64", Q_NULLPTR)
        );
        comboBox_nBins->setCurrentText(QApplication::translate("popup_GLSZM", "8", Q_NULLPTR));
        radioButton_FBN->setText(QApplication::translate("popup_GLSZM", "Fixed Bin Number (FBN)  ", Q_NULLPTR));
        radioButton_FBS->setText(QApplication::translate("popup_GLSZM", "Fixed Bin Size (FBS)", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class popup_GLSZM: public Ui_popup_GLSZM {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POPUP_GLSZM_H
