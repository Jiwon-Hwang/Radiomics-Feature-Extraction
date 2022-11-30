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
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
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
    QTabWidget *tabWidget_PreProcessing;
    QWidget *tabWidget_Filters;
    QVBoxLayout *verticalLayout;
    QRadioButton *radioButton_None;
    QRadioButton *radioButton_Gaussian;
    QRadioButton *radioButton_Laplacian;
    QWidget *tabWidget_Resampling;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout;
    QLabel *label_x_mm;
    QLineEdit *lineEdit_x;
    QLabel *label_x;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_y_mm;
    QLineEdit *lineEdit_y;
    QLabel *label_y;
    QGroupBox *groupBox_Families;
    QVBoxLayout *verticalLayout_6;
    QCheckBox *checkBox_Morph;
    QCheckBox *checkBox_Intensity;
    QCheckBox *checkBox_Statistics;
    QCheckBox *checkBox_Histogram;
    QCheckBox *checkBox_GLCM;
    QCheckBox *checkBox_GLRLM;
    QCheckBox *checkBox_GLSZM;
    QCheckBox *checkBox_GLDZM;
    QCheckBox *checkBox_NGTDM;
    QCheckBox *checkBox_NGLDM;
    QPushButton *pushButton_run;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *platformClass)
    {
        if (platformClass->objectName().isEmpty())
            platformClass->setObjectName(QStringLiteral("platformClass"));
        platformClass->resize(1235, 720);
        platformClass->setMaximumSize(QSize(16777215, 16777215));
        platformClass->setFocusPolicy(Qt::StrongFocus);
        QIcon icon;
        icon.addFile(QStringLiteral("../../../../../../../.designer/backup/Resources/Deep Imaging Laboratory.bmp"), QSize(), QIcon::Normal, QIcon::Off);
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
        horizontalLayout_3->setSpacing(2);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetMaximumSize);
        horizontalLayout_3->setContentsMargins(5, -1, 5, -1);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(4);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalLayout_9->setSizeConstraint(QLayout::SetDefaultConstraint);
        treeWidget_FileDirectory = new QTreeWidget(centralWidget);
        treeWidget_FileDirectory->setObjectName(QStringLiteral("treeWidget_FileDirectory"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(treeWidget_FileDirectory->sizePolicy().hasHeightForWidth());
        treeWidget_FileDirectory->setSizePolicy(sizePolicy1);
        treeWidget_FileDirectory->setMinimumSize(QSize(200, 500));
        treeWidget_FileDirectory->setMaximumSize(QSize(256, 16777215));
        treeWidget_FileDirectory->setFocusPolicy(Qt::NoFocus);
        treeWidget_FileDirectory->setContextMenuPolicy(Qt::CustomContextMenu);
        treeWidget_FileDirectory->setAutoFillBackground(false);
        treeWidget_FileDirectory->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeWidget_FileDirectory->setDragDropMode(QAbstractItemView::DropOnly);
        treeWidget_FileDirectory->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
        treeWidget_FileDirectory->setAllColumnsShowFocus(true);
        treeWidget_FileDirectory->setHeaderHidden(false);
        treeWidget_FileDirectory->header()->setVisible(true);
        treeWidget_FileDirectory->header()->setMinimumSectionSize(30);

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

        horizontalLayout_9->setStretch(0, 1);
        horizontalLayout_9->setStretch(1, 3);

        verticalLayout_3->addLayout(horizontalLayout_9);


        horizontalLayout_3->addLayout(verticalLayout_3);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy2);
        groupBox->setFlat(true);
        verticalLayout_5 = new QVBoxLayout(groupBox);
        verticalLayout_5->setSpacing(4);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout_5->setContentsMargins(4, 2, 4, 2);
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(8);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        tabWidget_PreProcessing = new QTabWidget(groupBox);
        tabWidget_PreProcessing->setObjectName(QStringLiteral("tabWidget_PreProcessing"));
        tabWidget_Filters = new QWidget();
        tabWidget_Filters->setObjectName(QStringLiteral("tabWidget_Filters"));
        verticalLayout = new QVBoxLayout(tabWidget_Filters);
        verticalLayout->setSpacing(22);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(28, 36, 20, 36);
        radioButton_None = new QRadioButton(tabWidget_Filters);
        radioButton_None->setObjectName(QStringLiteral("radioButton_None"));
        radioButton_None->setFocusPolicy(Qt::NoFocus);
        radioButton_None->setContextMenuPolicy(Qt::NoContextMenu);

        verticalLayout->addWidget(radioButton_None);

        radioButton_Gaussian = new QRadioButton(tabWidget_Filters);
        radioButton_Gaussian->setObjectName(QStringLiteral("radioButton_Gaussian"));
        radioButton_Gaussian->setFocusPolicy(Qt::NoFocus);
        radioButton_Gaussian->setContextMenuPolicy(Qt::NoContextMenu);

        verticalLayout->addWidget(radioButton_Gaussian);

        radioButton_Laplacian = new QRadioButton(tabWidget_Filters);
        radioButton_Laplacian->setObjectName(QStringLiteral("radioButton_Laplacian"));
        radioButton_Laplacian->setFocusPolicy(Qt::NoFocus);

        verticalLayout->addWidget(radioButton_Laplacian);

        tabWidget_PreProcessing->addTab(tabWidget_Filters, QString());
        tabWidget_Resampling = new QWidget();
        tabWidget_Resampling->setObjectName(QStringLiteral("tabWidget_Resampling"));
        tabWidget_Resampling->setLayoutDirection(Qt::RightToLeft);
        verticalLayout_7 = new QVBoxLayout(tabWidget_Resampling);
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(-1, 30, -1, 30);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout->setContentsMargins(10, 12, 10, 12);
        label_x_mm = new QLabel(tabWidget_Resampling);
        label_x_mm->setObjectName(QStringLiteral("label_x_mm"));
        label_x_mm->setLayoutDirection(Qt::LeftToRight);
        label_x_mm->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_x_mm);

        lineEdit_x = new QLineEdit(tabWidget_Resampling);
        lineEdit_x->setObjectName(QStringLiteral("lineEdit_x"));
        sizePolicy2.setHeightForWidth(lineEdit_x->sizePolicy().hasHeightForWidth());
        lineEdit_x->setSizePolicy(sizePolicy2);
        lineEdit_x->setFocusPolicy(Qt::ClickFocus);
        lineEdit_x->setLayoutDirection(Qt::LeftToRight);
        lineEdit_x->setMaxLength(6);
        lineEdit_x->setAlignment(Qt::AlignCenter);
        lineEdit_x->setDragEnabled(true);

        horizontalLayout->addWidget(lineEdit_x);

        label_x = new QLabel(tabWidget_Resampling);
        label_x->setObjectName(QStringLiteral("label_x"));
        label_x->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_x);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 1);
        horizontalLayout->setStretch(2, 1);

        verticalLayout_7->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout_2->setContentsMargins(10, 12, 10, 12);
        label_y_mm = new QLabel(tabWidget_Resampling);
        label_y_mm->setObjectName(QStringLiteral("label_y_mm"));
        label_y_mm->setLayoutDirection(Qt::LeftToRight);
        label_y_mm->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label_y_mm);

        lineEdit_y = new QLineEdit(tabWidget_Resampling);
        lineEdit_y->setObjectName(QStringLiteral("lineEdit_y"));
        sizePolicy2.setHeightForWidth(lineEdit_y->sizePolicy().hasHeightForWidth());
        lineEdit_y->setSizePolicy(sizePolicy2);
        lineEdit_y->setFocusPolicy(Qt::ClickFocus);
        lineEdit_y->setLayoutDirection(Qt::LeftToRight);
        lineEdit_y->setMaxLength(6);
        lineEdit_y->setAlignment(Qt::AlignCenter);
        lineEdit_y->setDragEnabled(true);

        horizontalLayout_2->addWidget(lineEdit_y);

        label_y = new QLabel(tabWidget_Resampling);
        label_y->setObjectName(QStringLiteral("label_y"));
        label_y->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_y);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 1);
        horizontalLayout_2->setStretch(2, 1);

        verticalLayout_7->addLayout(horizontalLayout_2);

        tabWidget_PreProcessing->addTab(tabWidget_Resampling, QString());

        verticalLayout_4->addWidget(tabWidget_PreProcessing);

        groupBox_Families = new QGroupBox(groupBox);
        groupBox_Families->setObjectName(QStringLiteral("groupBox_Families"));
        verticalLayout_6 = new QVBoxLayout(groupBox_Families);
        verticalLayout_6->setSpacing(20);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(30, 32, 20, 10);
        checkBox_Morph = new QCheckBox(groupBox_Families);
        checkBox_Morph->setObjectName(QStringLiteral("checkBox_Morph"));
        checkBox_Morph->setEnabled(false);
        checkBox_Morph->setFocusPolicy(Qt::NoFocus);
        checkBox_Morph->setCheckable(true);

        verticalLayout_6->addWidget(checkBox_Morph);

        checkBox_Intensity = new QCheckBox(groupBox_Families);
        checkBox_Intensity->setObjectName(QStringLiteral("checkBox_Intensity"));
        checkBox_Intensity->setFocusPolicy(Qt::NoFocus);
        checkBox_Intensity->setCheckable(true);
        checkBox_Intensity->setChecked(false);
        checkBox_Intensity->setTristate(false);

        verticalLayout_6->addWidget(checkBox_Intensity);

        checkBox_Statistics = new QCheckBox(groupBox_Families);
        checkBox_Statistics->setObjectName(QStringLiteral("checkBox_Statistics"));
        checkBox_Statistics->setEnabled(true);
        checkBox_Statistics->setFocusPolicy(Qt::NoFocus);

        verticalLayout_6->addWidget(checkBox_Statistics);

        checkBox_Histogram = new QCheckBox(groupBox_Families);
        checkBox_Histogram->setObjectName(QStringLiteral("checkBox_Histogram"));
        checkBox_Histogram->setEnabled(true);
        checkBox_Histogram->setFocusPolicy(Qt::NoFocus);
        checkBox_Histogram->setAutoFillBackground(false);
        checkBox_Histogram->setChecked(false);
        checkBox_Histogram->setTristate(false);

        verticalLayout_6->addWidget(checkBox_Histogram);

        checkBox_GLCM = new QCheckBox(groupBox_Families);
        checkBox_GLCM->setObjectName(QStringLiteral("checkBox_GLCM"));
        checkBox_GLCM->setEnabled(true);
        checkBox_GLCM->setFocusPolicy(Qt::NoFocus);
        checkBox_GLCM->setChecked(false);

        verticalLayout_6->addWidget(checkBox_GLCM);

        checkBox_GLRLM = new QCheckBox(groupBox_Families);
        checkBox_GLRLM->setObjectName(QStringLiteral("checkBox_GLRLM"));
        checkBox_GLRLM->setEnabled(true);
        checkBox_GLRLM->setFocusPolicy(Qt::NoFocus);
        checkBox_GLRLM->setChecked(false);

        verticalLayout_6->addWidget(checkBox_GLRLM);

        checkBox_GLSZM = new QCheckBox(groupBox_Families);
        checkBox_GLSZM->setObjectName(QStringLiteral("checkBox_GLSZM"));
        checkBox_GLSZM->setFocusPolicy(Qt::NoFocus);
        checkBox_GLSZM->setCheckable(true);

        verticalLayout_6->addWidget(checkBox_GLSZM);

        checkBox_GLDZM = new QCheckBox(groupBox_Families);
        checkBox_GLDZM->setObjectName(QStringLiteral("checkBox_GLDZM"));
        checkBox_GLDZM->setEnabled(true);
        checkBox_GLDZM->setFocusPolicy(Qt::NoFocus);
        checkBox_GLDZM->setCheckable(true);

        verticalLayout_6->addWidget(checkBox_GLDZM);

        checkBox_NGTDM = new QCheckBox(groupBox_Families);
        checkBox_NGTDM->setObjectName(QStringLiteral("checkBox_NGTDM"));
        checkBox_NGTDM->setEnabled(false);
        checkBox_NGTDM->setFocusPolicy(Qt::NoFocus);
        checkBox_NGTDM->setCheckable(true);

        verticalLayout_6->addWidget(checkBox_NGTDM);

        checkBox_NGLDM = new QCheckBox(groupBox_Families);
        checkBox_NGLDM->setObjectName(QStringLiteral("checkBox_NGLDM"));
        checkBox_NGLDM->setEnabled(false);
        checkBox_NGLDM->setFocusPolicy(Qt::NoFocus);
        checkBox_NGLDM->setCheckable(true);

        verticalLayout_6->addWidget(checkBox_NGLDM);


        verticalLayout_4->addWidget(groupBox_Families);

        verticalLayout_4->setStretch(0, 1);
        verticalLayout_4->setStretch(1, 3);

        verticalLayout_5->addLayout(verticalLayout_4);

        pushButton_run = new QPushButton(groupBox);
        pushButton_run->setObjectName(QStringLiteral("pushButton_run"));
        pushButton_run->setEnabled(false);
        pushButton_run->setMinimumSize(QSize(0, 24));
        pushButton_run->setFocusPolicy(Qt::NoFocus);

        verticalLayout_5->addWidget(pushButton_run);


        horizontalLayout_3->addWidget(groupBox);

        horizontalLayout_3->setStretch(0, 4);
        horizontalLayout_3->setStretch(1, 1);

        gridLayout_3->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        platformClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(platformClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1235, 22));
        platformClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(platformClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        platformClass->setStatusBar(statusBar);

        retranslateUi(platformClass);

        tabWidget_PreProcessing->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(platformClass);
    } // setupUi

    void retranslateUi(QMainWindow *platformClass)
    {
        platformClass->setWindowTitle(QApplication::translate("platformClass", "Radiomics Feature Extraction", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget_FileDirectory->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("platformClass", "DICOM", Q_NULLPTR));
        groupBox->setTitle(QString());
        radioButton_None->setText(QApplication::translate("platformClass", "None", Q_NULLPTR));
        radioButton_Gaussian->setText(QApplication::translate("platformClass", "Gaussian", Q_NULLPTR));
        radioButton_Laplacian->setText(QApplication::translate("platformClass", "Laplacian", Q_NULLPTR));
        tabWidget_PreProcessing->setTabText(tabWidget_PreProcessing->indexOf(tabWidget_Filters), QApplication::translate("platformClass", "Filter", Q_NULLPTR));
        label_x_mm->setText(QApplication::translate("platformClass", "  mm", Q_NULLPTR));
        label_x->setText(QApplication::translate("platformClass", "    x :", Q_NULLPTR));
        label_y_mm->setText(QApplication::translate("platformClass", "  mm", Q_NULLPTR));
        label_y->setText(QApplication::translate("platformClass", "    y :", Q_NULLPTR));
        tabWidget_PreProcessing->setTabText(tabWidget_PreProcessing->indexOf(tabWidget_Resampling), QApplication::translate("platformClass", "Resampling", Q_NULLPTR));
        groupBox_Families->setTitle(QApplication::translate("platformClass", "Radiomics Feature Family", Q_NULLPTR));
        checkBox_Morph->setText(QApplication::translate("platformClass", "Morphological ", Q_NULLPTR));
        checkBox_Intensity->setText(QApplication::translate("platformClass", "Local Intensity ", Q_NULLPTR));
        checkBox_Statistics->setText(QApplication::translate("platformClass", "Intensity Statistics", Q_NULLPTR));
        checkBox_Histogram->setText(QApplication::translate("platformClass", "Intensity Histogram", Q_NULLPTR));
        checkBox_GLCM->setText(QApplication::translate("platformClass", "GLCM ", Q_NULLPTR));
        checkBox_GLRLM->setText(QApplication::translate("platformClass", "GLRLM ", Q_NULLPTR));
        checkBox_GLSZM->setText(QApplication::translate("platformClass", "GLSZM ", Q_NULLPTR));
        checkBox_GLDZM->setText(QApplication::translate("platformClass", "GLDZM", Q_NULLPTR));
        checkBox_NGTDM->setText(QApplication::translate("platformClass", "NGTDM", Q_NULLPTR));
        checkBox_NGLDM->setText(QApplication::translate("platformClass", "NGLDM", Q_NULLPTR));
        pushButton_run->setText(QApplication::translate("platformClass", "Run", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class platformClass: public Ui_platformClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLATFORM_H
