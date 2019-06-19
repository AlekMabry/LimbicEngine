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
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
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
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tabLevelView;
    QOpenGLWidget *openGLWidget;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QTreeWidget *treeWidget;
    QLabel *labelX;
    QLabel *labelY;
    QLabel *labelZ;
    QPlainTextEdit *pTextEdit_X;
    QPlainTextEdit *pTextEdit_Y;
    QPlainTextEdit *pTextEdit_Z;
    QPlainTextEdit *pTextEdit_xAngle;
    QPlainTextEdit *pTextEdit_zAngle;
    QPlainTextEdit *pTextEdit_yAngle;
    QLabel *labelPosition;
    QLabel *labelAngle;
    QWidget *tabAssetView;
    QHBoxLayout *horizontalLayout;
    QFrame *matDirectoryframe;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_materialsTitle;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_subMat;
    QPushButton *pushButton_addMat;
    QTreeWidget *treeWidget_materials;
    QFrame *matEditorFrame;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_shaderFile;
    QLineEdit *lineEdit_shaderFile;
    QLabel *label_textureCount;
    QLabel *label_frameCount;
    QSpinBox *spinBox_textureCount;
    QSpinBox *spinBox_frameCount;
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
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabLevelView = new QWidget();
        tabLevelView->setObjectName(QString::fromUtf8("tabLevelView"));
        openGLWidget = new QOpenGLWidget(tabLevelView);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        openGLWidget->setGeometry(QRect(9, 9, 971, 611));
        scrollArea = new QScrollArea(tabLevelView);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(990, 10, 281, 611));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 277, 607));
        treeWidget = new QTreeWidget(scrollAreaWidgetContents);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setGeometry(QRect(10, 10, 256, 192));
        labelX = new QLabel(scrollAreaWidgetContents);
        labelX->setObjectName(QString::fromUtf8("labelX"));
        labelX->setGeometry(QRect(20, 230, 21, 19));
        labelY = new QLabel(scrollAreaWidgetContents);
        labelY->setObjectName(QString::fromUtf8("labelY"));
        labelY->setGeometry(QRect(20, 250, 21, 19));
        labelZ = new QLabel(scrollAreaWidgetContents);
        labelZ->setObjectName(QString::fromUtf8("labelZ"));
        labelZ->setGeometry(QRect(20, 270, 21, 19));
        pTextEdit_X = new QPlainTextEdit(scrollAreaWidgetContents);
        pTextEdit_X->setObjectName(QString::fromUtf8("pTextEdit_X"));
        pTextEdit_X->setGeometry(QRect(40, 230, 111, 21));
        pTextEdit_Y = new QPlainTextEdit(scrollAreaWidgetContents);
        pTextEdit_Y->setObjectName(QString::fromUtf8("pTextEdit_Y"));
        pTextEdit_Y->setGeometry(QRect(40, 250, 111, 21));
        pTextEdit_Z = new QPlainTextEdit(scrollAreaWidgetContents);
        pTextEdit_Z->setObjectName(QString::fromUtf8("pTextEdit_Z"));
        pTextEdit_Z->setGeometry(QRect(40, 270, 111, 21));
        pTextEdit_xAngle = new QPlainTextEdit(scrollAreaWidgetContents);
        pTextEdit_xAngle->setObjectName(QString::fromUtf8("pTextEdit_xAngle"));
        pTextEdit_xAngle->setGeometry(QRect(150, 230, 111, 21));
        pTextEdit_zAngle = new QPlainTextEdit(scrollAreaWidgetContents);
        pTextEdit_zAngle->setObjectName(QString::fromUtf8("pTextEdit_zAngle"));
        pTextEdit_zAngle->setGeometry(QRect(150, 270, 111, 21));
        pTextEdit_yAngle = new QPlainTextEdit(scrollAreaWidgetContents);
        pTextEdit_yAngle->setObjectName(QString::fromUtf8("pTextEdit_yAngle"));
        pTextEdit_yAngle->setGeometry(QRect(150, 250, 111, 21));
        labelPosition = new QLabel(scrollAreaWidgetContents);
        labelPosition->setObjectName(QString::fromUtf8("labelPosition"));
        labelPosition->setGeometry(QRect(70, 210, 67, 19));
        labelAngle = new QLabel(scrollAreaWidgetContents);
        labelAngle->setObjectName(QString::fromUtf8("labelAngle"));
        labelAngle->setGeometry(QRect(180, 210, 41, 19));
        scrollArea->setWidget(scrollAreaWidgetContents);
        tabWidget->addTab(tabLevelView, QString());
        tabAssetView = new QWidget();
        tabAssetView->setObjectName(QString::fromUtf8("tabAssetView"));
        horizontalLayout = new QHBoxLayout(tabAssetView);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        matDirectoryframe = new QFrame(tabAssetView);
        matDirectoryframe->setObjectName(QString::fromUtf8("matDirectoryframe"));
        matDirectoryframe->setFrameShape(QFrame::StyledPanel);
        matDirectoryframe->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(matDirectoryframe);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new QWidget(matDirectoryframe);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout_3 = new QHBoxLayout(widget);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        label_materialsTitle = new QLabel(widget);
        label_materialsTitle->setObjectName(QString::fromUtf8("label_materialsTitle"));
        QFont font;
        font.setPointSize(14);
        label_materialsTitle->setFont(font);

        horizontalLayout_3->addWidget(label_materialsTitle);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        pushButton_subMat = new QPushButton(widget);
        pushButton_subMat->setObjectName(QString::fromUtf8("pushButton_subMat"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_subMat->sizePolicy().hasHeightForWidth());
        pushButton_subMat->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(pushButton_subMat);

        pushButton_addMat = new QPushButton(widget);
        pushButton_addMat->setObjectName(QString::fromUtf8("pushButton_addMat"));

        horizontalLayout_3->addWidget(pushButton_addMat);


        verticalLayout->addWidget(widget);

        treeWidget_materials = new QTreeWidget(matDirectoryframe);
        treeWidget_materials->setObjectName(QString::fromUtf8("treeWidget_materials"));

        verticalLayout->addWidget(treeWidget_materials);


        horizontalLayout->addWidget(matDirectoryframe);

        matEditorFrame = new QFrame(tabAssetView);
        matEditorFrame->setObjectName(QString::fromUtf8("matEditorFrame"));
        matEditorFrame->setFrameShape(QFrame::StyledPanel);
        matEditorFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(matEditorFrame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_shaderFile = new QLabel(matEditorFrame);
        label_shaderFile->setObjectName(QString::fromUtf8("label_shaderFile"));

        verticalLayout_2->addWidget(label_shaderFile);

        lineEdit_shaderFile = new QLineEdit(matEditorFrame);
        lineEdit_shaderFile->setObjectName(QString::fromUtf8("lineEdit_shaderFile"));

        verticalLayout_2->addWidget(lineEdit_shaderFile);

        label_textureCount = new QLabel(matEditorFrame);
        label_textureCount->setObjectName(QString::fromUtf8("label_textureCount"));

        verticalLayout_2->addWidget(label_textureCount);

        label_frameCount = new QLabel(matEditorFrame);
        label_frameCount->setObjectName(QString::fromUtf8("label_frameCount"));

        verticalLayout_2->addWidget(label_frameCount);

        spinBox_textureCount = new QSpinBox(matEditorFrame);
        spinBox_textureCount->setObjectName(QString::fromUtf8("spinBox_textureCount"));

        verticalLayout_2->addWidget(spinBox_textureCount);

        spinBox_frameCount = new QSpinBox(matEditorFrame);
        spinBox_frameCount->setObjectName(QString::fromUtf8("spinBox_frameCount"));

        verticalLayout_2->addWidget(spinBox_frameCount);

        treeWidget_texTree = new QTreeWidget(matEditorFrame);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QString::fromUtf8("1"));
        treeWidget_texTree->setHeaderItem(__qtreewidgetitem1);
        treeWidget_texTree->setObjectName(QString::fromUtf8("treeWidget_texTree"));

        verticalLayout_2->addWidget(treeWidget_texTree);


        horizontalLayout->addWidget(matEditorFrame);

        tabWidget->addTab(tabAssetView, QString());

        horizontalLayout_2->addWidget(tabWidget);

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
        labelX->setText(QApplication::translate("MainWindow", "X:", nullptr));
        labelY->setText(QApplication::translate("MainWindow", "Y:", nullptr));
        labelZ->setText(QApplication::translate("MainWindow", "Z:", nullptr));
        labelPosition->setText(QApplication::translate("MainWindow", "Position", nullptr));
        labelAngle->setText(QApplication::translate("MainWindow", "Angle", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabLevelView), QApplication::translate("MainWindow", "Level", nullptr));
        label_materialsTitle->setText(QApplication::translate("MainWindow", "Materials", nullptr));
        pushButton_subMat->setText(QApplication::translate("MainWindow", "-", nullptr));
        pushButton_addMat->setText(QApplication::translate("MainWindow", "+", nullptr));
        label_shaderFile->setText(QApplication::translate("MainWindow", "Shader File:", nullptr));
        label_textureCount->setText(QApplication::translate("MainWindow", "Textures:", nullptr));
        label_frameCount->setText(QApplication::translate("MainWindow", "Frames:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabAssetView), QApplication::translate("MainWindow", "Asset Manager", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
