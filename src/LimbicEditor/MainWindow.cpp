#include "MainWindow.h"
#include <iostream>

MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void MainWindow::on_pushButton_openDirectory_pressed()
{
    // todo - Check if there are any unsaved files and ask the user if they want to save them first

    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Materials Directory"), ".");

    // Check if a selection has been made
    if (fileName.isEmpty())
    {
        return;
    }
    else
    {
        // todo - Remove existing opened files from the tree

        std::string path = fileName.toStdString();
        for (const auto & entry : std::filesystem::directory_iterator(path))
        {
            std::string material_string = entry.path();

            // Confirm that file is a material file
            // todo - Recursively load directories
            if (material_string.substr(material_string.length()-4) == ".lmf")
            {
                MaterialTreeItem *materialItem = new MaterialTreeItem;

                // Remove filename from path and extension
                std::string fname = material_string.substr(
                        material_string.find_last_of("/\\")+1, material_string.length());
                std::string materialName = fname.substr(0, fname.length()-4);

                materialItem->setText(0, QString::fromUtf8(materialName.data(), materialName.length()));
                materialItem->setFlags(materialItem->flags() | Qt::ItemIsEditable);
                materialItem->file_path = material_string;
                materialItem->new_file_path = material_string;

                ui.treeWidget_materials->addTopLevelItem(materialItem);
            }
        }

        // The material creation buttons are enabled after a directory is chosen
        ui.pushButton_addMat->setEnabled(true);
        ui.pushButton_subMat->setEnabled(true);
        ui.pushButton_saveMat->setEnabled(true);
        ui.pushButton_saveAllMat->setEnabled(true);

    }
}

void MainWindow::on_pushButton_saveMat_pressed()
{

}

void MainWindow::on_pushButton_saveAllMat_pressed()
{

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


void MainWindow::on_treeWidget_materials_itemChanged(QTreeWidgetItem *item, int column) {
    MaterialTreeItem* matItem = dynamic_cast<MaterialTreeItem*>(item);
    if (matItem != nullptr)
    {
        // todo - Check if the new name has the correct file ending

        QString itemName_qstr = matItem->text(0);
        std::string filePathName = matItem->file_path;      // This is the entire old filename
        std::string filePath = filePathName.substr(0, filePathName.find_last_of("/\\")+1);    // Cull old filename
        filePath = filePath+itemName_qstr.toStdString();
        filePath = filePath+".lmf";
        matItem->new_file_path = filePath;
        std::cout << "OLD FILEPATH: " << filePathName << "\n";
        std::cout << "NEW FILEPATH: " << filePath << "\n";
    }

}