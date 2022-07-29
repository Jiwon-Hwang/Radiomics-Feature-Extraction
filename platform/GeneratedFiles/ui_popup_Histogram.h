/********************************************************************************
** Form generated from reading UI file 'popup_Histogram.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POPUP_HISTOGRAM_H
#define UI_POPUP_HISTOGRAM_H

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

class Ui_popup_Histogram
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox_Features;
    QGridLayout *gridLayout;
    QCheckBox *checkBox_MinHistGradient;
    QCheckBox *checkBox_90thPercentile;
    QCheckBox *checkBox_MinHistGradGrey;
    QCheckBox *checkBox_Minimum;
    QCheckBox *checkBox_Maximum;
    QCheckBox *checkBox_InterquartileRange;
    QCheckBox *checkBox_Skewness;
    QCheckBox *checkBox_10thPercentile;
    QCheckBox *checkBox_Median;
    QCheckBox *checkBox_Mode;
    QCheckBox *checkBox_MedianAbsDev;
    QCheckBox *checkBox_Entropy;
    QCheckBox *checkBox_Uniformity;
    QCheckBox *checkBox_QuartileCoeff;
    QCheckBox *checkBox_MaxHistGradGrey;
    QCheckBox *checkBox_Range;
    QCheckBox *checkBox_Kurtosis;
    QCheckBox *checkBox_CoeffOfVar;
    QCheckBox *checkBox_Mean;
    QCheckBox *checkBox_Variance;
    QCheckBox *checkBox_RobustMeanAbsDev;
    QCheckBox *checkBox_MaxHisGradient;
    QCheckBox *checkBox_MeanAbsDev;
    QCheckBox *checkBox_All;
    QGroupBox *groupBox_Bins;
    QGridLayout *gridLayout_3;
    QLineEdit *lineEdit_sBin;
    QComboBox *comboBox_nBins;
    QRadioButton *radioButton_FBN;
    QRadioButton *radioButton_FBS;

    void setupUi(QDialog *popup_Histogram)
    {
        if (popup_Histogram->objectName().isEmpty())
            popup_Histogram->setObjectName(QStringLiteral("popup_Histogram"));
        popup_Histogram->resize(400, 770);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(popup_Histogram->sizePolicy().hasHeightForWidth());
        popup_Histogram->setSizePolicy(sizePolicy);
        buttonBox = new QDialogButtonBox(popup_Histogram);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(230, 730, 151, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);
        groupBox_Features = new QGroupBox(popup_Histogram);
        groupBox_Features->setObjectName(QStringLiteral("groupBox_Features"));
        groupBox_Features->setGeometry(QRect(20, 30, 360, 580));
        groupBox_Features->setCheckable(false);
        gridLayout = new QGridLayout(groupBox_Features);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(12, 10, 12, 10);
        checkBox_MinHistGradient = new QCheckBox(groupBox_Features);
        checkBox_MinHistGradient->setObjectName(QStringLiteral("checkBox_MinHistGradient"));
        checkBox_MinHistGradient->setEnabled(true);
        checkBox_MinHistGradient->setAutoFillBackground(false);
        checkBox_MinHistGradient->setChecked(false);
        checkBox_MinHistGradient->setTristate(false);

        gridLayout->addWidget(checkBox_MinHistGradient, 13, 1, 1, 1);

        checkBox_90thPercentile = new QCheckBox(groupBox_Features);
        checkBox_90thPercentile->setObjectName(QStringLiteral("checkBox_90thPercentile"));
        checkBox_90thPercentile->setEnabled(true);
        checkBox_90thPercentile->setChecked(false);

        gridLayout->addWidget(checkBox_90thPercentile, 6, 1, 1, 1);

        checkBox_MinHistGradGrey = new QCheckBox(groupBox_Features);
        checkBox_MinHistGradGrey->setObjectName(QStringLiteral("checkBox_MinHistGradGrey"));
        checkBox_MinHistGradGrey->setEnabled(true);
        checkBox_MinHistGradGrey->setAutoFillBackground(false);
        checkBox_MinHistGradGrey->setChecked(false);
        checkBox_MinHistGradGrey->setTristate(false);

        gridLayout->addWidget(checkBox_MinHistGradGrey, 14, 0, 1, 1);

        checkBox_Minimum = new QCheckBox(groupBox_Features);
        checkBox_Minimum->setObjectName(QStringLiteral("checkBox_Minimum"));
        checkBox_Minimum->setEnabled(true);
        checkBox_Minimum->setChecked(false);

        gridLayout->addWidget(checkBox_Minimum, 5, 1, 1, 1);

        checkBox_Maximum = new QCheckBox(groupBox_Features);
        checkBox_Maximum->setObjectName(QStringLiteral("checkBox_Maximum"));
        checkBox_Maximum->setAutoFillBackground(false);
        checkBox_Maximum->setChecked(false);
        checkBox_Maximum->setTristate(false);

        gridLayout->addWidget(checkBox_Maximum, 7, 0, 1, 1);

        checkBox_InterquartileRange = new QCheckBox(groupBox_Features);
        checkBox_InterquartileRange->setObjectName(QStringLiteral("checkBox_InterquartileRange"));
        checkBox_InterquartileRange->setEnabled(true);
        checkBox_InterquartileRange->setCheckable(true);
        checkBox_InterquartileRange->setChecked(false);
        checkBox_InterquartileRange->setTristate(false);

        gridLayout->addWidget(checkBox_InterquartileRange, 7, 1, 1, 1);

        checkBox_Skewness = new QCheckBox(groupBox_Features);
        checkBox_Skewness->setObjectName(QStringLiteral("checkBox_Skewness"));
        checkBox_Skewness->setEnabled(true);

        gridLayout->addWidget(checkBox_Skewness, 2, 0, 1, 1);

        checkBox_10thPercentile = new QCheckBox(groupBox_Features);
        checkBox_10thPercentile->setObjectName(QStringLiteral("checkBox_10thPercentile"));
        checkBox_10thPercentile->setEnabled(true);
        checkBox_10thPercentile->setChecked(false);

        gridLayout->addWidget(checkBox_10thPercentile, 6, 0, 1, 1);

        checkBox_Median = new QCheckBox(groupBox_Features);
        checkBox_Median->setObjectName(QStringLiteral("checkBox_Median"));
        checkBox_Median->setEnabled(true);
        checkBox_Median->setChecked(false);

        gridLayout->addWidget(checkBox_Median, 5, 0, 1, 1);

        checkBox_Mode = new QCheckBox(groupBox_Features);
        checkBox_Mode->setObjectName(QStringLiteral("checkBox_Mode"));
        checkBox_Mode->setEnabled(true);
        checkBox_Mode->setAutoFillBackground(false);
        checkBox_Mode->setChecked(false);
        checkBox_Mode->setTristate(false);

        gridLayout->addWidget(checkBox_Mode, 8, 0, 1, 1);

        checkBox_MedianAbsDev = new QCheckBox(groupBox_Features);
        checkBox_MedianAbsDev->setObjectName(QStringLiteral("checkBox_MedianAbsDev"));
        checkBox_MedianAbsDev->setEnabled(true);
        QFont font;
        font.setFamily(QString::fromUtf8("\352\265\264\353\246\274"));
        font.setPointSize(8);
        checkBox_MedianAbsDev->setFont(font);
        checkBox_MedianAbsDev->setAutoFillBackground(false);
        checkBox_MedianAbsDev->setChecked(false);
        checkBox_MedianAbsDev->setTristate(false);

        gridLayout->addWidget(checkBox_MedianAbsDev, 10, 0, 1, 1);

        checkBox_Entropy = new QCheckBox(groupBox_Features);
        checkBox_Entropy->setObjectName(QStringLiteral("checkBox_Entropy"));
        checkBox_Entropy->setEnabled(true);
        checkBox_Entropy->setAutoFillBackground(false);
        checkBox_Entropy->setChecked(false);
        checkBox_Entropy->setTristate(false);

        gridLayout->addWidget(checkBox_Entropy, 11, 1, 1, 1);

        checkBox_Uniformity = new QCheckBox(groupBox_Features);
        checkBox_Uniformity->setObjectName(QStringLiteral("checkBox_Uniformity"));
        checkBox_Uniformity->setEnabled(true);
        checkBox_Uniformity->setAutoFillBackground(false);
        checkBox_Uniformity->setChecked(false);
        checkBox_Uniformity->setTristate(false);

        gridLayout->addWidget(checkBox_Uniformity, 12, 0, 1, 1);

        checkBox_QuartileCoeff = new QCheckBox(groupBox_Features);
        checkBox_QuartileCoeff->setObjectName(QStringLiteral("checkBox_QuartileCoeff"));
        checkBox_QuartileCoeff->setEnabled(true);
        checkBox_QuartileCoeff->setAutoFillBackground(false);
        checkBox_QuartileCoeff->setChecked(false);
        checkBox_QuartileCoeff->setTristate(false);

        gridLayout->addWidget(checkBox_QuartileCoeff, 11, 0, 1, 1);

        checkBox_MaxHistGradGrey = new QCheckBox(groupBox_Features);
        checkBox_MaxHistGradGrey->setObjectName(QStringLiteral("checkBox_MaxHistGradGrey"));
        checkBox_MaxHistGradGrey->setEnabled(true);
        checkBox_MaxHistGradGrey->setAutoFillBackground(false);
        checkBox_MaxHistGradGrey->setChecked(false);
        checkBox_MaxHistGradGrey->setTristate(false);

        gridLayout->addWidget(checkBox_MaxHistGradGrey, 13, 0, 1, 1);

        checkBox_Range = new QCheckBox(groupBox_Features);
        checkBox_Range->setObjectName(QStringLiteral("checkBox_Range"));
        checkBox_Range->setEnabled(true);
        checkBox_Range->setAutoFillBackground(false);
        checkBox_Range->setChecked(false);
        checkBox_Range->setTristate(false);

        gridLayout->addWidget(checkBox_Range, 8, 1, 1, 1);

        checkBox_Kurtosis = new QCheckBox(groupBox_Features);
        checkBox_Kurtosis->setObjectName(QStringLiteral("checkBox_Kurtosis"));
        checkBox_Kurtosis->setEnabled(true);
        checkBox_Kurtosis->setChecked(false);

        gridLayout->addWidget(checkBox_Kurtosis, 2, 1, 1, 1);

        checkBox_CoeffOfVar = new QCheckBox(groupBox_Features);
        checkBox_CoeffOfVar->setObjectName(QStringLiteral("checkBox_CoeffOfVar"));
        checkBox_CoeffOfVar->setEnabled(true);
        checkBox_CoeffOfVar->setFont(font);
        checkBox_CoeffOfVar->setAutoFillBackground(false);
        checkBox_CoeffOfVar->setChecked(false);
        checkBox_CoeffOfVar->setTristate(false);

        gridLayout->addWidget(checkBox_CoeffOfVar, 10, 1, 1, 1);

        checkBox_Mean = new QCheckBox(groupBox_Features);
        checkBox_Mean->setObjectName(QStringLiteral("checkBox_Mean"));
        checkBox_Mean->setAutoFillBackground(false);
        checkBox_Mean->setChecked(false);
        checkBox_Mean->setTristate(false);

        gridLayout->addWidget(checkBox_Mean, 1, 0, 1, 1);

        checkBox_Variance = new QCheckBox(groupBox_Features);
        checkBox_Variance->setObjectName(QStringLiteral("checkBox_Variance"));
        checkBox_Variance->setEnabled(true);
        checkBox_Variance->setCheckable(true);
        checkBox_Variance->setChecked(false);
        checkBox_Variance->setTristate(false);

        gridLayout->addWidget(checkBox_Variance, 1, 1, 1, 1);

        checkBox_RobustMeanAbsDev = new QCheckBox(groupBox_Features);
        checkBox_RobustMeanAbsDev->setObjectName(QStringLiteral("checkBox_RobustMeanAbsDev"));
        checkBox_RobustMeanAbsDev->setEnabled(true);
        checkBox_RobustMeanAbsDev->setFont(font);
        checkBox_RobustMeanAbsDev->setAcceptDrops(false);
        checkBox_RobustMeanAbsDev->setAutoFillBackground(false);
        checkBox_RobustMeanAbsDev->setChecked(false);
        checkBox_RobustMeanAbsDev->setTristate(false);

        gridLayout->addWidget(checkBox_RobustMeanAbsDev, 9, 1, 1, 1);

        checkBox_MaxHisGradient = new QCheckBox(groupBox_Features);
        checkBox_MaxHisGradient->setObjectName(QStringLiteral("checkBox_MaxHisGradient"));
        checkBox_MaxHisGradient->setEnabled(true);
        checkBox_MaxHisGradient->setAutoFillBackground(false);
        checkBox_MaxHisGradient->setChecked(false);
        checkBox_MaxHisGradient->setTristate(false);

        gridLayout->addWidget(checkBox_MaxHisGradient, 12, 1, 1, 1);

        checkBox_MeanAbsDev = new QCheckBox(groupBox_Features);
        checkBox_MeanAbsDev->setObjectName(QStringLiteral("checkBox_MeanAbsDev"));
        checkBox_MeanAbsDev->setEnabled(true);
        checkBox_MeanAbsDev->setFont(font);
        checkBox_MeanAbsDev->setAutoFillBackground(false);
        checkBox_MeanAbsDev->setChecked(false);
        checkBox_MeanAbsDev->setTristate(false);

        gridLayout->addWidget(checkBox_MeanAbsDev, 9, 0, 1, 1);

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

        groupBox_Bins = new QGroupBox(popup_Histogram);
        groupBox_Bins->setObjectName(QStringLiteral("groupBox_Bins"));
        groupBox_Bins->setGeometry(QRect(20, 620, 360, 100));
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


        retranslateUi(popup_Histogram);
        QObject::connect(buttonBox, SIGNAL(accepted()), popup_Histogram, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), popup_Histogram, SLOT(reject()));

        QMetaObject::connectSlotsByName(popup_Histogram);
    } // setupUi

    void retranslateUi(QDialog *popup_Histogram)
    {
        popup_Histogram->setWindowTitle(QApplication::translate("popup_Histogram", "Set Measurements", Q_NULLPTR));
        groupBox_Features->setTitle(QApplication::translate("popup_Histogram", "Features", Q_NULLPTR));
        checkBox_MinHistGradient->setText(QApplication::translate("popup_Histogram", "Min Hist Gradient", Q_NULLPTR));
        checkBox_90thPercentile->setText(QApplication::translate("popup_Histogram", "90th Percentile", Q_NULLPTR));
        checkBox_MinHistGradGrey->setText(QApplication::translate("popup_Histogram", "Min Hist Gradient Grey", Q_NULLPTR));
        checkBox_Minimum->setText(QApplication::translate("popup_Histogram", "Minimum", Q_NULLPTR));
        checkBox_Maximum->setText(QApplication::translate("popup_Histogram", "Maximum", Q_NULLPTR));
        checkBox_InterquartileRange->setText(QApplication::translate("popup_Histogram", "Interquartile range", Q_NULLPTR));
        checkBox_Skewness->setText(QApplication::translate("popup_Histogram", "Skewness", Q_NULLPTR));
        checkBox_10thPercentile->setText(QApplication::translate("popup_Histogram", "10th Percentile", Q_NULLPTR));
        checkBox_Median->setText(QApplication::translate("popup_Histogram", "Median", Q_NULLPTR));
        checkBox_Mode->setText(QApplication::translate("popup_Histogram", "Mode", Q_NULLPTR));
        checkBox_MedianAbsDev->setText(QApplication::translate("popup_Histogram", "Median Absolute Deviation", Q_NULLPTR));
        checkBox_Entropy->setText(QApplication::translate("popup_Histogram", "Entropy", Q_NULLPTR));
        checkBox_Uniformity->setText(QApplication::translate("popup_Histogram", "Uniformity", Q_NULLPTR));
        checkBox_QuartileCoeff->setText(QApplication::translate("popup_Histogram", "Quartile Coefficient", Q_NULLPTR));
        checkBox_MaxHistGradGrey->setText(QApplication::translate("popup_Histogram", "Max Hist Gradient Grey", Q_NULLPTR));
        checkBox_Range->setText(QApplication::translate("popup_Histogram", "Range", Q_NULLPTR));
        checkBox_Kurtosis->setText(QApplication::translate("popup_Histogram", "Kurtosis", Q_NULLPTR));
        checkBox_CoeffOfVar->setText(QApplication::translate("popup_Histogram", "Coefficient of Variation", Q_NULLPTR));
        checkBox_Mean->setText(QApplication::translate("popup_Histogram", "Mean", Q_NULLPTR));
        checkBox_Variance->setText(QApplication::translate("popup_Histogram", "Variance", Q_NULLPTR));
        checkBox_RobustMeanAbsDev->setText(QApplication::translate("popup_Histogram", "Robust Mean Abs Dev", Q_NULLPTR));
        checkBox_MaxHisGradient->setText(QApplication::translate("popup_Histogram", "Max Hist Gradient", Q_NULLPTR));
        checkBox_MeanAbsDev->setText(QApplication::translate("popup_Histogram", "Mean Absolute Deviation", Q_NULLPTR));
        checkBox_All->setText(QApplication::translate("popup_Histogram", "All", Q_NULLPTR));
        groupBox_Bins->setTitle(QApplication::translate("popup_Histogram", "Discretization", Q_NULLPTR));
        comboBox_nBins->clear();
        comboBox_nBins->insertItems(0, QStringList()
         << QApplication::translate("popup_Histogram", "8", Q_NULLPTR)
         << QApplication::translate("popup_Histogram", "16", Q_NULLPTR)
         << QApplication::translate("popup_Histogram", "32", Q_NULLPTR)
         << QApplication::translate("popup_Histogram", "64", Q_NULLPTR)
        );
        comboBox_nBins->setCurrentText(QApplication::translate("popup_Histogram", "8", Q_NULLPTR));
        radioButton_FBN->setText(QApplication::translate("popup_Histogram", "Fixed Bin Number (FBN)  ", Q_NULLPTR));
        radioButton_FBS->setText(QApplication::translate("popup_Histogram", "Fixed Bin Size (FBS)   ", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class popup_Histogram: public Ui_popup_Histogram {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POPUP_HISTOGRAM_H
