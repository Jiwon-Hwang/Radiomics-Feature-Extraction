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
#include <QtWidgets/QTreeWidget>
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
    QHBoxLayout *horizontalLayout_9;
    QTreeWidget *treeWidget_FileDirectory;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout_4;
    QSpacerItem *verticalSpacer;
    QScrollBar *horizontalScrollBar;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox_Filters;
    QVBoxLayout *verticalLayout;
    QRadioButton *radioButton_None;
    QRadioButton *radioButton_Gaussian;
    QRadioButton *radioButton_Laplacian;
    QGroupBox *groupBox_Families;
    QVBoxLayout *verticalLayout_6;
    QCheckBox *checkBox_Histogram;
    QCheckBox *checkBox_Intensity;
    QCheckBox *checkBox_Morph;
    QCheckBox *checkBox_GLCM;
    QCheckBox *checkBox_GLRLM;
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
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        treeWidget_FileDirectory = new QTreeWidget(centralWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget_FileDirectory->setHeaderItem(__qtreewidgetitem);
        treeWidget_FileDirectory->setObjectName(QStringLiteral("treeWidget_FileDirectory"));
        sizePolicy.setHeightForWidth(treeWidget_FileDirectory->sizePolicy().hasHeightForWidth());
        treeWidget_FileDirectory->setSizePolicy(sizePolicy);

        horizontalLayout_9->addWidget(treeWidget_FileDirectory);

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
        groupBox_Filters = new QGroupBox(groupBox);
        groupBox_Filters->setObjectName(QStringLiteral("groupBox_Filters"));
        groupBox_Filters->setFlat(false);
        verticalLayout = new QVBoxLayout(groupBox_Filters);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(30, 40, 30, 40);
        radioButton_None = new QRadioButton(groupBox_Filters);
        radioButton_None->setObjectName(QStringLiteral("radioButton_None"));

        verticalLayout->addWidget(radioButton_None);

        radioButton_Gaussian = new QRadioButton(groupBox_Filters);
        radioButton_Gaussian->setObjectName(QStringLiteral("radioButton_Gaussian"));

        verticalLayout->addWidget(radioButton_Gaussian);

        radioButton_Laplacian = new QRadioButton(groupBox_Filters);
        radioButton_Laplacian->setObjectName(QStringLiteral("radioButton_Laplacian"));

        verticalLayout->addWidget(radioButton_Laplacian);


        verticalLayout_4->addWidget(groupBox_Filters);

        groupBox_Families = new QGroupBox(groupBox);
        groupBox_Families->setObjectName(QStringLiteral("groupBox_Families"));
        verticalLayout_6 = new QVBoxLayout(groupBox_Families);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(30, 40, 30, 20);
        checkBox_Histogram = new QCheckBox(groupBox_Families);
        checkBox_Histogram->setObjectName(QStringLiteral("checkBox_Histogram"));
        checkBox_Histogram->setAutoFillBackground(false);
        checkBox_Histogram->setChecked(false);
        checkBox_Histogram->setTristate(false);

        verticalLayout_6->addWidget(checkBox_Histogram);

        checkBox_Intensity = new QCheckBox(groupBox_Families);
        checkBox_Intensity->setObjectName(QStringLiteral("checkBox_Intensity"));
        checkBox_Intensity->setEnabled(false);
        checkBox_Intensity->setCheckable(true);
        checkBox_Intensity->setChecked(false);
        checkBox_Intensity->setTristate(false);

        verticalLayout_6->addWidget(checkBox_Intensity);

        checkBox_Morph = new QCheckBox(groupBox_Families);
        checkBox_Morph->setObjectName(QStringLiteral("checkBox_Morph"));
        checkBox_Morph->setEnabled(false);

        verticalLayout_6->addWidget(checkBox_Morph);

        checkBox_GLCM = new QCheckBox(groupBox_Families);
        checkBox_GLCM->setObjectName(QStringLiteral("checkBox_GLCM"));
        checkBox_GLCM->setEnabled(true);
        checkBox_GLCM->setChecked(false);

        verticalLayout_6->addWidget(checkBox_GLCM);

        checkBox_GLRLM = new QCheckBox(groupBox_Families);
        checkBox_GLRLM->setObjectName(QStringLiteral("checkBox_GLRLM"));
        checkBox_GLRLM->setEnabled(false);
        checkBox_GLRLM->setChecked(false);

        verticalLayout_6->addWidget(checkBox_GLRLM);


        verticalLayout_4->addWidget(groupBox_Families);


        verticalLayout_5->addLayout(verticalLayout_4);

        pushButton_run = new QPushButton(groupBox);
        pushButton_run->setObjectName(QStringLiteral("pushButton_run"));
        pushButton_run->setEnabled(false);

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
        platformClass->setWindowTitle(QApplication::translate("platformClass", "Radiomics Feature Extraction", Q_NULLPTR));
        groupBox->setTitle(QString());
        groupBox_Filters->setTitle(QApplication::translate("platformClass", "Filter", Q_NULLPTR));
        radioButton_None->setText(QApplication::translate("platformClass", "None", Q_NULLPTR));
        radioButton_Gaussian->setText(QApplication::translate("platformClass", "Gaussian", Q_NULLPTR));
        radioButton_Laplacian->setText(QApplication::translate("platformClass", "Laplacian", Q_NULLPTR));
        groupBox_Families->setTitle(QApplication::translate("platformClass", "Radiomics Feature Family", Q_NULLPTR));
        checkBox_Histogram->setText(QApplication::translate("platformClass", "Intensity Histogram", Q_NULLPTR));
        checkBox_Intensity->setText(QApplication::translate("platformClass", "Local Intensity ", Q_NULLPTR));
        checkBox_Morph->setText(QApplication::translate("platformClass", "Morphological ", Q_NULLPTR));
        checkBox_GLCM->setText(QApplication::translate("platformClass", "GLCM ", Q_NULLPTR));
        checkBox_GLRLM->setText(QApplication::translate("platformClass", "GLRLM ", Q_NULLPTR));
        pushButton_run->setText(QApplication::translate("platformClass", "Run", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class platformClass: public Ui_platformClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLATFORM_H
