#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    QSettings settings;

    if (settings.contains("Editor/AssetImportDirectory"))
        ui.lineEdit_assetImportDirectory->setText(
            settings.value("Editor/AssetImportDirectory").toString());

    connect(ui.toolButton_assetImportDirectory,
            SIGNAL(clicked()),
            this, SLOT(setAssetImportDirectory()));

    if (settings.contains("Editor/AssetExportDirectory"))
        ui.lineEdit_assetExportDirectory->setText(
            settings.value("Editor/AssetExportDirectory").toString());

    connect(ui.toolButton_assetExportDirectory,
            SIGNAL(clicked()),
            this,
            SLOT(setAssetExportDirectory()));

    connect(ui.pushButton_save, SIGNAL(clicked()), this, SLOT(saveSettings()));
    connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(close()));
}

void SettingsDialog::setAssetImportDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(this,
        tr("Asset Import Directory"), "",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    ui.lineEdit_assetImportDirectory->setText(dir);
    ui.pushButton_save->setEnabled(true);
}

void SettingsDialog::setAssetExportDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("Asset Export Directory"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);

    ui.lineEdit_assetExportDirectory->setText(dir);
    ui.pushButton_save->setEnabled(true);
}

void SettingsDialog::saveSettings()
{
    QSettings settings;

    settings.setValue("Editor/AssetImportDirectory", ui.lineEdit_assetImportDirectory->text());
    settings.setValue("Editor/AssetExportDirectory", ui.lineEdit_assetExportDirectory->text());

    ui.pushButton_save->setEnabled(false);
}