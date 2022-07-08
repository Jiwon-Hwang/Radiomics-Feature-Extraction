/********************************************************************************
** Form generated from reading UI file 'popup_GLCM.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POPUP_GLCM_H
#define UI_POPUP_GLCM_H

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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_popup_GLCM
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox_Features;
    QGridLayout *gridLayout;
    QCheckBox *checkBox_Correlation;
    QCheckBox *checkBox_InverseDiffMomNorm;
    QCheckBox *checkBox_InverseDiffNorm;
    QCheckBox *checkBox_InverseVariance;
    QCheckBox *checkBox_InverseDiff;
    QCheckBox *checkBox_SumAverage;
    QCheckBox *checkBox_FirstMCorrelation;
    QCheckBox *checkBox_ClusterProminence;
    QCheckBox *checkBox_SumVariance;
    QCheckBox *checkBox_Autocorrelation;
    QCheckBox *checkBox_AngSecMoment;
    QCheckBox *checkBox_SumEntropy;
    QCheckBox *checkBox_JointEntropy;
    QCheckBox *checkBox_JointMaximum;
    QCheckBox *checkBox_JointAverage;
    QCheckBox *checkBox_DifferenceEntropy;
    QCheckBox *checkBox_DifferenceVariance;
    QCheckBox *checkBox_JointVariance;
    QCheckBox *checkBox_InverseDiffMom;
    QCheckBox *checkBox_Contrast;
    QCheckBox *checkBox_ClusterShade;
    QCheckBox *checkBox_ClusterTendency;
    QCheckBox *checkBox_SecondMCorrelation;
    QCheckBox *checkBox_DifferenceAverage;
    QCheckBox *checkBox_Dissimilarity;
    QCheckBox *checkBox_All;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout_2;
    QComboBox *comboBox_nBins;
    QRadioButton *radioButton_FBN;
    QRadioButton *radioButton_FBS;
    QLineEdit *lineEdit_sBin;

    void setupUi(QDialog *popup_GLCM)
    {
        if (popup_GLCM->objectName().isEmpty())
            popup_GLCM->setObjectName(QStringLiteral("popup_GLCM"));
        popup_GLCM->resize(400, 770);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(popup_GLCM->sizePolicy().hasHeightForWidth());
        popup_GLCM->setSizePolicy(sizePolicy);
        buttonBox = new QDialogButtonBox(popup_GLCM);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(230, 730, 151, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);
        groupBox_Features = new QGroupBox(popup_GLCM);
        groupBox_Features->setObjectName(QStringLiteral("groupBox_Features"));
        groupBox_Features->setGeometry(QRect(20, 30, 360, 580));
        groupBox_Features->setCheckable(false);
        gridLayout = new QGridLayout(groupBox_Features);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(12, 10, 12, 10);
        checkBox_Correlation = new QCheckBox(groupBox_Features);
        checkBox_Correlation->setObjectName(QStringLiteral("checkBox_Correlation"));
        checkBox_Correlation->setEnabled(true);
        checkBox_Correlation->setAutoFillBackground(false);
        checkBox_Correlation->setChecked(false);
        checkBox_Correlation->setTristate(false);

        gridLayout->addWidget(checkBox_Correlation, 12, 0, 1, 1);

        checkBox_InverseDiffMomNorm = new QCheckBox(groupBox_Features);
        checkBox_InverseDiffMomNorm->setObjectName(QStringLiteral("checkBox_InverseDiffMomNorm"));
        checkBox_InverseDiffMomNorm->setEnabled(true);
        checkBox_InverseDiffMomNorm->setAutoFillBackground(false);
        checkBox_InverseDiffMomNorm->setChecked(false);
        checkBox_InverseDiffMomNorm->setTristate(false);

        gridLayout->addWidget(checkBox_InverseDiffMomNorm, 11, 0, 1, 1);

        checkBox_InverseDiffNorm = new QCheckBox(groupBox_Features);
        checkBox_InverseDiffNorm->setObjectName(QStringLiteral("checkBox_InverseDiffNorm"));
        checkBox_InverseDiffNorm->setEnabled(true);
        QFont font;
        font.setFamily(QString::fromUtf8("\352\265\264\353\246\274"));
        font.setPointSize(9);
        checkBox_InverseDiffNorm->setFont(font);
        checkBox_InverseDiffNorm->setAutoFillBackground(false);
        checkBox_InverseDiffNorm->setChecked(false);
        checkBox_InverseDiffNorm->setTristate(false);

        gridLayout->addWidget(checkBox_InverseDiffNorm, 10, 0, 1, 1);

        checkBox_InverseVariance = new QCheckBox(groupBox_Features);
        checkBox_InverseVariance->setObjectName(QStringLiteral("checkBox_InverseVariance"));
        checkBox_InverseVariance->setEnabled(true);
        checkBox_InverseVariance->setAutoFillBackground(false);
        checkBox_InverseVariance->setChecked(false);
        checkBox_InverseVariance->setTristate(false);

        gridLayout->addWidget(checkBox_InverseVariance, 11, 1, 1, 1);

        checkBox_InverseDiff = new QCheckBox(groupBox_Features);
        checkBox_InverseDiff->setObjectName(QStringLiteral("checkBox_InverseDiff"));
        checkBox_InverseDiff->setEnabled(true);
        checkBox_InverseDiff->setFont(font);
        checkBox_InverseDiff->setAcceptDrops(false);
        checkBox_InverseDiff->setAutoFillBackground(false);
        checkBox_InverseDiff->setChecked(false);
        checkBox_InverseDiff->setTristate(false);

        gridLayout->addWidget(checkBox_InverseDiff, 9, 1, 1, 1);

        checkBox_SumAverage = new QCheckBox(groupBox_Features);
        checkBox_SumAverage->setObjectName(QStringLiteral("checkBox_SumAverage"));
        checkBox_SumAverage->setEnabled(true);
        checkBox_SumAverage->setChecked(false);

        gridLayout->addWidget(checkBox_SumAverage, 6, 1, 1, 1);

        checkBox_FirstMCorrelation = new QCheckBox(groupBox_Features);
        checkBox_FirstMCorrelation->setObjectName(QStringLiteral("checkBox_FirstMCorrelation"));
        checkBox_FirstMCorrelation->setEnabled(true);
        checkBox_FirstMCorrelation->setAutoFillBackground(false);
        checkBox_FirstMCorrelation->setChecked(false);
        checkBox_FirstMCorrelation->setTristate(false);

        gridLayout->addWidget(checkBox_FirstMCorrelation, 14, 1, 1, 1);

        checkBox_ClusterProminence = new QCheckBox(groupBox_Features);
        checkBox_ClusterProminence->setObjectName(QStringLiteral("checkBox_ClusterProminence"));
        checkBox_ClusterProminence->setEnabled(true);
        checkBox_ClusterProminence->setAutoFillBackground(false);
        checkBox_ClusterProminence->setChecked(false);
        checkBox_ClusterProminence->setTristate(false);

        gridLayout->addWidget(checkBox_ClusterProminence, 14, 0, 1, 1);

        checkBox_SumVariance = new QCheckBox(groupBox_Features);
        checkBox_SumVariance->setObjectName(QStringLiteral("checkBox_SumVariance"));
        checkBox_SumVariance->setEnabled(true);
        checkBox_SumVariance->setAutoFillBackground(false);
        checkBox_SumVariance->setChecked(false);
        checkBox_SumVariance->setTristate(false);

        gridLayout->addWidget(checkBox_SumVariance, 7, 0, 1, 1);

        checkBox_Autocorrelation = new QCheckBox(groupBox_Features);
        checkBox_Autocorrelation->setObjectName(QStringLiteral("checkBox_Autocorrelation"));
        checkBox_Autocorrelation->setEnabled(true);
        checkBox_Autocorrelation->setAutoFillBackground(false);
        checkBox_Autocorrelation->setChecked(false);
        checkBox_Autocorrelation->setTristate(false);

        gridLayout->addWidget(checkBox_Autocorrelation, 12, 1, 1, 1);

        checkBox_AngSecMoment = new QCheckBox(groupBox_Features);
        checkBox_AngSecMoment->setObjectName(QStringLiteral("checkBox_AngSecMoment"));
        checkBox_AngSecMoment->setEnabled(true);
        checkBox_AngSecMoment->setAutoFillBackground(false);
        checkBox_AngSecMoment->setChecked(false);
        checkBox_AngSecMoment->setTristate(false);

        gridLayout->addWidget(checkBox_AngSecMoment, 8, 0, 1, 1);

        checkBox_SumEntropy = new QCheckBox(groupBox_Features);
        checkBox_SumEntropy->setObjectName(QStringLiteral("checkBox_SumEntropy"));
        checkBox_SumEntropy->setEnabled(true);
        checkBox_SumEntropy->setCheckable(true);
        checkBox_SumEntropy->setChecked(false);
        checkBox_SumEntropy->setTristate(false);

        gridLayout->addWidget(checkBox_SumEntropy, 7, 1, 1, 1);

        checkBox_JointEntropy = new QCheckBox(groupBox_Features);
        checkBox_JointEntropy->setObjectName(QStringLiteral("checkBox_JointEntropy"));
        checkBox_JointEntropy->setEnabled(true);
        checkBox_JointEntropy->setChecked(false);

        gridLayout->addWidget(checkBox_JointEntropy, 2, 1, 1, 1);

        checkBox_JointMaximum = new QCheckBox(groupBox_Features);
        checkBox_JointMaximum->setObjectName(QStringLiteral("checkBox_JointMaximum"));
        checkBox_JointMaximum->setAutoFillBackground(false);
        checkBox_JointMaximum->setChecked(false);
        checkBox_JointMaximum->setTristate(false);

        gridLayout->addWidget(checkBox_JointMaximum, 1, 0, 1, 1);

        checkBox_JointAverage = new QCheckBox(groupBox_Features);
        checkBox_JointAverage->setObjectName(QStringLiteral("checkBox_JointAverage"));
        checkBox_JointAverage->setEnabled(true);
        checkBox_JointAverage->setCheckable(true);
        checkBox_JointAverage->setChecked(false);
        checkBox_JointAverage->setTristate(false);

        gridLayout->addWidget(checkBox_JointAverage, 1, 1, 1, 1);

        checkBox_DifferenceEntropy = new QCheckBox(groupBox_Features);
        checkBox_DifferenceEntropy->setObjectName(QStringLiteral("checkBox_DifferenceEntropy"));
        checkBox_DifferenceEntropy->setEnabled(true);
        checkBox_DifferenceEntropy->setChecked(false);

        gridLayout->addWidget(checkBox_DifferenceEntropy, 6, 0, 1, 1);

        checkBox_DifferenceVariance = new QCheckBox(groupBox_Features);
        checkBox_DifferenceVariance->setObjectName(QStringLiteral("checkBox_DifferenceVariance"));
        checkBox_DifferenceVariance->setEnabled(true);
        checkBox_DifferenceVariance->setChecked(false);

        gridLayout->addWidget(checkBox_DifferenceVariance, 5, 1, 1, 1);

        checkBox_JointVariance = new QCheckBox(groupBox_Features);
        checkBox_JointVariance->setObjectName(QStringLiteral("checkBox_JointVariance"));
        checkBox_JointVariance->setEnabled(true);

        gridLayout->addWidget(checkBox_JointVariance, 2, 0, 1, 1);

        checkBox_InverseDiffMom = new QCheckBox(groupBox_Features);
        checkBox_InverseDiffMom->setObjectName(QStringLiteral("checkBox_InverseDiffMom"));
        checkBox_InverseDiffMom->setEnabled(true);
        checkBox_InverseDiffMom->setFont(font);
        checkBox_InverseDiffMom->setAutoFillBackground(false);
        checkBox_InverseDiffMom->setChecked(false);
        checkBox_InverseDiffMom->setTristate(false);

        gridLayout->addWidget(checkBox_InverseDiffMom, 10, 1, 1, 1);

        checkBox_Contrast = new QCheckBox(groupBox_Features);
        checkBox_Contrast->setObjectName(QStringLiteral("checkBox_Contrast"));
        checkBox_Contrast->setEnabled(true);
        checkBox_Contrast->setAutoFillBackground(false);
        checkBox_Contrast->setChecked(false);
        checkBox_Contrast->setTristate(false);

        gridLayout->addWidget(checkBox_Contrast, 8, 1, 1, 1);

        checkBox_ClusterShade = new QCheckBox(groupBox_Features);
        checkBox_ClusterShade->setObjectName(QStringLiteral("checkBox_ClusterShade"));
        checkBox_ClusterShade->setEnabled(true);
        checkBox_ClusterShade->setAutoFillBackground(false);
        checkBox_ClusterShade->setChecked(false);
        checkBox_ClusterShade->setTristate(false);

        gridLayout->addWidget(checkBox_ClusterShade, 13, 1, 1, 1);

        checkBox_ClusterTendency = new QCheckBox(groupBox_Features);
        checkBox_ClusterTendency->setObjectName(QStringLiteral("checkBox_ClusterTendency"));
        checkBox_ClusterTendency->setEnabled(true);
        checkBox_ClusterTendency->setAutoFillBackground(false);
        checkBox_ClusterTendency->setChecked(false);
        checkBox_ClusterTendency->setTristate(false);

        gridLayout->addWidget(checkBox_ClusterTendency, 13, 0, 1, 1);

        checkBox_SecondMCorrelation = new QCheckBox(groupBox_Features);
        checkBox_SecondMCorrelation->setObjectName(QStringLiteral("checkBox_SecondMCorrelation"));
        checkBox_SecondMCorrelation->setEnabled(true);
        checkBox_SecondMCorrelation->setAutoFillBackground(false);
        checkBox_SecondMCorrelation->setChecked(false);
        checkBox_SecondMCorrelation->setTristate(false);

        gridLayout->addWidget(checkBox_SecondMCorrelation, 15, 0, 1, 1);

        checkBox_DifferenceAverage = new QCheckBox(groupBox_Features);
        checkBox_DifferenceAverage->setObjectName(QStringLiteral("checkBox_DifferenceAverage"));
        checkBox_DifferenceAverage->setEnabled(true);
        checkBox_DifferenceAverage->setChecked(false);

        gridLayout->addWidget(checkBox_DifferenceAverage, 5, 0, 1, 1);

        checkBox_Dissimilarity = new QCheckBox(groupBox_Features);
        checkBox_Dissimilarity->setObjectName(QStringLiteral("checkBox_Dissimilarity"));
        checkBox_Dissimilarity->setEnabled(true);
        checkBox_Dissimilarity->setFont(font);
        checkBox_Dissimilarity->setAutoFillBackground(false);
        checkBox_Dissimilarity->setChecked(false);
        checkBox_Dissimilarity->setTristate(false);

        gridLayout->addWidget(checkBox_Dissimilarity, 9, 0, 1, 1);

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

        gridLayoutWidget = new QWidget(popup_GLCM);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(20, 620, 361, 101));
        gridLayout_2 = new QGridLayout(gridLayoutWidget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout_2->setHorizontalSpacing(6);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        comboBox_nBins = new QComboBox(gridLayoutWidget);
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

        gridLayout_2->addWidget(comboBox_nBins, 0, 1, 1, 1);

        radioButton_FBN = new QRadioButton(gridLayoutWidget);
        radioButton_FBN->setObjectName(QStringLiteral("radioButton_FBN"));

        gridLayout_2->addWidget(radioButton_FBN, 0, 0, 1, 1);

        radioButton_FBS = new QRadioButton(gridLayoutWidget);
        radioButton_FBS->setObjectName(QStringLiteral("radioButton_FBS"));

        gridLayout_2->addWidget(radioButton_FBS, 1, 0, 1, 1);

        lineEdit_sBin = new QLineEdit(gridLayoutWidget);
        lineEdit_sBin->setObjectName(QStringLiteral("lineEdit_sBin"));

        gridLayout_2->addWidget(lineEdit_sBin, 1, 1, 1, 1);


        retranslateUi(popup_GLCM);
        QObject::connect(buttonBox, SIGNAL(accepted()), popup_GLCM, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), popup_GLCM, SLOT(reject()));

        QMetaObject::connectSlotsByName(popup_GLCM);
    } // setupUi

    void retranslateUi(QDialog *popup_GLCM)
    {
        popup_GLCM->setWindowTitle(QApplication::translate("popup_GLCM", "Set Measurements", Q_NULLPTR));
        groupBox_Features->setTitle(QApplication::translate("popup_GLCM", "Features", Q_NULLPTR));
        checkBox_Correlation->setText(QApplication::translate("popup_GLCM", "Correlation", Q_NULLPTR));
        checkBox_InverseDiffMomNorm->setText(QApplication::translate("popup_GLCM", "Inverse Diff Mom Norm", Q_NULLPTR));
        checkBox_InverseDiffNorm->setText(QApplication::translate("popup_GLCM", "Inverse Diff Normalised", Q_NULLPTR));
        checkBox_InverseVariance->setText(QApplication::translate("popup_GLCM", "Inverse Variance", Q_NULLPTR));
        checkBox_InverseDiff->setText(QApplication::translate("popup_GLCM", "Inverse Difference", Q_NULLPTR));
        checkBox_SumAverage->setText(QApplication::translate("popup_GLCM", "Sum Average", Q_NULLPTR));
        checkBox_FirstMCorrelation->setText(QApplication::translate("popup_GLCM", "Correlation 1", Q_NULLPTR));
        checkBox_ClusterProminence->setText(QApplication::translate("popup_GLCM", "Cluster Prominence", Q_NULLPTR));
        checkBox_SumVariance->setText(QApplication::translate("popup_GLCM", "Sum Variance", Q_NULLPTR));
        checkBox_Autocorrelation->setText(QApplication::translate("popup_GLCM", "Autocorrelation", Q_NULLPTR));
        checkBox_AngSecMoment->setText(QApplication::translate("popup_GLCM", "Angular Second Moment", Q_NULLPTR));
        checkBox_SumEntropy->setText(QApplication::translate("popup_GLCM", "Sum Entropy", Q_NULLPTR));
        checkBox_JointEntropy->setText(QApplication::translate("popup_GLCM", "Joint Entropy", Q_NULLPTR));
        checkBox_JointMaximum->setText(QApplication::translate("popup_GLCM", "Joint Maximum", Q_NULLPTR));
        checkBox_JointAverage->setText(QApplication::translate("popup_GLCM", "Joint Average", Q_NULLPTR));
        checkBox_DifferenceEntropy->setText(QApplication::translate("popup_GLCM", "Difference Entropy", Q_NULLPTR));
        checkBox_DifferenceVariance->setText(QApplication::translate("popup_GLCM", "Difference Variance", Q_NULLPTR));
        checkBox_JointVariance->setText(QApplication::translate("popup_GLCM", "Joint Variance", Q_NULLPTR));
        checkBox_InverseDiffMom->setText(QApplication::translate("popup_GLCM", "Inverse Diff Moment", Q_NULLPTR));
        checkBox_Contrast->setText(QApplication::translate("popup_GLCM", "Contrast", Q_NULLPTR));
        checkBox_ClusterShade->setText(QApplication::translate("popup_GLCM", "Cluster Shade", Q_NULLPTR));
        checkBox_ClusterTendency->setText(QApplication::translate("popup_GLCM", "Cluster Tendency", Q_NULLPTR));
        checkBox_SecondMCorrelation->setText(QApplication::translate("popup_GLCM", "Correlation 2", Q_NULLPTR));
        checkBox_DifferenceAverage->setText(QApplication::translate("popup_GLCM", "Difference Average", Q_NULLPTR));
        checkBox_Dissimilarity->setText(QApplication::translate("popup_GLCM", "Dissimilarity", Q_NULLPTR));
        checkBox_All->setText(QApplication::translate("popup_GLCM", "All", Q_NULLPTR));
        comboBox_nBins->clear();
        comboBox_nBins->insertItems(0, QStringList()
         << QApplication::translate("popup_GLCM", "8", Q_NULLPTR)
         << QApplication::translate("popup_GLCM", "16", Q_NULLPTR)
         << QApplication::translate("popup_GLCM", "32", Q_NULLPTR)
         << QApplication::translate("popup_GLCM", "64", Q_NULLPTR)
        );
        comboBox_nBins->setCurrentText(QApplication::translate("popup_GLCM", "8", Q_NULLPTR));
        radioButton_FBN->setText(QApplication::translate("popup_GLCM", "Fixed Bin Number (FBN)", Q_NULLPTR));
        radioButton_FBS->setText(QApplication::translate("popup_GLCM", "Fixed Bin Size (FBS)", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class popup_GLCM: public Ui_popup_GLCM {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POPUP_GLCM_H
