#ifndef LIMBICEDITOR_MAINWINDOW_H
#define LIMBICEDITOR_MAINWINDOW_H

#include "ui_MainWindow.h"
#include "MaterialTreeItem.h"
#include <QFileDialog>
#include <filesystem>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QMainWindow *parent = nullptr);
    QString materials_directory;

private slots:
    void on_pushButton_openDirectory_pressed();
    void on_pushButton_saveMat_pressed();
    void on_pushButton_saveAllMat_pressed();
    void on_pushButton_addMat_pressed();
    void on_pushButton_subMat_pressed();

    void on_treeWidget_materials_itemChanged(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow ui;
};

#endif //LIMBICEDITOR_MAINWINDOW_H
