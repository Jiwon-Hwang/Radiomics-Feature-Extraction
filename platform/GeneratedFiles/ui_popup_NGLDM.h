/********************************************************************************
** Form generated from reading UI file 'popup_NGLDM.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POPUP_NGLDM_H
#define UI_POPUP_NGLDM_H

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

class Ui_popup_NGLDM
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox_Features;
    QGridLayout *gridLayout;
    QCheckBox *checkBox_GNUN;
    QCheckBox *checkBox_LRHE;
    QCheckBox *checkBox_LRE;
    QCheckBox *checkBox_RLV;
    QCheckBox *checkBox_HGRE;
    QCheckBox *checkBox_LRLE;
    QCheckBox *checkBox_SRLE;
    QCheckBox *checkBox_GLV;
    QCheckBox *checkBox_SRE;
    QCheckBox *checkBox_All;
    QCheckBox *checkBox_LGRE;
    QCheckBox *checkBox_RLNU;
    QCheckBox *checkBox_RE;
    QCheckBox *checkBox_GNU;
    QCheckBox *checkBox_RLNUN;
    QCheckBox *checkBox_SRHE;
    QCheckBox *checkBox_RP;
    QCheckBox *checkBox_Energy;
    QGroupBox *groupBox_Bins;
    QGridLayout *gridLayout_3;
    QLineEdit *lineEdit_sBin;
    QComboBox *comboBox_nBins;
    QRadioButton *radioButton_FBN;
    QRadioButton *radioButton_FBS;

    void setupUi(QDialog *popup_NGLDM)
    {
        if (popup_NGLDM->objectName().isEmpty())
            popup_NGLDM->setObjectName(QStringLiteral("popup_NGLDM"));
        popup_NGLDM->resize(480, 770);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(popup_NGLDM->sizePolicy().hasHeightForWidth());
        popup_NGLDM->setSizePolicy(sizePolicy);
        buttonBox = new QDialogButtonBox(popup_NGLDM);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(310, 730, 151, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);
        groupBox_Features = new QGroupBox(popup_NGLDM);
        groupBox_Features->setObjectName(QStringLiteral("groupBox_Features"));
        groupBox_Features->setGeometry(QRect(20, 30, 441, 580));
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

        checkBox_LRHE = new QCheckBox(groupBox_Features);
        checkBox_LRHE->setObjectName(QStringLiteral("checkBox_LRHE"));
        checkBox_LRHE->setEnabled(true);
        checkBox_LRHE->setChecked(false);

        gridLayout->addWidget(checkBox_LRHE, 6, 1, 1, 1);

        checkBox_LRE = new QCheckBox(groupBox_Features);
        checkBox_LRE->setObjectName(QStringLiteral("checkBox_LRE"));
        checkBox_LRE->setEnabled(true);
        checkBox_LRE->setCheckable(true);
        checkBox_LRE->setChecked(false);
        checkBox_LRE->setTristate(false);

        gridLayout->addWidget(checkBox_LRE, 1, 1, 1, 1);

        checkBox_RLV = new QCheckBox(groupBox_Features);
        checkBox_RLV->setObjectName(QStringLiteral("checkBox_RLV"));
        checkBox_RLV->setEnabled(true);
        QFont font;
        font.setFamily(QString::fromUtf8("\352\265\264\353\246\274"));
        font.setPointSize(9);
        checkBox_RLV->setFont(font);
        checkBox_RLV->setAutoFillBackground(false);
        checkBox_RLV->setChecked(false);
        checkBox_RLV->setTristate(false);

        gridLayout->addWidget(checkBox_RLV, 10, 0, 1, 1);

        checkBox_HGRE = new QCheckBox(groupBox_Features);
        checkBox_HGRE->setObjectName(QStringLiteral("checkBox_HGRE"));
        checkBox_HGRE->setEnabled(true);
        checkBox_HGRE->setChecked(false);

        gridLayout->addWidget(checkBox_HGRE, 2, 1, 1, 1);

        checkBox_LRLE = new QCheckBox(groupBox_Features);
        checkBox_LRLE->setObjectName(QStringLiteral("checkBox_LRLE"));
        checkBox_LRLE->setEnabled(true);
        checkBox_LRLE->setChecked(false);

        gridLayout->addWidget(checkBox_LRLE, 6, 0, 1, 1);

        checkBox_SRLE = new QCheckBox(groupBox_Features);
        checkBox_SRLE->setObjectName(QStringLiteral("checkBox_SRLE"));
        checkBox_SRLE->setEnabled(true);
        checkBox_SRLE->setChecked(false);

        gridLayout->addWidget(checkBox_SRLE, 5, 0, 1, 1);

        checkBox_GLV = new QCheckBox(groupBox_Features);
        checkBox_GLV->setObjectName(QStringLiteral("checkBox_GLV"));
        checkBox_GLV->setEnabled(true);
        checkBox_GLV->setFont(font);
        checkBox_GLV->setAcceptDrops(false);
        checkBox_GLV->setAutoFillBackground(false);
        checkBox_GLV->setChecked(false);
        checkBox_GLV->setTristate(false);

        gridLayout->addWidget(checkBox_GLV, 9, 1, 1, 1);

        checkBox_SRE = new QCheckBox(groupBox_Features);
        checkBox_SRE->setObjectName(QStringLiteral("checkBox_SRE"));
        checkBox_SRE->setAutoFillBackground(false);
        checkBox_SRE->setChecked(false);
        checkBox_SRE->setTristate(false);

        gridLayout->addWidget(checkBox_SRE, 1, 0, 1, 1);

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

        checkBox_LGRE = new QCheckBox(groupBox_Features);
        checkBox_LGRE->setObjectName(QStringLiteral("checkBox_LGRE"));
        checkBox_LGRE->setEnabled(true);

        gridLayout->addWidget(checkBox_LGRE, 2, 0, 1, 1);

        checkBox_RLNU = new QCheckBox(groupBox_Features);
        checkBox_RLNU->setObjectName(QStringLiteral("checkBox_RLNU"));
        checkBox_RLNU->setEnabled(true);
        checkBox_RLNU->setAutoFillBackground(false);
        checkBox_RLNU->setChecked(false);
        checkBox_RLNU->setTristate(false);

        gridLayout->addWidget(checkBox_RLNU, 8, 0, 1, 1);

        checkBox_RE = new QCheckBox(groupBox_Features);
        checkBox_RE->setObjectName(QStringLiteral("checkBox_RE"));
        checkBox_RE->setEnabled(true);
        checkBox_RE->setFont(font);
        checkBox_RE->setAutoFillBackground(false);
        checkBox_RE->setChecked(false);
        checkBox_RE->setTristate(false);

        gridLayout->addWidget(checkBox_RE, 10, 1, 1, 1);

        checkBox_GNU = new QCheckBox(groupBox_Features);
        checkBox_GNU->setObjectName(QStringLiteral("checkBox_GNU"));
        checkBox_GNU->setEnabled(true);
        checkBox_GNU->setAutoFillBackground(false);
        checkBox_GNU->setChecked(false);
        checkBox_GNU->setTristate(false);

        gridLayout->addWidget(checkBox_GNU, 7, 0, 1, 1);

        checkBox_RLNUN = new QCheckBox(groupBox_Features);
        checkBox_RLNUN->setObjectName(QStringLiteral("checkBox_RLNUN"));
        checkBox_RLNUN->setEnabled(true);
        checkBox_RLNUN->setAutoFillBackground(false);
        checkBox_RLNUN->setChecked(false);
        checkBox_RLNUN->setTristate(false);

        gridLayout->addWidget(checkBox_RLNUN, 8, 1, 1, 1);

        checkBox_SRHE = new QCheckBox(groupBox_Features);
        checkBox_SRHE->setObjectName(QStringLiteral("checkBox_SRHE"));
        checkBox_SRHE->setEnabled(true);
        checkBox_SRHE->setChecked(false);

        gridLayout->addWidget(checkBox_SRHE, 5, 1, 1, 1);

        checkBox_RP = new QCheckBox(groupBox_Features);
        checkBox_RP->setObjectName(QStringLiteral("checkBox_RP"));
        checkBox_RP->setEnabled(true);
        checkBox_RP->setFont(font);
        checkBox_RP->setAutoFillBackground(false);
        checkBox_RP->setChecked(false);
        checkBox_RP->setTristate(false);

        gridLayout->addWidget(checkBox_RP, 9, 0, 1, 1);

        checkBox_Energy = new QCheckBox(groupBox_Features);
        checkBox_Energy->setObjectName(QStringLiteral("checkBox_Energy"));
        checkBox_Energy->setEnabled(true);
        checkBox_Energy->setFont(font);
        checkBox_Energy->setAutoFillBackground(false);
        checkBox_Energy->setChecked(false);
        checkBox_Energy->setTristate(false);

        gridLayout->addWidget(checkBox_Energy, 11, 0, 1, 1);

        groupBox_Bins = new QGroupBox(popup_NGLDM);
        groupBox_Bins->setObjectName(QStringLiteral("groupBox_Bins"));
        groupBox_Bins->setGeometry(QRect(20, 620, 441, 100));
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


        retranslateUi(popup_NGLDM);
        QObject::connect(buttonBox, SIGNAL(accepted()), popup_NGLDM, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), popup_NGLDM, SLOT(reject()));

        QMetaObject::connectSlotsByName(popup_NGLDM);
    } // setupUi

    void retranslateUi(QDialog *popup_NGLDM)
    {
        popup_NGLDM->setWindowTitle(QApplication::translate("popup_NGLDM", "Set Measurements", Q_NULLPTR));
        groupBox_Features->setTitle(QApplication::translate("popup_NGLDM", "Features", Q_NULLPTR));
        checkBox_GNUN->setText(QApplication::translate("popup_NGLDM", "Grey Non-Uniform Norm", Q_NULLPTR));
        checkBox_LRHE->setText(QApplication::translate("popup_NGLDM", "High Depend High Emphasis", Q_NULLPTR));
        checkBox_LRE->setText(QApplication::translate("popup_NGLDM", "High Dependence Emphasis", Q_NULLPTR));
        checkBox_RLV->setText(QApplication::translate("popup_NGLDM", "Depend Count Variance", Q_NULLPTR));
        checkBox_HGRE->setText(QApplication::translate("popup_NGLDM", "High Grey Count Emphasis", Q_NULLPTR));
        checkBox_LRLE->setText(QApplication::translate("popup_NGLDM", "High Depend Low Emphasis", Q_NULLPTR));
        checkBox_SRLE->setText(QApplication::translate("popup_NGLDM", "Low Depend Low Emphasis", Q_NULLPTR));
        checkBox_GLV->setText(QApplication::translate("popup_NGLDM", "Grey Level Variance", Q_NULLPTR));
        checkBox_SRE->setText(QApplication::translate("popup_NGLDM", "Low Dependence Emphasis", Q_NULLPTR));
        checkBox_All->setText(QApplication::translate("popup_NGLDM", "All", Q_NULLPTR));
        checkBox_LGRE->setText(QApplication::translate("popup_NGLDM", "Low Grey Count Emphasis", Q_NULLPTR));
        checkBox_RLNU->setText(QApplication::translate("popup_NGLDM", "Depend Count Non-Uniform", Q_NULLPTR));
        checkBox_RE->setText(QApplication::translate("popup_NGLDM", "Depend Count Entropy", Q_NULLPTR));
        checkBox_GNU->setText(QApplication::translate("popup_NGLDM", "Grey Non-Uniform", Q_NULLPTR));
        checkBox_RLNUN->setText(QApplication::translate("popup_NGLDM", "Depend Count Non-Uniform Norm", Q_NULLPTR));
        checkBox_SRHE->setText(QApplication::translate("popup_NGLDM", "Low Depend High Emphasis", Q_NULLPTR));
        checkBox_RP->setText(QApplication::translate("popup_NGLDM", "Depend Count Percentage", Q_NULLPTR));
        checkBox_Energy->setText(QApplication::translate("popup_NGLDM", "Depend Count Energy", Q_NULLPTR));
        groupBox_Bins->setTitle(QApplication::translate("popup_NGLDM", "Discretization", Q_NULLPTR));
        comboBox_nBins->clear();
        comboBox_nBins->insertItems(0, QStringList()
         << QApplication::translate("popup_NGLDM", "8", Q_NULLPTR)
         << QApplication::translate("popup_NGLDM", "16", Q_NULLPTR)
         << QApplication::translate("popup_NGLDM", "32", Q_NULLPTR)
         << QApplication::translate("popup_NGLDM", "64", Q_NULLPTR)
        );
        comboBox_nBins->setCurrentText(QApplication::translate("popup_NGLDM", "8", Q_NULLPTR));
        radioButton_FBN->setText(QApplication::translate("popup_NGLDM", "Fixed Bin Number (FBN)      ", Q_NULLPTR));
        radioButton_FBS->setText(QApplication::translate("popup_NGLDM", "Fixed Bin Size (FBS)", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class popup_NGLDM: public Ui_popup_NGLDM {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POPUP_NGLDM_H
