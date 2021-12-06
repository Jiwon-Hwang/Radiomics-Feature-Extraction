/********************************************************************************
** Form generated from reading UI file 'platform.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLATFORM_H
#define UI_PLATFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_platformClass
{
public:
    QWidget *centralWidget;
    QSplitter *splitter;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_1;
    QPushButton *pushButton_2;
    QHBoxLayout *horizontalLayout_9;
    QTreeView *treeView;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout_4;
    QSpacerItem *verticalSpacer;
    QScrollBar *horizontalScrollBar;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox_filters;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_6;
    QRadioButton *radioButton_Gaussian;
    QRadioButton *radioButton_LoG;
    QGroupBox *groupBox_features;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_7;
    QCheckBox *checkBox_Histogram;
    QCheckBox *checkBox_GLCM;
    QCheckBox *checkBox_Morph;
    QCheckBox *checkBox_Intensity;
    QPushButton *pushButton_run;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *platformClass)
    {
        if (platformClass->objectName().isEmpty())
            platformClass->setObjectName(QStringLiteral("platformClass"));
        platformClass->resize(1097, 735);
        platformClass->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon;
        icon.addFile(QStringLiteral("Resources/Deep Imaging Laboratory.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        platformClass->setWindowIcon(icon);
        platformClass->setAutoFillBackground(false);
        platformClass->setStyleSheet(QStringLiteral(""));
        centralWidget = new QWidget(platformClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setGeometry(QRect(0, 0, 100, 30));
        splitter->setOrientation(Qt::Vertical);
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetMaximumSize);
        horizontalLayout_3->setContentsMargins(5, -1, 5, -1);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_1 = new QPushButton(centralWidget);
        pushButton_1->setObjectName(QStringLiteral("pushButton_1"));

        horizontalLayout->addWidget(pushButton_1);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        treeView = new QTreeView(centralWidget);
        treeView->setObjectName(QStringLiteral("treeView"));

        horizontalLayout_9->addWidget(treeView);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer, 0, 0, 1, 1);


        verticalLayout_2->addLayout(gridLayout_4);

        horizontalScrollBar = new QScrollBar(centralWidget);
        horizontalScrollBar->setObjectName(QStringLiteral("horizontalScrollBar"));
        horizontalScrollBar->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(horizontalScrollBar);


        horizontalLayout_9->addLayout(verticalLayout_2);

        horizontalLayout_9->setStretch(0, 1);
        horizontalLayout_9->setStretch(1, 4);

        verticalLayout_3->addLayout(horizontalLayout_9);


        horizontalLayout_3->addLayout(verticalLayout_3);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        verticalLayout_5 = new QVBoxLayout(groupBox);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        groupBox_filters = new QGroupBox(groupBox);
        groupBox_filters->setObjectName(QStringLiteral("groupBox_filters"));
        layoutWidget = new QWidget(groupBox_filters);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 90, 193, 141));
        verticalLayout_6 = new QVBoxLayout(layoutWidget);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        radioButton_Gaussian = new QRadioButton(layoutWidget);
        radioButton_Gaussian->setObjectName(QStringLiteral("radioButton_Gaussian"));

        verticalLayout_6->addWidget(radioButton_Gaussian);

        radioButton_LoG = new QRadioButton(layoutWidget);
        radioButton_LoG->setObjectName(QStringLiteral("radioButton_LoG"));

        verticalLayout_6->addWidget(radioButton_LoG);


        verticalLayout_4->addWidget(groupBox_filters);

        groupBox_features = new QGroupBox(groupBox);
        groupBox_features->setObjectName(QStringLiteral("groupBox_features"));
        layoutWidget1 = new QWidget(groupBox_features);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(30, 70, 186, 171));
        verticalLayout_7 = new QVBoxLayout(layoutWidget1);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        checkBox_Histogram = new QCheckBox(layoutWidget1);
        checkBox_Histogram->setObjectName(QStringLiteral("checkBox_Histogram"));

        verticalLayout_7->addWidget(checkBox_Histogram);

        checkBox_GLCM = new QCheckBox(layoutWidget1);
        checkBox_GLCM->setObjectName(QStringLiteral("checkBox_GLCM"));

        verticalLayout_7->addWidget(checkBox_GLCM);

        checkBox_Morph = new QCheckBox(layoutWidget1);
        checkBox_Morph->setObjectName(QStringLiteral("checkBox_Morph"));

        verticalLayout_7->addWidget(checkBox_Morph);

        checkBox_Intensity = new QCheckBox(layoutWidget1);
        checkBox_Intensity->setObjectName(QStringLiteral("checkBox_Intensity"));
        checkBox_Intensity->setCheckable(true);
        checkBox_Intensity->setChecked(false);
        checkBox_Intensity->setTristate(false);

        verticalLayout_7->addWidget(checkBox_Intensity);


        verticalLayout_4->addWidget(groupBox_features);


        verticalLayout_5->addLayout(verticalLayout_4);

        pushButton_run = new QPushButton(groupBox);
        pushButton_run->setObjectName(QStringLiteral("pushButton_run"));

        verticalLayout_5->addWidget(pushButton_run);


        horizontalLayout_3->addWidget(groupBox);

        horizontalLayout_3->setStretch(0, 3);
        horizontalLayout_3->setStretch(1, 1);

        gridLayout_3->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        platformClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(platformClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1097, 21));
        platformClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(platformClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        platformClass->setStatusBar(statusBar);

        retranslateUi(platformClass);

        QMetaObject::connectSlotsByName(platformClass);
    } // setupUi

    void retranslateUi(QMainWindow *platformClass)
    {
        platformClass->setWindowTitle(QApplication::translate("platformClass", "\354\227\260\352\265\254\354\213\244 \355\224\214\353\236\253\355\217\274 \354\227\260\354\212\265\354\232\251", Q_NULLPTR));
        pushButton_1->setText(QApplication::translate("platformClass", "1 case", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("platformClass", "multiple cases", Q_NULLPTR));
        groupBox->setTitle(QString());
        groupBox_filters->setTitle(QApplication::translate("platformClass", "Filters", Q_NULLPTR));
        radioButton_Gaussian->setText(QApplication::translate("platformClass", "Gaussian", Q_NULLPTR));
        radioButton_LoG->setText(QApplication::translate("platformClass", "LoG (Laplacian-of-Gaussian)", Q_NULLPTR));
        groupBox_features->setTitle(QApplication::translate("platformClass", "Radiomics Features", Q_NULLPTR));
        checkBox_Histogram->setText(QApplication::translate("platformClass", "Intensity Histogram Features", Q_NULLPTR));
        checkBox_GLCM->setText(QApplication::translate("platformClass", "Local Intensity Features", Q_NULLPTR));
        checkBox_Morph->setText(QApplication::translate("platformClass", "Morphological Features", Q_NULLPTR));
        checkBox_Intensity->setText(QApplication::translate("platformClass", "GLCM", Q_NULLPTR));
        pushButton_run->setText(QApplication::translate("platformClass", "Run", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class platformClass: public Ui_platformClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLATFORM_H
