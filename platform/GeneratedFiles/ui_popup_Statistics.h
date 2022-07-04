/********************************************************************************
** Form generated from reading UI file 'popup_Statistics.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POPUP_STATISTICS_H
#define UI_POPUP_STATISTICS_H

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

class Ui_popup_Statistics
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox_Features;
    QGridLayout *gridLayout;
    QCheckBox *checkBox_RootMean;
    QCheckBox *checkBox_Kurtosis;
    QCheckBox *checkBox_10thPercentile;
    QCheckBox *checkBox_Median;
    QCheckBox *checkBox_All;
    QCheckBox *checkBox_Variance;
    QCheckBox *checkBox_Mean;
    QCheckBox *checkBox_Energy;
    QCheckBox *checkBox_RobustMeanAbsDev;
    QCheckBox *checkBox_CoeffOfVar;
    QCheckBox *checkBox_MedianAbsDev;
    QCheckBox *checkBox_Range;
    QCheckBox *checkBox_90thPercentile;
    QCheckBox *checkBox_Minimum;
    QCheckBox *checkBox_MeanAbsDev;
    QCheckBox *checkBox_Maximum;
    QCheckBox *checkBox_Skewness;
    QCheckBox *checkBox_InterquartileRange;
    QCheckBox *checkBox_QuartileCoeff;

    void setupUi(QDialog *popup_Statistics)
    {
        if (popup_Statistics->objectName().isEmpty())
            popup_Statistics->setObjectName(QStringLiteral("popup_Statistics"));
        popup_Statistics->resize(400, 680);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(popup_Statistics->sizePolicy().hasHeightForWidth());
        popup_Statistics->setSizePolicy(sizePolicy);
        buttonBox = new QDialogButtonBox(popup_Statistics);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(230, 630, 151, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);
        groupBox_Features = new QGroupBox(popup_Statistics);
        groupBox_Features->setObjectName(QStringLiteral("groupBox_Features"));
        groupBox_Features->setGeometry(QRect(20, 30, 361, 581));
        groupBox_Features->setCheckable(false);
        gridLayout = new QGridLayout(groupBox_Features);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(12, 10, 12, 10);
        checkBox_RootMean = new QCheckBox(groupBox_Features);
        checkBox_RootMean->setObjectName(QStringLiteral("checkBox_RootMean"));
        checkBox_RootMean->setEnabled(true);
        checkBox_RootMean->setAutoFillBackground(false);
        checkBox_RootMean->setChecked(false);
        checkBox_RootMean->setTristate(false);

        gridLayout->addWidget(checkBox_RootMean, 11, 1, 1, 1);

        checkBox_Kurtosis = new QCheckBox(groupBox_Features);
        checkBox_Kurtosis->setObjectName(QStringLiteral("checkBox_Kurtosis"));
        checkBox_Kurtosis->setEnabled(true);
        checkBox_Kurtosis->setChecked(false);

        gridLayout->addWidget(checkBox_Kurtosis, 2, 1, 1, 1);

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

        checkBox_Variance = new QCheckBox(groupBox_Features);
        checkBox_Variance->setObjectName(QStringLiteral("checkBox_Variance"));
        checkBox_Variance->setEnabled(true);
        checkBox_Variance->setCheckable(true);
        checkBox_Variance->setChecked(false);
        checkBox_Variance->setTristate(false);

        gridLayout->addWidget(checkBox_Variance, 1, 1, 1, 1);

        checkBox_Mean = new QCheckBox(groupBox_Features);
        checkBox_Mean->setObjectName(QStringLiteral("checkBox_Mean"));
        checkBox_Mean->setAutoFillBackground(false);
        checkBox_Mean->setChecked(false);
        checkBox_Mean->setTristate(false);

        gridLayout->addWidget(checkBox_Mean, 1, 0, 1, 1);

        checkBox_Energy = new QCheckBox(groupBox_Features);
        checkBox_Energy->setObjectName(QStringLiteral("checkBox_Energy"));
        checkBox_Energy->setEnabled(true);
        checkBox_Energy->setAutoFillBackground(false);
        checkBox_Energy->setChecked(false);
        checkBox_Energy->setTristate(false);

        gridLayout->addWidget(checkBox_Energy, 11, 0, 1, 1);

        checkBox_RobustMeanAbsDev = new QCheckBox(groupBox_Features);
        checkBox_RobustMeanAbsDev->setObjectName(QStringLiteral("checkBox_RobustMeanAbsDev"));
        checkBox_RobustMeanAbsDev->setEnabled(true);
        QFont font1;
        font1.setFamily(QString::fromUtf8("\352\265\264\353\246\274"));
        font1.setPointSize(8);
        checkBox_RobustMeanAbsDev->setFont(font1);
        checkBox_RobustMeanAbsDev->setAcceptDrops(false);
        checkBox_RobustMeanAbsDev->setAutoFillBackground(false);
        checkBox_RobustMeanAbsDev->setChecked(false);
        checkBox_RobustMeanAbsDev->setTristate(false);

        gridLayout->addWidget(checkBox_RobustMeanAbsDev, 9, 0, 1, 1);

        checkBox_CoeffOfVar = new QCheckBox(groupBox_Features);
        checkBox_CoeffOfVar->setObjectName(QStringLiteral("checkBox_CoeffOfVar"));
        checkBox_CoeffOfVar->setEnabled(true);
        checkBox_CoeffOfVar->setFont(font1);
        checkBox_CoeffOfVar->setAutoFillBackground(false);
        checkBox_CoeffOfVar->setChecked(false);
        checkBox_CoeffOfVar->setTristate(false);

        gridLayout->addWidget(checkBox_CoeffOfVar, 10, 0, 1, 1);

        checkBox_MedianAbsDev = new QCheckBox(groupBox_Features);
        checkBox_MedianAbsDev->setObjectName(QStringLiteral("checkBox_MedianAbsDev"));
        checkBox_MedianAbsDev->setEnabled(true);
        checkBox_MedianAbsDev->setFont(font1);
        checkBox_MedianAbsDev->setAutoFillBackground(false);
        checkBox_MedianAbsDev->setChecked(false);
        checkBox_MedianAbsDev->setTristate(false);

        gridLayout->addWidget(checkBox_MedianAbsDev, 9, 1, 1, 1);

        checkBox_Range = new QCheckBox(groupBox_Features);
        checkBox_Range->setObjectName(QStringLiteral("checkBox_Range"));
        checkBox_Range->setEnabled(true);
        checkBox_Range->setAutoFillBackground(false);
        checkBox_Range->setChecked(false);
        checkBox_Range->setTristate(false);

        gridLayout->addWidget(checkBox_Range, 8, 0, 1, 1);

        checkBox_90thPercentile = new QCheckBox(groupBox_Features);
        checkBox_90thPercentile->setObjectName(QStringLiteral("checkBox_90thPercentile"));
        checkBox_90thPercentile->setEnabled(true);
        checkBox_90thPercentile->setChecked(false);

        gridLayout->addWidget(checkBox_90thPercentile, 6, 1, 1, 1);

        checkBox_Minimum = new QCheckBox(groupBox_Features);
        checkBox_Minimum->setObjectName(QStringLiteral("checkBox_Minimum"));
        checkBox_Minimum->setEnabled(true);
        checkBox_Minimum->setChecked(false);

        gridLayout->addWidget(checkBox_Minimum, 5, 1, 1, 1);

        checkBox_MeanAbsDev = new QCheckBox(groupBox_Features);
        checkBox_MeanAbsDev->setObjectName(QStringLiteral("checkBox_MeanAbsDev"));
        checkBox_MeanAbsDev->setEnabled(true);
        checkBox_MeanAbsDev->setFont(font1);
        checkBox_MeanAbsDev->setAutoFillBackground(false);
        checkBox_MeanAbsDev->setChecked(false);
        checkBox_MeanAbsDev->setTristate(false);

        gridLayout->addWidget(checkBox_MeanAbsDev, 8, 1, 1, 1);

        checkBox_Maximum = new QCheckBox(groupBox_Features);
        checkBox_Maximum->setObjectName(QStringLiteral("checkBox_Maximum"));
        checkBox_Maximum->setAutoFillBackground(false);
        checkBox_Maximum->setChecked(false);
        checkBox_Maximum->setTristate(false);

        gridLayout->addWidget(checkBox_Maximum, 7, 0, 1, 1);

        checkBox_Skewness = new QCheckBox(groupBox_Features);
        checkBox_Skewness->setObjectName(QStringLiteral("checkBox_Skewness"));
        checkBox_Skewness->setEnabled(true);

        gridLayout->addWidget(checkBox_Skewness, 2, 0, 1, 1);

        checkBox_InterquartileRange = new QCheckBox(groupBox_Features);
        checkBox_InterquartileRange->setObjectName(QStringLiteral("checkBox_InterquartileRange"));
        checkBox_InterquartileRange->setEnabled(true);
        checkBox_InterquartileRange->setCheckable(true);
        checkBox_InterquartileRange->setChecked(false);
        checkBox_InterquartileRange->setTristate(false);

        gridLayout->addWidget(checkBox_InterquartileRange, 7, 1, 1, 1);

        checkBox_QuartileCoeff = new QCheckBox(groupBox_Features);
        checkBox_QuartileCoeff->setObjectName(QStringLiteral("checkBox_QuartileCoeff"));
        checkBox_QuartileCoeff->setEnabled(true);
        checkBox_QuartileCoeff->setAutoFillBackground(false);
        checkBox_QuartileCoeff->setChecked(false);
        checkBox_QuartileCoeff->setTristate(false);

        gridLayout->addWidget(checkBox_QuartileCoeff, 10, 1, 1, 1);


        retranslateUi(popup_Statistics);
        QObject::connect(buttonBox, SIGNAL(accepted()), popup_Statistics, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), popup_Statistics, SLOT(reject()));

        QMetaObject::connectSlotsByName(popup_Statistics);
    } // setupUi

    void retranslateUi(QDialog *popup_Statistics)
    {
        popup_Statistics->setWindowTitle(QApplication::translate("popup_Statistics", "Set Measurements", Q_NULLPTR));
        groupBox_Features->setTitle(QApplication::translate("popup_Statistics", "Features", Q_NULLPTR));
        checkBox_RootMean->setText(QApplication::translate("popup_Statistics", "Root Mean Squre", Q_NULLPTR));
        checkBox_Kurtosis->setText(QApplication::translate("popup_Statistics", "Kurtosis", Q_NULLPTR));
        checkBox_10thPercentile->setText(QApplication::translate("popup_Statistics", "10th Percentile", Q_NULLPTR));
        checkBox_Median->setText(QApplication::translate("popup_Statistics", "Median", Q_NULLPTR));
        checkBox_All->setText(QApplication::translate("popup_Statistics", "All", Q_NULLPTR));
        checkBox_Variance->setText(QApplication::translate("popup_Statistics", "Variance", Q_NULLPTR));
        checkBox_Mean->setText(QApplication::translate("popup_Statistics", "Mean", Q_NULLPTR));
        checkBox_Energy->setText(QApplication::translate("popup_Statistics", "Energy", Q_NULLPTR));
        checkBox_RobustMeanAbsDev->setText(QApplication::translate("popup_Statistics", "Robust Mean Abs Dev", Q_NULLPTR));
        checkBox_CoeffOfVar->setText(QApplication::translate("popup_Statistics", "Coefficient of Variation", Q_NULLPTR));
        checkBox_MedianAbsDev->setText(QApplication::translate("popup_Statistics", "Median Absolute Deviation", Q_NULLPTR));
        checkBox_Range->setText(QApplication::translate("popup_Statistics", "Range", Q_NULLPTR));
        checkBox_90thPercentile->setText(QApplication::translate("popup_Statistics", "90th Percentile", Q_NULLPTR));
        checkBox_Minimum->setText(QApplication::translate("popup_Statistics", "Minimum", Q_NULLPTR));
        checkBox_MeanAbsDev->setText(QApplication::translate("popup_Statistics", "Mean Absolute Deviation", Q_NULLPTR));
        checkBox_Maximum->setText(QApplication::translate("popup_Statistics", "Maximum", Q_NULLPTR));
        checkBox_Skewness->setText(QApplication::translate("popup_Statistics", "Skewness", Q_NULLPTR));
        checkBox_InterquartileRange->setText(QApplication::translate("popup_Statistics", "Interquartile range", Q_NULLPTR));
        checkBox_QuartileCoeff->setText(QApplication::translate("popup_Statistics", "Quartile Coefficient", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class popup_Statistics: public Ui_popup_Statistics {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POPUP_STATISTICS_H
