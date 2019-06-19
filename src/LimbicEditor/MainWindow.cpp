#include "MainWindow.h"
#include <iostream>

MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void MainWindow::on_pushButton_addMat_pressed()
{

    MaterialTreeItem *materialItem = new MaterialTreeItem;
    materialItem->setText(0, "Untitled");
    materialItem->setFlags(materialItem->flags() | Qt::ItemIsEditable);

    ui.treeWidget_materials->addTopLevelItem(materialItem);

    std::cout << "new material added \n";
}

void MainWindow::on_pushButton_subMat_pressed()
{
    QTreeWidgetItem *currentMat = ui.treeWidget_materials->currentItem();
    delete currentMat;
}