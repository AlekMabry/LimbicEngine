#ifndef LIMBICEDITOR_MAINWINDOW_H
#define LIMBICEDITOR_MAINWINDOW_H

#include "ui_MainWindow.h"
#include "MaterialTreeItem.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QMainWindow *parent = nullptr);

private slots:
    void on_pushButton_addMat_pressed();
    void on_pushButton_subMat_pressed();

private:
    Ui::MainWindow ui;
};

#endif //LIMBICEDITOR_MAINWINDOW_H
