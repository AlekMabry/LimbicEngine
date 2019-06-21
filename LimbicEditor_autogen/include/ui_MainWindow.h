/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLevel_Load;
    QAction *actionLevel_Save_As;
    QAction *actionLevel_Save;
    QAction *actionMaterial_Directory_Load;
    QAction *actionMaterial_Directory_Save_All;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tabLevelView;
    QHBoxLayout *horizontalLayout_2;
    QFrame *frame;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_3;
    QComboBox *comboBox;
    QWidget *tabMaterialView;
    QHBoxLayout *horizontalLayout;
    QFrame *matDirectoryframe;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_openDirectory;
    QPushButton *pushButton_saveMat;
    QPushButton *pushButton_saveAllMat;
    QPushButton *pushButton_subMat;
    QPushButton *pushButton_addMat;
    QTreeWidget *treeWidget_materials;
    QFrame *matEditorFrame;
    QHBoxLayout *horizontalLayout_4;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_shaderFile;
    QLineEdit *lineEdit_shaderFile;
    QLabel *label_textureCount;
    QSpinBox *spinBox_textureCount;
    QLabel *label_frameCount;
    QSpinBox *spinBox_frameCount;
    QCheckBox *checkBox_saveInMaterial;
    QWidget *widget_5;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_materialPreview;
    QComboBox *comboBox_materialPreviewModel;
    QOpenGLWidget *openGLWidget;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_4;
    QTreeWidget *treeWidget_texTree;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1280, 720);
        actionLevel_Load = new QAction(MainWindow);
        actionLevel_Load->setObjectName(QString::fromUtf8("actionLevel_Load"));
        actionLevel_Save_As = new QAction(MainWindow);
        actionLevel_Save_As->setObjectName(QString::fromUtf8("actionLevel_Save_As"));
        actionLevel_Save = new QAction(MainWindow);
        actionLevel_Save->setObjectName(QString::fromUtf8("actionLevel_Save"));
        actionMaterial_Directory_Load = new QAction(MainWindow);
        actionMaterial_Directory_Load->setObjectName(QString::fromUtf8("actionMaterial_Directory_Load"));
        actionMaterial_Directory_Save_All = new QAction(MainWindow);
        actionMaterial_Directory_Save_All->setObjectName(QString::fromUtf8("actionMaterial_Directory_Save_All"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabLevelView = new QWidget();
        tabLevelView->setObjectName(QString::fromUtf8("tabLevelView"));
        horizontalLayout_2 = new QHBoxLayout(tabLevelView);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        frame = new QFrame(tabLevelView);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        horizontalLayout_2->addWidget(frame);

        scrollArea = new QScrollArea(tabLevelView);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1228, 597));
        verticalLayout_3 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        comboBox = new QComboBox(scrollAreaWidgetContents);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        verticalLayout_3->addWidget(comboBox);

        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout_2->addWidget(scrollArea);

        tabWidget->addTab(tabLevelView, QString());
        tabMaterialView = new QWidget();
        tabMaterialView->setObjectName(QString::fromUtf8("tabMaterialView"));
        horizontalLayout = new QHBoxLayout(tabMaterialView);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        matDirectoryframe = new QFrame(tabMaterialView);
        matDirectoryframe->setObjectName(QString::fromUtf8("matDirectoryframe"));
        matDirectoryframe->setMaximumSize(QSize(640, 16777215));
        matDirectoryframe->setFrameShape(QFrame::StyledPanel);
        matDirectoryframe->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(matDirectoryframe);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new QWidget(matDirectoryframe);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout_3 = new QHBoxLayout(widget);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        pushButton_openDirectory = new QPushButton(widget);
        pushButton_openDirectory->setObjectName(QString::fromUtf8("pushButton_openDirectory"));

        horizontalLayout_3->addWidget(pushButton_openDirectory);

        pushButton_saveMat = new QPushButton(widget);
        pushButton_saveMat->setObjectName(QString::fromUtf8("pushButton_saveMat"));
        pushButton_saveMat->setEnabled(false);

        horizontalLayout_3->addWidget(pushButton_saveMat);

        pushButton_saveAllMat = new QPushButton(widget);
        pushButton_saveAllMat->setObjectName(QString::fromUtf8("pushButton_saveAllMat"));
        pushButton_saveAllMat->setEnabled(false);

        horizontalLayout_3->addWidget(pushButton_saveAllMat);

        pushButton_subMat = new QPushButton(widget);
        pushButton_subMat->setObjectName(QString::fromUtf8("pushButton_subMat"));
        pushButton_subMat->setEnabled(false);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_subMat->sizePolicy().hasHeightForWidth());
        pushButton_subMat->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(pushButton_subMat);

        pushButton_addMat = new QPushButton(widget);
        pushButton_addMat->setObjectName(QString::fromUtf8("pushButton_addMat"));
        pushButton_addMat->setEnabled(false);
        sizePolicy.setHeightForWidth(pushButton_addMat->sizePolicy().hasHeightForWidth());
        pushButton_addMat->setSizePolicy(sizePolicy);
        pushButton_addMat->setMinimumSize(QSize(0, 0));

        horizontalLayout_3->addWidget(pushButton_addMat);


        verticalLayout->addWidget(widget);

        treeWidget_materials = new QTreeWidget(matDirectoryframe);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget_materials->setHeaderItem(__qtreewidgetitem);
        treeWidget_materials->setObjectName(QString::fromUtf8("treeWidget_materials"));

        verticalLayout->addWidget(treeWidget_materials);


        horizontalLayout->addWidget(matDirectoryframe);

        matEditorFrame = new QFrame(tabMaterialView);
        matEditorFrame->setObjectName(QString::fromUtf8("matEditorFrame"));
        matEditorFrame->setEnabled(false);
        matEditorFrame->setFrameShape(QFrame::StyledPanel);
        matEditorFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(matEditorFrame);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        widget_2 = new QWidget(matEditorFrame);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, -1);
        widget_4 = new QWidget(widget_2);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget_4->sizePolicy().hasHeightForWidth());
        widget_4->setSizePolicy(sizePolicy1);
        verticalLayout_5 = new QVBoxLayout(widget_4);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_shaderFile = new QLabel(widget_4);
        label_shaderFile->setObjectName(QString::fromUtf8("label_shaderFile"));
        sizePolicy1.setHeightForWidth(label_shaderFile->sizePolicy().hasHeightForWidth());
        label_shaderFile->setSizePolicy(sizePolicy1);
        label_shaderFile->setMinimumSize(QSize(0, 0));
        label_shaderFile->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout_5->addWidget(label_shaderFile);

        lineEdit_shaderFile = new QLineEdit(widget_4);
        lineEdit_shaderFile->setObjectName(QString::fromUtf8("lineEdit_shaderFile"));

        verticalLayout_5->addWidget(lineEdit_shaderFile);

        label_textureCount = new QLabel(widget_4);
        label_textureCount->setObjectName(QString::fromUtf8("label_textureCount"));
        sizePolicy1.setHeightForWidth(label_textureCount->sizePolicy().hasHeightForWidth());
        label_textureCount->setSizePolicy(sizePolicy1);

        verticalLayout_5->addWidget(label_textureCount);

        spinBox_textureCount = new QSpinBox(widget_4);
        spinBox_textureCount->setObjectName(QString::fromUtf8("spinBox_textureCount"));

        verticalLayout_5->addWidget(spinBox_textureCount);

        label_frameCount = new QLabel(widget_4);
        label_frameCount->setObjectName(QString::fromUtf8("label_frameCount"));
        sizePolicy1.setHeightForWidth(label_frameCount->sizePolicy().hasHeightForWidth());
        label_frameCount->setSizePolicy(sizePolicy1);

        verticalLayout_5->addWidget(label_frameCount);

        spinBox_frameCount = new QSpinBox(widget_4);
        spinBox_frameCount->setObjectName(QString::fromUtf8("spinBox_frameCount"));

        verticalLayout_5->addWidget(spinBox_frameCount);

        checkBox_saveInMaterial = new QCheckBox(widget_4);
        checkBox_saveInMaterial->setObjectName(QString::fromUtf8("checkBox_saveInMaterial"));

        verticalLayout_5->addWidget(checkBox_saveInMaterial);


        verticalLayout_2->addWidget(widget_4);

        widget_5 = new QWidget(widget_2);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        verticalLayout_6 = new QVBoxLayout(widget_5);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label_materialPreview = new QLabel(widget_5);
        label_materialPreview->setObjectName(QString::fromUtf8("label_materialPreview"));
        sizePolicy1.setHeightForWidth(label_materialPreview->sizePolicy().hasHeightForWidth());
        label_materialPreview->setSizePolicy(sizePolicy1);

        verticalLayout_6->addWidget(label_materialPreview);

        comboBox_materialPreviewModel = new QComboBox(widget_5);
        comboBox_materialPreviewModel->addItem(QString());
        comboBox_materialPreviewModel->addItem(QString());
        comboBox_materialPreviewModel->addItem(QString());
        comboBox_materialPreviewModel->setObjectName(QString::fromUtf8("comboBox_materialPreviewModel"));

        verticalLayout_6->addWidget(comboBox_materialPreviewModel);

        openGLWidget = new QOpenGLWidget(widget_5);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy2);
        openGLWidget->setMinimumSize(QSize(0, 160));

        verticalLayout_6->addWidget(openGLWidget);


        verticalLayout_2->addWidget(widget_5);


        horizontalLayout_4->addWidget(widget_2);

        widget_3 = new QWidget(matEditorFrame);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        verticalLayout_4 = new QVBoxLayout(widget_3);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        treeWidget_texTree = new QTreeWidget(widget_3);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QString::fromUtf8("1"));
        treeWidget_texTree->setHeaderItem(__qtreewidgetitem1);
        treeWidget_texTree->setObjectName(QString::fromUtf8("treeWidget_texTree"));

        verticalLayout_4->addWidget(treeWidget_texTree);


        horizontalLayout_4->addWidget(widget_3);


        horizontalLayout->addWidget(matEditorFrame);

        tabWidget->addTab(tabMaterialView, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1280, 27));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menuFile->addAction(actionLevel_Load);
        menuFile->addAction(actionLevel_Save_As);
        menuFile->addAction(actionLevel_Save);
        menuFile->addSeparator();
        menuFile->addAction(actionMaterial_Directory_Load);
        menuFile->addAction(actionMaterial_Directory_Save_All);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Limbic Editor", nullptr));
        actionLevel_Load->setText(QApplication::translate("MainWindow", "Level Load", nullptr));
        actionLevel_Save_As->setText(QApplication::translate("MainWindow", "Level Save As", nullptr));
        actionLevel_Save->setText(QApplication::translate("MainWindow", "Level Save", nullptr));
        actionMaterial_Directory_Load->setText(QApplication::translate("MainWindow", "Material Directory Load", nullptr));
        actionMaterial_Directory_Save_All->setText(QApplication::translate("MainWindow", "Material Directory Save All", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabLevelView), QApplication::translate("MainWindow", "Level", nullptr));
        pushButton_openDirectory->setText(QApplication::translate("MainWindow", "Open Directory", nullptr));
        pushButton_saveMat->setText(QApplication::translate("MainWindow", "Save", nullptr));
        pushButton_saveAllMat->setText(QApplication::translate("MainWindow", "Save All", nullptr));
        pushButton_subMat->setText(QApplication::translate("MainWindow", "-", nullptr));
        pushButton_addMat->setText(QApplication::translate("MainWindow", "+", nullptr));
        label_shaderFile->setText(QApplication::translate("MainWindow", "Shader File:", nullptr));
        label_textureCount->setText(QApplication::translate("MainWindow", "Textures:", nullptr));
        label_frameCount->setText(QApplication::translate("MainWindow", "Frames:", nullptr));
        checkBox_saveInMaterial->setText(QApplication::translate("MainWindow", "Save textures in file", nullptr));
        label_materialPreview->setText(QApplication::translate("MainWindow", "Preview:", nullptr));
        comboBox_materialPreviewModel->setItemText(0, QApplication::translate("MainWindow", "Sphere", nullptr));
        comboBox_materialPreviewModel->setItemText(1, QApplication::translate("MainWindow", "Cube", nullptr));
        comboBox_materialPreviewModel->setItemText(2, QApplication::translate("MainWindow", "Plane", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(tabMaterialView), QApplication::translate("MainWindow", "Materials", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
