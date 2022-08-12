/********************************************************************************
** Form generated from reading UI file 'popup_Intensity.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POPUP_INTENSITY_H
#define UI_POPUP_INTENSITY_H

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

class Ui_popup_Intensity
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox_Features;
    QGridLayout *gridLayout;
    QCheckBox *checkBox_All;
    QCheckBox *checkBox_LocalPeak;
    QCheckBox *checkBox_GlobalPeak;

    void setupUi(QDialog *popup_Intensity)
    {
        if (popup_Intensity->objectName().isEmpty())
            popup_Intensity->setObjectName(QStringLiteral("popup_Intensity"));
        popup_Intensity->resize(400, 280);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(popup_Intensity->sizePolicy().hasHeightForWidth());
        popup_Intensity->setSizePolicy(sizePolicy);
        buttonBox = new QDialogButtonBox(popup_Intensity);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(230, 230, 151, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);
        groupBox_Features = new QGroupBox(popup_Intensity);
        groupBox_Features->setObjectName(QStringLiteral("groupBox_Features"));
        groupBox_Features->setGeometry(QRect(20, 30, 361, 181));
        groupBox_Features->setCheckable(false);
        gridLayout = new QGridLayout(groupBox_Features);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(12, 10, 12, 10);
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

        checkBox_LocalPeak = new QCheckBox(groupBox_Features);
        checkBox_LocalPeak->setObjectName(QStringLiteral("checkBox_LocalPeak"));
        checkBox_LocalPeak->setAutoFillBackground(false);
        checkBox_LocalPeak->setChecked(false);
        checkBox_LocalPeak->setTristate(false);

        gridLayout->addWidget(checkBox_LocalPeak, 1, 0, 1, 1);

        checkBox_GlobalPeak = new QCheckBox(groupBox_Features);
        checkBox_GlobalPeak->setObjectName(QStringLiteral("checkBox_GlobalPeak"));
        checkBox_GlobalPeak->setEnabled(true);
        checkBox_GlobalPeak->setCheckable(true);
        checkBox_GlobalPeak->setChecked(false);
        checkBox_GlobalPeak->setTristate(false);

        gridLayout->addWidget(checkBox_GlobalPeak, 1, 1, 1, 1);


        retranslateUi(popup_Intensity);
        QObject::connect(buttonBox, SIGNAL(accepted()), popup_Intensity, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), popup_Intensity, SLOT(reject()));

        QMetaObject::connectSlotsByName(popup_Intensity);
    } // setupUi

    void retranslateUi(QDialog *popup_Intensity)
    {
        popup_Intensity->setWindowTitle(QApplication::translate("popup_Intensity", "Set Measurements", Q_NULLPTR));
        groupBox_Features->setTitle(QApplication::translate("popup_Intensity", "Features", Q_NULLPTR));
        checkBox_All->setText(QApplication::translate("popup_Intensity", "All", Q_NULLPTR));
        checkBox_LocalPeak->setText(QApplication::translate("popup_Intensity", "Local Intensity Peak", Q_NULLPTR));
        checkBox_GlobalPeak->setText(QApplication::translate("popup_Intensity", "Global Intensity Peak", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class popup_Intensity: public Ui_popup_Intensity {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POPUP_INTENSITY_H
